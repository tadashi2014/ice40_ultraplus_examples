filename = top
pcf_file = ../common/io.pcf
CPU ?= picorv32

ifeq ($(CPU),picorv32)
cpu_define = -DCPU_PICORV32
cpu_extra_srcs =
else ifeq ($(CPU),simple)
cpu_define = -DCPU_SIMPLE
cpu_extra_srcs = simple_riscv_cpu/simple_cpu/alu/alu.v simple_riscv_cpu/simple_cpu/register_file/register_file.v
else
$(error CPU must be "picorv32" or "simple")
endif

ifeq ($(CPU),simple)
selected_pcf = top_uart.pcf
else
selected_pcf = $(pcf_file)
endif

yosys_cpu = yosys $(cpu_define)

# ---- Production build (requires SPI firmware load to start CPU) ----
# Select CPU with CPU=picorv32 (default) or CPU=simple.
# Example: make CPU=simple build
build:
	$(yosys_cpu) -p "synth_ice40 -top top -json $(filename).json -blif $(filename).blif" $(filename).v $(cpu_extra_srcs)
	nextpnr-ice40 --up5k --json $(filename).json --pcf $(selected_pcf) --asc $(filename).asc
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
	$(MAKE) -C tests timer_debug CPU=$(CPU)

prog_debug:
	$(MAKE) -C tests prog_timer_debug

# ---- Blink test: no CPU, no SPI, no firmware needed ----
# Use this first to confirm the FPGA is being programmed correctly.
# All 3 LEDs blink independently at different rates (~0.35/0.70/1.40 s).
# If LEDs blink -> FPGA programming OK, problem is in CPU/firmware/SPI.
# If LEDs do NOT blink -> check iceprog connection / FPGA power.
blink:
	$(MAKE) -C tests blink

prog_blink:
	$(MAKE) -C tests prog_blink

# ---- SPI debug build: SPI required to start CPU; LED_B=waiting, LED_G=error, LED_R=firmware ----
# Step 4 in the debug progression.  Load firmware with host_server/./host.
# LED behaviour (active-low: 0=ON, 1=OFF):
#   LED_B ON  : cpu_reset=1 (SPI START_CPU not yet received)
#   LED_B OFF + LED_R ON  : SPI firmware loaded and CPU running  ✓
#   LED_B OFF + LED_G ON  : CPU started but selected CPU reported an error/trap ✗
#   LED_B stays ON        : host never sent SPI_START_CPU (SPI path problem) ✗
spi_debug:
	$(MAKE) -C tests spi_debug CPU=$(CPU)

prog_spi_debug:
	$(MAKE) -C tests prog_spi_debug

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
#   LED_G ON : selected CPU reported an error/trap (ROM encoding bug, should not happen).
#   LED_R OFF + LED_G OFF : CPU is not starting (check power-on reset logic).
selftest:
	$(MAKE) -C tests selftest CPU=$(CPU)

prog_selftest:
	$(MAKE) -C tests prog_selftest

clean:
	rm -rf $(filename).json $(filename).blif $(filename).asc $(filename).bin
	$(MAKE) -C tests clean
