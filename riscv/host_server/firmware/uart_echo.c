#define UART_BASE 0x8200

static inline int uart_status(void)
{
   return *(volatile int *)(UART_BASE + 0x0);
}

static inline int uart_read_byte(void)
{
   return *(volatile int *)(UART_BASE + 0x4) & 0xff;
}

static inline void uart_write_byte(int value)
{
   *(volatile int *)(UART_BASE + 0x8) = value & 0xff;
}

void main()
{
   while (1) {
      if ((uart_status() & 0x1) != 0) {
         int ch = uart_read_byte();
         while ((uart_status() & 0x2) != 0) {
         }
         uart_write_byte(ch);
      }
   }
}
