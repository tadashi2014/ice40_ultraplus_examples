/*
   host software running on the computer,
   used to send the firmware to the riscv cpu and communicates with it
   sends commands and read answers via SPI
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include "../../spi/spi_host/spi_lib.h"

#define SPI_NOP 0x00
#define SPI_INIT 0x01
#define SPI_SEND_FIRMWARE 0x02
#define SPI_START_CPU 0x03
#define SPI_SET_LED 0x04
#define SPI_RUN_GRADIENT 0x05
#define SPI_FIBONACCI 0x06
#define SPI_POW 0x07
#define SPI_MATRIX_MULT 0x08

int main()
{
   spi_init();

   uint8_t no_param[3] = {0x0, 0x0, 0x0};
   uint8_t spi_status = 0;
   uint8_t data_read[3];
   // val[0] が FPGA受信データの bits[15:8] に入る (LSBファースト受信)
   // firmware側: value = read>>8 で取得 → bits[15:8] が value の下位バイト
   // よって val[0] に色ビット(bit0=R,bit1=G,bit2=B)を入れる
   uint8_t val_led_red[3]    = {0x1, 0x0, 0x0};
   uint8_t val_led_green[3]  = {0x2, 0x0, 0x0};
   uint8_t val_led_yellow[3] = {0x3, 0x0, 0x0};
   uint8_t val_led_white[3]  = {0x7, 0x0, 0x0};

   //matrix multiplication
   //matrix index is | 0 1 |
   //                | 2 3 |
   uint32_t mat0[4] = {1, 2, 3, 1};
   uint32_t mat1[4] = {10, 0, 5, 2};

   #define NB_ELEM_READ (1<<14)
   uint8_t buffer_read[NB_ELEM_READ];
   FILE *file;

   file = fopen("./firmware/main", "rb");
   if(file == NULL)
   {
      printf("error opening the file ./firmware/main\n");
      printf("hint: run 'make' inside host_server/firmware/ to compile the firmware\n");
      exit(1);
   }
   int firmware_read_size = fread(buffer_read, sizeof(uint8_t), NB_ELEM_READ, file);
   fclose(file);

   if(firmware_read_size == 0)
   {
      printf("error: firmware file is empty (0 bytes).\n");
      printf("hint: run 'make' inside host_server/firmware/ to compile the firmware\n");
      exit(1);
   }

   printf("size firmware: %d bytes\n", firmware_read_size);
   printf("firmware[0..3]: 0x%02x 0x%02x 0x%02x 0x%02x  (expected for default start.S: 0x37 0x41 0x00 0x00 = lui sp,0x4)\n",
      firmware_read_size > 0 ? buffer_read[0] : 0,
      firmware_read_size > 1 ? buffer_read[1] : 0,
      firmware_read_size > 2 ? buffer_read[2] : 0,
      firmware_read_size > 3 ? buffer_read[3] : 0);

   spi_send(SPI_INIT, no_param, NULL); // init
   printf("sent SPI_INIT\n");

   //sends the riscv firmware to the fpga
   int load_failures = 0;
   for (size_t i = 0; i < firmware_read_size; i+=2)
   {
      uint8_t b0 = buffer_read[i];
      uint8_t b1 = (i + 1 < (size_t)firmware_read_size) ? buffer_read[i+1] : 0x00;
      uint8_t array_send[3] = {b0, b1, 0x0};
      usleep(1000);
      printf("sending %lu of %d\n", i, firmware_read_size);
      int result = spi_send(SPI_SEND_FIRMWARE, array_send, &spi_status);
      if(result != 0)
      {
         load_failures++;
         printf("too many retries at offset %lu (b0=0x%02x b1=0x%02x)\n", i, b0, b1);
      }
      else
      {
         printf("done\n");
      }
   }

   if(load_failures > 0)
   {
      printf("WARNING: %d firmware packet(s) failed — firmware may be incomplete!\n", load_failures);
      printf("         spi_debug LED_G will light even if the SPI layer works.\n");
   }
   else
   {
      printf("all %d firmware packets sent successfully\n", (firmware_read_size + 1) / 2);
   }

   sleep(2);

   printf("start cpu\n");
   spi_send(SPI_START_CPU, no_param, &spi_status);

   sleep(1);

   spi_send(SPI_SET_LED, val_led_red, &spi_status);
   printf("sent red color, status: 0x%x\n", spi_status);

   sleep(1);

   spi_send(SPI_SET_LED, val_led_green, &spi_status);
   printf("sent green color, status: 0x%x\n", spi_status);

   sleep(1);
   spi_send(SPI_SET_LED, val_led_yellow, &spi_status);
   printf("sent yellow color, status: 0x%x\n", spi_status);

   sleep(1);
   spi_send(SPI_SET_LED, val_led_white, &spi_status);
   printf("sent white color, status: 0x%x\n", spi_status);

   sleep(1);
   spi_send24b(SPI_POW, 125, &spi_status);
   sleep(1); // CPUの計算を待つ
   spi_read(data_read, &spi_status);
   printf("calulating 125^2 read: %d, status: 0x%x\n", data_read[0] + (data_read[1]<<8) + (data_read[2]<<16), spi_status);

   sleep(1);
   spi_send24b(SPI_FIBONACCI, 20, &spi_status); //a fibo too big will make a stack overflow
   sleep(1); // CPUの計算を待つ
   spi_read(data_read, &spi_status);
   printf("calculating fib(20) read: %d, status: 0x%x\n", data_read[0] + (data_read[1]<<8) + (data_read[2]<<16), spi_status);

   sleep(1);
   printf("sending the matrices\n");
   for (size_t i = 0; i < 4; i++) {
      spi_send24b(SPI_MATRIX_MULT, mat0[i], &spi_status);
      usleep(1000);
      printf("sent %i, status: 0x%x\n", mat0[i], spi_status);
   }

   for (size_t i = 0; i < 4; i++) {
      spi_send24b(SPI_MATRIX_MULT, mat1[i], &spi_status);
      usleep(10000);
      printf("sent %i, status: 0x%x\n", mat1[i], spi_status);
   }

   printf("reading the multiplied matrix\n");
   for (size_t i = 0; i < 4; i++) {
      spi_read(data_read, &spi_status);
      printf("received %d, status: 0x%x\n", data_read[0] + (data_read[1]<<8) + (data_read[2]<<16), spi_status);
   }

   sleep(1);
   spi_send24b(SPI_RUN_GRADIENT, 0, &spi_status);
   printf("sent gradient, status: 0x%x\n", spi_status);

   sleep(8); //time to finish the gradient

   return 0;
}
