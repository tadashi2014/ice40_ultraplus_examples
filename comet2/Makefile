filename = top
pcf_file = ../common/io.pcf
yosys_cmd = yosys

build:
	$(yosys_cmd) -p "synth_ice40 -top top -json $(filename).json -blif $(filename).blif" $(filename).v
	nextpnr-ice40 --up5k --json $(filename).json --pcf $(pcf_file) --asc $(filename).asc
	icepack $(filename).asc $(filename).bin

prog:
	iceprog -S $(filename).bin

prog_flash:
	iceprog $(filename).bin

debug:
	$(yosys_cmd) -p "synth_ice40 -top top -json top_timer_debug.json -blif top_timer_debug.blif" top_timer_debug.v
	nextpnr-ice40 --up5k --json top_timer_debug.json --pcf $(pcf_file) --asc top_timer_debug.asc
	icepack top_timer_debug.asc top_timer_debug.bin

prog_debug:
	iceprog -S top_timer_debug.bin

spi_debug:
	$(yosys_cmd) -p "synth_ice40 -top top -json top_spi_debug.json -blif top_spi_debug.blif" top_spi_debug.v
	nextpnr-ice40 --up5k --json top_spi_debug.json --pcf $(pcf_file) --asc top_spi_debug.asc
	icepack top_spi_debug.asc top_spi_debug.bin

prog_spi_debug:
	iceprog -S top_spi_debug.bin

selftest:
	$(yosys_cmd) -p "synth_ice40 -top top -json top_selftest.json" top_selftest.v
	nextpnr-ice40 --up5k --json top_selftest.json --pcf $(pcf_file) --asc top_selftest.asc
	icepack top_selftest.asc top_selftest.bin

prog_selftest:
	iceprog -S top_selftest.bin

clean:
	rm -rf $(filename).json $(filename).blif $(filename).asc $(filename).bin \
	       top_timer_debug.json top_timer_debug.blif top_timer_debug.asc top_timer_debug.bin \
	       top_selftest.json top_selftest.blif top_selftest.asc top_selftest.bin \
	       top_spi_debug.json top_spi_debug.blif top_spi_debug.asc top_spi_debug.bin
