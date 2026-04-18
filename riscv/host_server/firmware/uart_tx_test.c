#define UART_BASE 0x8200

static inline int uart_status(void)
{
   return *(volatile int *)(UART_BASE + 0x0);
}

static inline void uart_write_byte(int value)
{
   *(volatile int *)(UART_BASE + 0x8) = value & 0xff;
}

static void uart_write_string(const char *s)
{
   while (*s != 0) {
      while ((uart_status() & 0x2) != 0) {
      }
      uart_write_byte(*s++);
   }
}

void main()
{
   while (1) {
      uart_write_string("HELLO\r\n");
      for (volatile int i = 0; i < 800000; ++i) {
      }
   }
}
