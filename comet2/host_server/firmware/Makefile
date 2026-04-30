PYTHON ?= python3

ASM = ../asm_tools/casl2_asm/casl2.py
OBJ2BIN = ../asm_tools/obj2bin.py

ALL_BINS = \
	led_red.bin \
	uart_echo.bin \
	uart_tx_test.bin \
	uart_tx_mmio_test.bin \
	uart_rx_mmio_echo.bin \
	timer_mmio_uart_test.bin \
	hanoi.bin

all: $(ALL_BINS)

vector_uart.obj: vector_uart.cas
	$(PYTHON) $(ASM) -x vector_uart.cas

vector_spi.obj: vector_spi.cas
	$(PYTHON) $(ASM) -x vector_spi.cas

runtime_mmio.obj: runtime_mmio.cas
	$(PYTHON) $(ASM) -x runtime_mmio.cas

hanoi.obj: hanoi.cas
	$(PYTHON) $(ASM) -x hanoi.cas

uart_echo.obj: uart_echo.cas
	$(PYTHON) $(ASM) -x uart_echo.cas

uart_tx_test.obj: uart_tx_test.cas
	$(PYTHON) $(ASM) -x uart_tx_test.cas

uart_tx_mmio_test.obj: uart_tx_mmio_test.cas
	$(PYTHON) $(ASM) -x uart_tx_mmio_test.cas

uart_rx_mmio_echo.obj: uart_rx_mmio_echo.cas
	$(PYTHON) $(ASM) -x uart_rx_mmio_echo.cas

timer_mmio_uart_test.obj: timer_mmio_uart_test.cas
	$(PYTHON) $(ASM) -x timer_mmio_uart_test.cas

led_red.obj: led_red.cas
	$(PYTHON) $(ASM) -x led_red.cas

hanoi.bin: vector_uart.obj hanoi.obj runtime_mmio.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime vector_uart.obj hanoi.obj runtime_mmio.obj -o hanoi.bin

uart_echo.bin: vector_uart.obj uart_echo.obj runtime_mmio.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime vector_uart.obj uart_echo.obj runtime_mmio.obj -o uart_echo.bin

uart_tx_test.bin: vector_uart.obj uart_tx_test.obj runtime_mmio.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime vector_uart.obj uart_tx_test.obj runtime_mmio.obj -o uart_tx_test.bin

uart_tx_mmio_test.bin: vector_uart.obj uart_tx_mmio_test.obj runtime_mmio.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime vector_uart.obj uart_tx_mmio_test.obj  runtime_mmio.obj -o uart_tx_mmio_test.bin

uart_rx_mmio_echo.bin: vector_uart.obj uart_rx_mmio_echo.obj runtime_mmio.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime vector_uart.obj uart_rx_mmio_echo.obj  runtime_mmio.obj -o uart_rx_mmio_echo.bin

timer_mmio_uart_test.bin: vector_uart.obj timer_mmio_uart_test.obj runtime_mmio.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime vector_uart.obj timer_mmio_uart_test.obj  runtime_mmio.obj -o timer_mmio_uart_test.bin

led_red.bin: vector_uart.obj led_red.obj runtime_mmio.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime vector_uart.obj led_red.obj  runtime_mmio.obj -o led_red.bin

clean:
	rm -f *.obj *.exe *.bin *.words.txt
