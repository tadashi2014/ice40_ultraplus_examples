filename = top
pcf_file = ../common/io.pcf
CPU ?= picorv32

ifeq ($(CPU),picorv32)
cpu_define = -DCPU_PICORV32
cpu_extra_srcs =
else ifeq ($(CPU),simple)
cpu_define = -DCPU_SIMPLE
cpu_extra_srcs = simple_riscv_cpu/simple_cpu/alu/alu.v simple_riscv_cpu/simple_cpu/register_file/register_file.v
else ifeq ($(CPU),comet2)
cpu_define = -DCPU_COMET2
cpu_extra_srcs =
filename = top_comet
else
$(error CPU must be "picorv32", "simple", or "comet2")
endif

yosys_cpu = yosys $(cpu_define)

# ---- Production build (requires SPI firmware load to start CPU) ----
# Select CPU with CPU=picorv32 (default), CPU=simple, or CPU=comet2.
# Example: make CPU=comet2 build
build:
	$(yosys_cpu) -p "synth_ice40 -top top -json $(filename).json -blif $(filename).blif" $(filename).v $(cpu_extra_srcs)
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
#   LED_G ON  if the selected CPU reports an error/trap
#   LED_R     controlled by firmware via gpio_mm (write 1 to 0x8100)
# With empty SPRAM, behaviour depends on the selected CPU and trap settings.
# Use selftest for CPU bring-up.
#   0-2 s : Blue ON
#   2 s+  : Blue OFF; firmware/debug behaviour depends on loaded memory
debug:
	$(yosys_cpu) -p "synth_ice40 -top top -json top_timer_debug.json -blif top_timer_debug.blif" top_timer_debug.v $(cpu_extra_srcs)
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
#   LED_B OFF + LED_G ON  : CPU started but selected CPU reported an error/trap ✗
#   LED_B stays ON        : host never sent SPI_START_CPU (SPI path problem) ✗
spi_debug:
	$(yosys_cpu) -p "synth_ice40 -top top -json top_spi_debug.json -blif top_spi_debug.blif" top_spi_debug.v $(cpu_extra_srcs)
	nextpnr-ice40 --up5k --json top_spi_debug.json --pcf $(pcf_file) --asc top_spi_debug.asc
	icepack top_spi_debug.asc top_spi_debug.bin

prog_spi_debug:
	iceprog -S top_spi_debug.bin

# ---- Self-test build: ROM embedded in bitstream, no SPI needed ----
# Firmware for CPU=picorv32/simple (embedded ROM):
#   lui  a0, 8       ; a0 = 0x8000
#   addi a0, a0, 256 ; a0 = 0x8100  (GPIO address)
#   addi a1, x0, 1   ; a1 = 1
#   sw   a1, 0(a0)   ; GPIO[0]=1  -> LED_R ON
#   jal  x0, 0       ; infinite loop
# Firmware for CPU=comet2 is in comet2_selftest_rom.v:
#   mem[0]  = reset vector
#   mem[19] = initial SP
#   GPIO word address 0xC100 drives the low 3 LED bits
#   R/G/B blink sequence, software wait loop, then SVC 0 reloads SP and jumps via vector 0
#
# Expected result within a few microseconds of programming:
#   LED_R/G/B activity : CPU fetch/decode/execute and GPIO write all work.
#              The problem is ONLY in the SPI firmware-load path.
#   LED_G ON : selected CPU reported an error/trap (ROM encoding bug, should not happen).
#   LED_R OFF + LED_G OFF : CPU is not starting (check power-on reset logic).
selftest:
	$(yosys_cpu) -p "synth_ice40 -top top -json top_selftest.json" top_selftest.v $(cpu_extra_srcs)
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
