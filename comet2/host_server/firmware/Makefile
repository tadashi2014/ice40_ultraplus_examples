PYTHON ?= python3

ASM = ../asm_tools/casl2_asm/casl2.py
OBJ2BIN = ../asm_tools/obj2bin.py

ALL_BINS = \
	hanoi.bin \
	uart_echo.bin \
	uart_tx_test.bin \
	uart_tx_mmio_test.bin \
	uart_rx_mmio_echo.bin

all: $(ALL_BINS)

init.obj: init.cas
	$(PYTHON) $(ASM) -x init.cas

inout_uart.obj: inout_uart.cas
	$(PYTHON) $(ASM) -x inout_uart.cas

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

hanoi.bin: init.obj hanoi.obj inout_uart.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime init.obj hanoi.obj inout_uart.obj -o hanoi.bin

uart_echo.bin: init.obj uart_echo.obj inout_uart.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime init.obj uart_echo.obj inout_uart.obj -o uart_echo.bin

uart_tx_test.bin: init.obj uart_tx_test.obj inout_uart.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime init.obj uart_tx_test.obj inout_uart.obj -o uart_tx_test.bin

uart_tx_mmio_test.bin: init.obj uart_tx_mmio_test.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime init.obj uart_tx_mmio_test.obj -o uart_tx_mmio_test.bin

uart_rx_mmio_echo.bin: init.obj uart_rx_mmio_echo.obj
	$(PYTHON) $(OBJ2BIN) --no-runtime init.obj uart_rx_mmio_echo.obj -o uart_rx_mmio_echo.bin

clean:
	rm -f *.obj *.exe *.bin *.words.txt
