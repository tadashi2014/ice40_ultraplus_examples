filename = top
pcf_file = ../common/io.pcf

# ---- Production build (requires SPI firmware load to start CPU) ----
build:
	yosys -p "synth_ice40 -top top -json $(filename).json -blif $(filename).blif" $(filename).v alu.v register_file.v
	nextpnr-ice40 --up5k --json $(filename).json --pcf $(pcf_file) --asc $(filename).asc
	icepack $(filename).asc $(filename).bin

prog:
	iceprog -S $(filename).bin

prog_flash:
	iceprog $(filename).bin

# ---- Debug build: auto-releases cpu_reset after ~2s, no SPI needed ----
# LED behaviour (active-low: 0=ON, 1=OFF):
#   LED_B ON  for the first ~2 s while cpu_reset=1
#   LED_B OFF after reset release; CPU starts executing
#   LED_G ON  if cpu_error_instruction fires (bad/unrecognised opcode)
#   LED_R     controlled by firmware via gpio_mm (write 1 to 0x8100)
# With empty SPRAM (all zeros = illegal opcode), expect:
#   0-2 s : Blue ON
#   2 s+  : Blue OFF, Green ON (error from 0x00000000 instruction)
debug:
	yosys -p "synth_ice40 -top top -json top_timer_debug.json -blif top_timer_debug.blif" top_timer_debug.v alu.v register_file.v
	nextpnr-ice40 --up5k --json top_timer_debug.json --pcf $(pcf_file) --asc top_timer_debug.asc
	icepack top_timer_debug.asc top_timer_debug.bin

prog_debug:
	iceprog -S top_timer_debug.bin

# ---- Blink test: no CPU, no SPI, no firmware needed ----
# Use this first to confirm the FPGA is being programmed correctly.
# All 3 LEDs blink independently at different rates (~0.35/0.70/1.40 s).
# If LEDs blink -> FPGA programming OK, problem is in CPU/firmware/SPI.
# If LEDs do NOT blink -> check iceprog connection / FPGA power.
blink:
	yosys -p "synth_ice40 -top top -json top_blink.json" top_blink.v
	nextpnr-ice40 --up5k --json top_blink.json --pcf $(pcf_file) --asc top_blink.asc
	icepack top_blink.asc top_blink.bin

prog_blink:
	iceprog -S top_blink.bin

clean:
	rm -rf $(filename).json $(filename).blif $(filename).asc $(filename).bin \
	       top_timer_debug.json top_timer_debug.blif top_timer_debug.asc top_timer_debug.bin \
	       top_blink.json top_blink.asc top_blink.bin
