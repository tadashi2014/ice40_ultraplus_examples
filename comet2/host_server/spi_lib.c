#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "spi_lib.h"

static struct ftdi_context ftdic;
static unsigned char ftdi_latency;

static void send_byte(uint8_t data)
{
  int rc = ftdi_write_data(&ftdic, &data, 1);
  if (rc != 1) {
     fprintf(stderr, "Write error (single byte, rc=%d, expected %d) data: 0x%x.\n", rc, 1, data);
     exit(2);
  }
}

static uint8_t recv_byte()
{
  uint8_t data;
  while (1) {
     int rc = ftdi_read_data(&ftdic, &data, 1);
     if (rc < 0) {
        fprintf(stderr, "Read error.\n");
        exit(2);
     }
     if (rc == 1)
        break;
     usleep(100);
  }
  return data;
}

static uint8_t xfer_spi_bits(uint8_t data, int n)
{
  if (n < 1)
     return 0;

  send_byte(MC_DATA_IN | MC_DATA_OUT | MC_DATA_BITS | MC_DATA_LSB);
  send_byte(n - 1);
  send_byte(data);
  send_byte(MC_FLUSH);
  return recv_byte();
}

static void xfer_spi(uint8_t *data, int n)
{
  if (n < 1)
     return;

  send_byte(MC_DATA_IN | MC_DATA_OUT | MC_DATA_LSB | MC_DATA_OCN);
  send_byte(n - 1);
  send_byte((n - 1) >> 8);

  if (ftdi_write_data(&ftdic, data, n) != n) {
     fprintf(stderr, "Write error (chunk, expected %d).\n", n);
     exit(2);
  }

  send_byte(MC_FLUSH);

  for (int i = 0; i < n; i++)
     data[i] = recv_byte();
}

static void set_gpio(int slavesel_b, int creset_b)
{
  uint8_t gpio = 0;

  if (slavesel_b)
     gpio |= 0x10;
  if (creset_b)
     gpio |= 0x80;

  send_byte(MC_SETB_LOW);
  send_byte(gpio);
  send_byte(0x93);
}

static void sram_chip_select()
{
  set_gpio(0, 1);
}

int spi_init()
{
   enum ftdi_interface ifnum = INTERFACE_A;

   if (ftdi_init(&ftdic) != 0) {
      fprintf(stderr, "couldn't initialize ftdi\n");
      return 1;
   }

   if (ftdi_set_interface(&ftdic, ifnum) != 0) {
      fprintf(stderr, "couldn't initialize ftdi interface\n");
      return 1;
   }

   if (ftdi_usb_open(&ftdic, 0x0403, 0x6010) && ftdi_usb_open(&ftdic, 0x0403, 0x6014)) {
      fprintf(stderr, "Can't find iCE FTDI USB device (vendor_id 0x0403, device_id 0x6010 or 0x6014).\n");
      return 1;
   }

   if (ftdi_usb_reset(&ftdic) || ftdi_tcioflush(&ftdic)) {
      fprintf(stderr, "Failed to reset/purge iCE FTDI USB device.\n");
      return 1;
   }

   if (ftdi_get_latency_timer(&ftdic, &ftdi_latency) < 0) {
      fprintf(stderr, "Failed to get latency timer (%s).\n", ftdi_get_error_string(&ftdic));
      return 1;
   }

   if (ftdi_set_latency_timer(&ftdic, 1) < 0) {
      fprintf(stderr, "Failed to set latency timer (%s).\n", ftdi_get_error_string(&ftdic));
      return 1;
   }

   if (ftdi_set_bitmode(&ftdic, 0xff, BITMODE_MPSSE) < 0) {
      fprintf(stderr, "Failed to set BITMODE_MPSSE on iCE FTDI USB device.\n");
      return 1;
   }

   send_byte(MC_TCK_D5);
   send_byte(MC_SET_CLK_DIV);
   send_byte(0);
   send_byte(0x01);
   usleep(100);
   sram_chip_select();
   usleep(2000);
   return 0;
}

int spi_send(uint8_t cmd, uint8_t val[3], uint8_t *status)
{
   uint8_t to_send[] = {cmd, val[0], val[1], val[2]};
   uint8_t status_recv = 0;
   uint32_t retries = 0;

   do {
      xfer_spi(to_send, 4);
      status_recv = to_send[0];
      retries++;
   } while (retries < 100 && (status_recv & STATUS_FPGA_RECV_MASK) == 0);

   if (status != NULL)
      *status = status_recv;
   return retries >= 100;
}

int spi_send3(uint8_t cmd, uint8_t val0, uint8_t val1, uint8_t val2, uint8_t *status)
{
   uint8_t param[3] = {val0, val1, val2};
   return spi_send(cmd, param, status);
}

int spi_send24b(uint8_t cmd, uint32_t val24b, uint8_t *status)
{
   uint8_t param[3] = {val24b & 0xff, (val24b >> 8) & 0xff, (val24b >> 16) & 0xff};
   return spi_send(cmd, param, status);
}

int spi_send_batch_packets(const uint8_t *packets, size_t packet_count, uint8_t *last_status)
{
   size_t bytes;
   uint8_t *txrx;

   if (packet_count == 0) {
      if (last_status != NULL)
         *last_status = 0;
      return 0;
   }

   bytes = packet_count * 4u;
   txrx = (uint8_t *)malloc(bytes);
   if (txrx == NULL) {
      fprintf(stderr, "out of memory while preparing SPI batch transfer.\n");
      return 1;
   }

   for (size_t i = 0; i < bytes; i++)
      txrx[i] = packets[i];

   xfer_spi(txrx, (int)bytes);

   for (size_t i = 0; i < packet_count; i++) {
      uint8_t status = txrx[i * 4u];
      if ((status & STATUS_FPGA_RECV_MASK) == 0) {
         if (last_status != NULL)
            *last_status = status;
         free(txrx);
         return 1;
      }
   }

   if (last_status != NULL)
      *last_status = txrx[(packet_count - 1u) * 4u];

   free(txrx);
   return 0;
}

int spi_read(uint8_t val[3], uint8_t *status)
{
   uint8_t nop_command[] = {0x00, 0x00, 0x00, 0x00};
   uint8_t status_recv = 0;
   uint32_t retries = 0;
   uint32_t max_retries = 50;

   do {
      usleep(2);
      xfer_spi(nop_command, 4);
      status_recv = nop_command[0];
      retries++;
   } while (retries < max_retries && (status_recv & STATUS_FPGA_SEND_MASK) == 0);

   val[0] = nop_command[1];
   val[1] = nop_command[2];
   val[2] = nop_command[3];

   if (status != NULL)
      *status = status_recv;

   return retries < max_retries;
}
