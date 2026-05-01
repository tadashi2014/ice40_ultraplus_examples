PYTHON ?= python3

ASM = ../asm_tools/casl2_asm/casl2.py
OBJ2BIN = ../asm_tools/obj2bin.py
IO_VARIANT ?= uart

ifeq ($(IO_VARIANT),spi)
VECTOR_SRC := vector_spi.cas
VECTOR_OBJ := vector_spi.obj
RUNTIME_SRC := svc_handler_spi.cas
RUNTIME_OBJ := svc_handler_spi.obj
else ifeq ($(IO_VARIANT),uart)
VECTOR_SRC := vector_uart.cas
VECTOR_OBJ := vector_uart.obj
RUNTIME_SRC := svc_handler_uart.cas
RUNTIME_OBJ := svc_handler_uart.obj
else
$(error Unsupported IO_VARIANT '$(IO_VARIANT)'. Use 'uart' or 'spi')
endif

ALL_BINS = \
	mmio_gpio_test.bin \
	mmio_uart_tx_test.bin \
	mmio_uart_rxtx_test.bin \
	mmio_timer_test.bin \
	tx_test.bin \
	echo.bin \
	hanoi.bin


all: $(ALL_BINS)

$(VECTOR_OBJ): $(VECTOR_SRC)
	$(PYTHON) $(ASM) -x $(VECTOR_SRC)

$(RUNTIME_OBJ): $(RUNTIME_SRC)
	$(PYTHON) $(ASM) -x $(RUNTIME_SRC)

mmio_gpio_test.obj: mmio_gpio_test.cas
	$(PYTHON) $(ASM) -x mmio_gpio_test.cas

mmio_uart_tx_test.obj: mmio_uart_tx_test.cas
	$(PYTHON) $(ASM) -x mmio_uart_tx_test.cas

mmio_uart_rxtx_test.obj: mmio_uart_rxtx_test.cas
	$(PYTHON) $(ASM) -x mmio_uart_rxtx_test.cas

mmio_timer_test.obj: mmio_timer_test.cas
	$(PYTHON) $(ASM) -x mmio_timer_test.cas

tx_test.obj: tx_test.cas
	$(PYTHON) $(ASM) -x tx_test.cas

echo.obj: echo.cas
	$(PYTHON) $(ASM) -x echo.cas

hanoi.obj: hanoi.cas
	$(PYTHON) $(ASM) -x hanoi.cas


mmio_gpio_test.bin: $(VECTOR_OBJ) mmio_gpio_test.obj $(RUNTIME_OBJ)
	$(PYTHON) $(OBJ2BIN) --no-runtime $(VECTOR_OBJ) mmio_gpio_test.obj $(RUNTIME_OBJ) -o mmio_gpio_test.bin

mmio_uart_tx_test.bin: $(VECTOR_OBJ) mmio_uart_tx_test.obj $(RUNTIME_OBJ)
	$(PYTHON) $(OBJ2BIN) --no-runtime $(VECTOR_OBJ) mmio_uart_tx_test.obj $(RUNTIME_OBJ) -o mmio_uart_tx_test.bin

mmio_uart_rxtx_test.bin: $(VECTOR_OBJ) mmio_uart_rxtx_test.obj $(RUNTIME_OBJ)
	$(PYTHON) $(OBJ2BIN) --no-runtime $(VECTOR_OBJ) mmio_uart_rxtx_test.obj $(RUNTIME_OBJ) -o mmio_uart_rxtx_test.bin

mmio_timer_test.bin: $(VECTOR_OBJ) mmio_timer_test.obj $(RUNTIME_OBJ)
	$(PYTHON) $(OBJ2BIN) --no-runtime $(VECTOR_OBJ) mmio_timer_test.obj $(RUNTIME_OBJ) -o mmio_timer_test.bin

tx_test.bin: $(VECTOR_OBJ) tx_test.obj $(RUNTIME_OBJ)
	$(PYTHON) $(OBJ2BIN) --no-runtime $(VECTOR_OBJ) tx_test.obj $(RUNTIME_OBJ) -o tx_test.bin

echo.bin: $(VECTOR_OBJ) echo.obj $(RUNTIME_OBJ)
	$(PYTHON) $(OBJ2BIN) --no-runtime $(VECTOR_OBJ) echo.obj $(RUNTIME_OBJ) -o echo.bin

hanoi.bin: $(VECTOR_OBJ) hanoi.obj $(RUNTIME_OBJ)
	$(PYTHON) $(OBJ2BIN) --no-runtime $(VECTOR_OBJ) hanoi.obj $(RUNTIME_OBJ) -o hanoi.bin

clean:
	rm -f *.obj *.exe *.bin *.words.txt
