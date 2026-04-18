pcf_file = ../../common/io.pcf
CPU ?= picorv32

ifeq ($(CPU),picorv32)
cpu_define = -DCPU_PICORV32
cpu_extra_srcs =
else ifeq ($(CPU),simple)
cpu_define = -DCPU_SIMPLE
cpu_extra_srcs = ../simple_riscv_cpu/simple_cpu/alu/alu.v ../simple_riscv_cpu/simple_cpu/register_file/register_file.v
else
$(error CPU must be "picorv32" or "simple")
endif

ifeq ($(CPU),simple)
selected_uart_pcf = ../top_uart.pcf
else
selected_uart_pcf = $(pcf_file)
endif

yosys_cpu = yosys $(cpu_define)

all: blink timer_debug spi_debug selftest

uart_tx_test:
	yosys -p "synth_ice40 -top top -json top_uart_tx_test.json" top_uart_tx_test.v
	nextpnr-ice40 --up5k --json top_uart_tx_test.json --pcf ../top_uart.pcf --asc top_uart_tx_test.asc
	icepack top_uart_tx_test.asc top_uart_tx_test.bin

prog_uart_tx_test:
	iceprog -S top_uart_tx_test.bin

blink:
	yosys -p "synth_ice40 -top top -json top_blink.json" top_blink.v
	nextpnr-ice40 --up5k --json top_blink.json --pcf $(pcf_file) --asc top_blink.asc
	icepack top_blink.asc top_blink.bin

prog_blink:
	iceprog -S top_blink.bin

timer_debug:
	$(yosys_cpu) -p "synth_ice40 -top top -json top_timer_debug.json -blif top_timer_debug.blif" top_timer_debug.v $(cpu_extra_srcs)
	nextpnr-ice40 --up5k --json top_timer_debug.json --pcf $(pcf_file) --asc top_timer_debug.asc
	icepack top_timer_debug.asc top_timer_debug.bin

prog_timer_debug:
	iceprog -S top_timer_debug.bin

spi_debug:
	$(yosys_cpu) -p "synth_ice40 -top top -json top_spi_debug.json -blif top_spi_debug.blif" top_spi_debug.v $(cpu_extra_srcs)
	nextpnr-ice40 --up5k --json top_spi_debug.json --pcf $(selected_uart_pcf) --asc top_spi_debug.asc
	icepack top_spi_debug.asc top_spi_debug.bin

prog_spi_debug:
	iceprog -S top_spi_debug.bin

selftest:
	$(yosys_cpu) -p "synth_ice40 -top top -json top_selftest.json" top_selftest.v $(cpu_extra_srcs)
	nextpnr-ice40 --up5k --json top_selftest.json --pcf $(pcf_file) --asc top_selftest.asc
	icepack top_selftest.asc top_selftest.bin

prog_selftest:
	iceprog -S top_selftest.bin

clean:
	rm -rf top_blink.json top_blink.asc top_blink.bin \
	       top_timer_debug.json top_timer_debug.blif top_timer_debug.asc top_timer_debug.bin \
	       top_spi_debug.json top_spi_debug.blif top_spi_debug.asc top_spi_debug.bin \
	       top_selftest.json top_selftest.asc top_selftest.bin \
	       top_uart_tx_test.json top_uart_tx_test.asc top_uart_tx_test.bin
