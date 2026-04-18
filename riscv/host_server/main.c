/*
   host software running on the computer,
   used to send the firmware to the riscv cpu and communicates with it
   sends commands and read answers via SPI
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
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

int main(int argc, char **argv)
{
   const char *firmware_path = "./firmware/main";
   int load_only = 0;

   for (int i = 1; i < argc; ++i)
   {
      if ((strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--firmware") == 0) && (i + 1) < argc)
      {
         firmware_path = argv[++i];
      }
      else if (strcmp(argv[i], "--load-only") == 0)
      {
         load_only = 1;
      }
      else
      {
         fprintf(stderr, "usage: %s [-f firmware_path] [--load-only]\n", argv[0]);
         return 1;
      }
   }

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

   file = fopen(firmware_path, "rb");
   if(file == NULL)
   {
      printf("error opening the file %s\n", firmware_path);
      exit(1);
   }
   int firmware_read_size = fread(buffer_read, sizeof(uint8_t), NB_ELEM_READ, file);

   sleep(1); // タイマー発火後 SPI が安定するまで待つ
   spi_send(SPI_INIT, no_param, NULL); // init

   printf("sent\n");

   printf("size firmware: %d\n", firmware_read_size);

   //sends the riscv firmware to the fpga
   for (size_t i = 0; i < firmware_read_size; i+=2)
   {
      uint8_t array_send[3] = {buffer_read[i], buffer_read[i+1], 0x0};
      usleep(1000);
      printf("sending %lu of %d\n", i, firmware_read_size);
      int result = spi_send(SPI_SEND_FIRMWARE, array_send, &spi_status);
      if(result != 0)
      {
         printf("too many retries\n");
      }
      else
      {
         printf("done\n");
      }
   }

   sleep(2);

   printf("start cpu\n");
   spi_send(SPI_START_CPU, no_param, &spi_status);

   sleep(1);

   if (load_only)
   {
      printf("firmware loaded and cpu started (%s)\n", firmware_path);
      return 0;
   }

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
