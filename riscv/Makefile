filename = top
pcf_file = ../common/io.pcf

# ---- Production build (requires SPI firmware load to start CPU) ----
build:
	yosys -p "synth_ice40 -top top -json $(filename).json -blif $(filename).blif" $(filename).v simple_cpu/alu.v simple_cpu/register_file.v
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
	yosys -p "synth_ice40 -top top -json top_timer_debug.json -blif top_timer_debug.blif" top_timer_debug.v simple_cpu/alu.v simple_cpu/register_file.v
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

# ---- SPI debug build: SPI required to start CPU; LED_B=waiting, LED_G=error, LED_R=firmware ----
# Step 4 in the debug progression.  Load firmware with host_server/./host.
# LED behaviour (active-low: 0=ON, 1=OFF):
#   LED_B ON  : cpu_reset=1 (SPI START_CPU not yet received)
#   LED_B OFF + LED_R ON  : SPI firmware loaded and CPU running  ✓
#   LED_B OFF + LED_G ON  : CPU started but cpu_error_instruction fired (bad firmware) ✗
#   LED_B stays ON        : host never sent SPI_START_CPU (SPI path problem) ✗
spi_debug:
	yosys -p "synth_ice40 -top top -json top_spi_debug.json -blif top_spi_debug.blif" top_spi_debug.v simple_cpu/alu.v simple_cpu/register_file.v
	nextpnr-ice40 --up5k --json top_spi_debug.json --pcf $(pcf_file) --asc top_spi_debug.asc
	icepack top_spi_debug.asc top_spi_debug.bin

prog_spi_debug:
	iceprog -S top_spi_debug.bin

# ---- Self-test build: 5-instruction ROM embedded in bitstream, no SPI needed ----
# Firmware (embedded ROM):
#   lui  a0, 8       ; a0 = 0x8000
#   addi a0, a0, 256 ; a0 = 0x8100  (GPIO address)
#   addi a1, x0, 1   ; a1 = 1
#   sw   a1, 0(a0)   ; GPIO[0]=1  -> LED_R ON
#   jal  x0, 0       ; infinite loop
#
# Expected result within a few microseconds of programming:
#   LED_R ON : CPU fetch/decode/execute and GPIO write all work.
#              The problem is ONLY in the SPI firmware-load path.
#   LED_G ON : cpu_error_instruction fired (ROM encoding bug, should not happen).
#   LED_R OFF + LED_G OFF : CPU is not starting (check power-on reset logic).
selftest:
	yosys -p "synth_ice40 -top top -json top_selftest.json" top_selftest.v simple_cpu/alu.v simple_cpu/register_file.v
	nextpnr-ice40 --up5k --json top_selftest.json --pcf $(pcf_file) --asc top_selftest.asc
	icepack top_selftest.asc top_selftest.bin

prog_selftest:
	iceprog -S top_selftest.bin

clean:
	rm -rf $(filename).json $(filename).blif $(filename).asc $(filename).bin \
	       top_timer_debug.json top_timer_debug.blif top_timer_debug.asc top_timer_debug.bin \
	       top_blink.json top_blink.asc top_blink.bin \
	       top_selftest.json top_selftest.blif top_selftest.asc top_selftest.bin \
	       top_spi_debug.json top_spi_debug.blif top_spi_debug.asc top_spi_debug.bin
