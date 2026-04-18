yosys_cmd = yosys
pcf_file = ../../common/io.pcf

all: spi timer_debug selftest

spi:
	$(yosys_cmd) -p "synth_ice40 -top top -json top_spi.json -blif top_spi.blif" top_spi.v
	nextpnr-ice40 --up5k --json top_spi.json --pcf $(pcf_file) --asc top_spi.asc
	icepack top_spi.asc top_spi.bin

prog_spi:
	iceprog -S top_spi.bin

timer_debug:
	$(yosys_cmd) -p "synth_ice40 -top top -json top_timer_debug.json -blif top_timer_debug.blif" top_timer_debug.v
	nextpnr-ice40 --up5k --json top_timer_debug.json --pcf $(pcf_file) --asc top_timer_debug.asc
	icepack top_timer_debug.asc top_timer_debug.bin

prog_timer_debug:
	iceprog -S top_timer_debug.bin

selftest:
	$(yosys_cmd) -p "synth_ice40 -top top -json top_selftest.json" top_selftest.v
	nextpnr-ice40 --up5k --json top_selftest.json --pcf $(pcf_file) --asc top_selftest.asc
	icepack top_selftest.asc top_selftest.bin

prog_selftest:
	iceprog -S top_selftest.bin

clean:
	rm -rf top_spi.json top_spi.blif top_spi.asc top_spi.bin \
	       top_timer_debug.json top_timer_debug.blif top_timer_debug.asc top_timer_debug.bin \
	       top_selftest.json top_selftest.asc top_selftest.bin
