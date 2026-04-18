filename = top
yosys_cmd = yosys
uart_pcf_file = top_uart.pcf
spi_pcf_file = ../common/io.pcf

build:
	$(yosys_cmd) -p "synth_ice40 -top top -json $(filename).json -blif $(filename).blif" $(filename).v
	nextpnr-ice40 --up5k --json $(filename).json --pcf $(uart_pcf_file) --asc $(filename).asc
	icepack $(filename).asc $(filename).bin

prog:
	iceprog -S $(filename).bin

prog_flash:
	iceprog $(filename).bin

spi:
	$(MAKE) -C tests spi

prog_spi:
	$(MAKE) -C tests prog_spi

debug:
	$(MAKE) -C tests timer_debug

prog_debug:
	$(MAKE) -C tests prog_timer_debug

spi_debug:
	@$(MAKE) build

prog_spi_debug:
	@$(MAKE) prog

selftest:
	$(MAKE) -C tests selftest

prog_selftest:
	$(MAKE) -C tests prog_selftest

clean:
	rm -rf $(filename).json $(filename).blif $(filename).asc $(filename).bin
	$(MAKE) -C tests clean
