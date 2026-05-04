#ifndef SPI_LIB_H
#define SPI_LIB_H
#if defined(__has_include) && __has_include(<libftdi1/ftdi.h>)
#  include <libftdi1/ftdi.h>
#else
#  include <ftdi.h>
#endif

enum mpsse_cmd
{
  MC_SETB_LOW = 0x80,
  MC_READB_LOW = 0x81,
  MC_SETB_HIGH = 0x82,
  MC_READB_HIGH = 0x83,
  MC_LOOPBACK_EN = 0x84,
  MC_LOOPBACK_DIS = 0x85,
  MC_SET_CLK_DIV = 0x86,
  MC_FLUSH = 0x87,
  MC_WAIT_H = 0x88,
  MC_WAIT_L = 0x89,
  MC_TCK_X5 = 0x8A,
  MC_TCK_D5 = 0x8B,
  MC_EN_3PH_CLK = 0x8C,
  MC_DIS_3PH_CLK = 0x8D,
  MC_CLK_N = 0x8E,
  MC_CLK_N8 = 0x8F,
  MC_CLK_TO_H = 0x94,
  MC_CLK_TO_L = 0x95,
  MC_EN_ADPT_CLK = 0x96,
  MC_DIS_ADPT_CLK = 0x97,
  MC_CLK8_TO_H = 0x9C,
  MC_CLK8_TO_L = 0x9D,
  MC_TRI = 0x9E,
  MC_CPU_RS = 0x90,
  MC_CPU_RE = 0x91,
  MC_CPU_WS = 0x92,
  MC_CPU_WE = 0x93,
};

#define MC_DATA_TMS  (0x40)
#define MC_DATA_IN   (0x20)
#define MC_DATA_OUT  (0x10)
#define MC_DATA_LSB  (0x08)
#define MC_DATA_ICN  (0x04)
#define MC_DATA_BITS (0x02)
#define MC_DATA_OCN  (0x01)

#define STATUS_FPGA_RECV_OFFSET 6
#define STATUS_FPGA_SEND_OFFSET 7

#define STATUS_FPGA_RECV_MASK (0x1<<STATUS_FPGA_RECV_OFFSET)
#define STATUS_FPGA_SEND_MASK (0x1<<STATUS_FPGA_SEND_OFFSET)

int spi_init();
int spi_send(uint8_t cmd, uint8_t val[3], uint8_t *status);
int spi_send3(uint8_t cmd, uint8_t val0, uint8_t val1, uint8_t val2, uint8_t *status);
int spi_send24b(uint8_t cmd, uint32_t val24b, uint8_t *status);
int spi_send_batch_packets(const uint8_t *packets, size_t packet_count, uint8_t *last_status);
int spi_read(uint8_t val[3], uint8_t *status);

#endif
