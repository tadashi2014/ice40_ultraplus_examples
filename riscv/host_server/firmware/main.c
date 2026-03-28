// void main() { while(1); }
/*
   program to run on the riscv cpu
*/

#include <math.h>

#define SPI_ADDRESS  0x8000
#define GPIO_ADDRESS 0x8100

#define MATRIX_SIDE_SIZE 2
#define MATRIX_TOTAL_SIZE (MATRIX_SIDE_SIZE*MATRIX_SIDE_SIZE)

typedef unsigned int uint;

void set_led(int *gpio_addr, int rgb24)
{
   *gpio_addr = rgb24 & 0x7;  // 下位3bit: bit0=R, bit1=G, bit2=B
}

uint fib(uint n)
{
   if(n == 0)
      return 0;
   else if(n <= 2)
      return 1;
   else
      return fib(n-2)+fib(n-1);
}

//create a soft gradient on the leds, using a soft pwm
void gradient(int *gpio_addr)
{
   int grad_speed = 256;
   for (size_t col = 1; col < 8; col++)
   {
      for (size_t i = 0; i < grad_speed; i++)
      {
         for (size_t j = 0; j < grad_speed; j++)
         {
            if(i > j)
            {
               *gpio_addr = col;
            }
            else
            {
               *gpio_addr = 0;
            }
         }
      }
   }
}

//most naive matrix mult possible in O^3
void matrix_mult(int *mat0, int *mat1, int *mat_result){
   for (int i = 0; i < MATRIX_SIDE_SIZE; i++) {
      for (int j = 0; j < MATRIX_SIDE_SIZE; j++) {
         mat_result[j*MATRIX_SIDE_SIZE+i] = 0;
         for (int k = 0; k < MATRIX_SIDE_SIZE; k++) {
            mat_result[j*MATRIX_SIDE_SIZE+i] += mat0[j*MATRIX_SIDE_SIZE+k]*mat1[k*MATRIX_SIDE_SIZE+i];
         }
      }
   }
}

void main()
{
   int *spi_addr  = (int*)SPI_ADDRESS;
   int *gpio_addr = (int*)GPIO_ADDRESS; //bit 0=R, 1=G, 2=B

   int mat_counter = 0;
   int mat0[MATRIX_TOTAL_SIZE];
   int mat1[MATRIX_TOTAL_SIZE];
   int mat_result[MATRIX_TOTAL_SIZE];

   while(1)
   {
      int read = *spi_addr;
      if((read & 0x00000001) != 0) //check if data in the spi module (status[0])
      {
         read = *(spi_addr+1);
         int operation = (read & 0xff);
         int value = (read >> 8);

         // BUG FIX: assert_read 後に十分なウェイトを入れる。
         // *(spi_addr+2) = 1 の書き込みは CPU の sw 命令から top.v を経由して
         // spi_mm の status_register[0] がクリアされるまで数 FPGAクロックかかる。
         // さらに spi_slave の rd_data_available がクリアされるのは
         // buffer_rd_ack==1 かつ counter_read==0 の条件が揃ったときのみ。
         // ウェイトなしで即座にループに戻ると status[0] がまだ 1 のままで
         // 同じデータを二重処理してしまう。
         *(spi_addr+2) = 1; //assert read (status_register[0] をクリア)
         // rd_ack が spi_slave に届き rd_data_available がクリアされるまで待つ。
         // SPI クロック 32 クロック分 = 少なくとも 1 SPIトランザクション時間相当が必要。
         // FPGA 内部クロック >> SPI クロックなので十分な余裕がある。
         for(volatile int w = 0; w < 16; w++);

         if(operation == 0x4) //set led
         {
            set_led(gpio_addr, value);
         }
         if(operation == 0x5) //gradient
         {
            gradient(gpio_addr);
         }
         if(operation == 0x6) //fibonacci
         {
            int ret = fib(value);
            // BUG FIX: 書き込みバッファが空いていることを確認してから書く。
            // status[1] が 0 (wr_buffer_free) になるまで待つ。
            while((*spi_addr & 0x2) != 0){} //write reg free?
            *(spi_addr+3) = ret;
         }

         if(operation == 0x7) //pow
         {
            while((*spi_addr & 0x2) != 0){} //write reg free?
            *(spi_addr+3) = value * value;
         }

         if(operation == 0x8) //matrix mult
         {
            //read first matrix
            if(mat_counter < MATRIX_TOTAL_SIZE)
            {
               mat0[mat_counter] = value;
               mat_counter++;
            }
            //read second matrix
            else if(mat_counter >= MATRIX_TOTAL_SIZE && mat_counter < (MATRIX_TOTAL_SIZE*2))
            {
               mat1[mat_counter-MATRIX_TOTAL_SIZE] = value;
               mat_counter++;
            }
            //will send result after last receive
            if(mat_counter >= (MATRIX_TOTAL_SIZE*2))
            {
               matrix_mult(mat0, mat1, mat_result);
               for (size_t i = 0; i < MATRIX_TOTAL_SIZE; i++) {
                  while((*spi_addr & 0x2) != 0){} //read status, write reg free?
                  *(spi_addr+3) = mat_result[i]; //write result
               }
               mat_counter = 0; //reset for next multiplication
            }
         }
      }
   }
}
