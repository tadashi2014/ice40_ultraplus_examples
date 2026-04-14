/*
 * Host-side SPI demo for the COMET II IN/OUT sample.
 *
 * Protocol used by comet2_spi_inout_echo.casl:
 *   host -> COMET : opcode 0x10, val[1] = ASCII byte
 *   COMET -> host : byte0   0x20, byte1 = ASCII byte
 *
 * This file loads the COMET echo firmware, starts the CPU, then performs the
 * IN/OUT echo exchange.
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "../../spi/spi_host/spi_lib.h"

#define SPI_INIT            0x01
#define SPI_SEND_FIRMWARE   0x02
#define SPI_START_CPU       0x03
#define COMET_CMD_IN_CHAR   0x10
#define COMET_RSP_OUT_CHAR  0x20
#define FIRMWARE_PATH       "../comet2_cpu/spi_test/comet2_inout_main.bin"
#define MAX_FIRMWARE_BYTES  (1 << 14)

static void load_and_start_comet_firmware(void)
{
   uint8_t no_param[3] = {0x00, 0x00, 0x00};
   uint8_t spi_status = 0;
   uint8_t firmware[MAX_FIRMWARE_BYTES];
   FILE *file;
   size_t firmware_size;

   file = fopen(FIRMWARE_PATH, "rb");
   if (file == NULL) {
      fprintf(stderr, "failed to open firmware image: %s\n", FIRMWARE_PATH);
      exit(1);
   }

   firmware_size = fread(firmware, sizeof(uint8_t), MAX_FIRMWARE_BYTES, file);
   fclose(file);

   if ((firmware_size & 1u) != 0u) {
      fprintf(stderr, "firmware size must be even, got %zu bytes\n", firmware_size);
      exit(1);
   }

   sleep(1);
   spi_send(SPI_INIT, no_param, NULL);
   printf("loaded firmware image: %zu bytes\n", firmware_size);

   for (size_t i = 0; i < firmware_size; i += 2) {
      uint8_t packet[3] = {firmware[i], firmware[i + 1], 0x00};
      if (spi_send(SPI_SEND_FIRMWARE, packet, &spi_status) != 0) {
         fprintf(stderr, "firmware transfer failed at byte %zu\n", i);
         exit(1);
      }
      usleep(1000);
   }

   sleep(1);
   spi_send(SPI_START_CPU, no_param, &spi_status);
   sleep(1);
}

static int comet_send_in_char(uint8_t ch)
{
   uint8_t status = 0;

   /*
    * CPU sees:
    *   C001 low byte  = opcode
    *   C002 low byte  = val[1]
    * so the character is placed in val[1].
    */
   uint8_t payload[3] = {0x00, ch, 0x00};
   return spi_send(COMET_CMD_IN_CHAR, payload, &status);
}

static int comet_read_out_char(uint8_t *ch)
{
   uint8_t status = 0;
   uint8_t data[3] = {0, 0, 0};

   if (!spi_read(data, &status)) {
      return -1;
   }
   if (data[0] != COMET_RSP_OUT_CHAR) {
      fprintf(stderr, "unexpected packet: %02x %02x %02x (status=%02x)\n",
              data[0], data[1], data[2], status);
      return -2;
   }

   *ch = data[1];
   return 0;
}

int main(int argc, char **argv)
{
   const char *message = "HELLO\n";
   size_t i;

   if (argc >= 2) {
      message = argv[1];
   }

   spi_init();
   printf("init..\n");
   load_and_start_comet_firmware();

   printf("sending: %s", message);
   if (message[strlen(message) - 1] != '\n') {
      printf("\n");
   }

   for (i = 0; message[i] != '\0'; ++i) {
      uint8_t rx = 0;

      if (comet_send_in_char((uint8_t)message[i]) != 0) {
         fprintf(stderr, "failed to send char 0x%02x at index %zu\n",
                 (unsigned char)message[i], i);
         return 1;
      }

      /*
       * The sample firmware echoes one character back immediately after SVC 1
       * completes, so reading right away keeps the host logic simple.
       */
      if (comet_read_out_char(&rx) != 0) {
         fprintf(stderr, "failed to read echoed char at index %zu\n", i);
         return 1;
      }

      printf("echo[%zu] = 0x%02x '%c'\n",
             i, rx, (rx >= 0x20 && rx <= 0x7e) ? rx : '.');
      usleep(1000);
   }

   return 0;
}
