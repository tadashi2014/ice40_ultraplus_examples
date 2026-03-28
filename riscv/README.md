# RISC-V implementation on iCE40

__**Note: this is a work in progress, it compiles and works but needs some finalizations**__

This is an implementation of the simplest RISC-V CPU (rv32i) on the ice40-ultraplus fpga.

On the FPGA are implemented, in addition to the CPU and memory, a GPIO module to drive a RGB led and a SPI module to communicate with a host computer (on linux for example)

The CPU can communicate with a computer using a bidirectional spi communication. The host computer can send the following commands which will be executed on the fpga's CPU:
- Light up the RGB LED with a given colour
- Calculate a Fibonacci number, to test the stack
- Calculate a power of two, to test the gcclib soft mathematical functions (mult not available in r32i)
- Matrix multplication, to test the maximum speed of spi communication

All these operations would normally be optimized by being implemented in fpga and used with a custom riscv command, but they are here to test the soft capabilities of the RISCV cpu

```
+----------------------------------+                +-------------------------+
| iCE40 UltraPlus FPGA             |                |Computer running Linux   |
| Breakout Board                   |                |                         |
| +--------------+     +---------+ |                |                         |
| |              |     |Memory   | |                |                         |
| | RISC-V CPU   +-----+SPRAM    | |                |                         |
| | R32i         |     |32K      | |                |                         |
| |              |     +---------+ |                | +----------+ +--------+ |
| |              |                 |                | |Host      | |Firmware| |
| |              |     +---------+ |  +-------+     | |Server    | |Risc-V  | |
| |              +-----+SPI      | |  |FTDI   |USB  | |x86       | |To be   | |
| |              |     |Module   +----+Chip   +-------+          | |sent    | |
| |              +--+  |         | |  |       |     | |          | |        | |
| |              |  |  +---------+ |  +-------+     | |          | |        | |
| +--------------+  |              |                | |          | |        | |
|                   |  +---------+ |                | +----------+ +--------+ |
|      +------+     +--+GPIO     | |                |                         |
|      |RGB   |        |Module   | |                |                         |
|      |LED   +--------+         | |                |                         |
|      +------+        +---------+ |                |                         |
|                                  |                |                         |
+----------------------------------+                +-------------------------+
```

## Debugging (LEDが消灯のまま / LED stays off)

LEDs are **active-low** (signal 0 = ON, signal 1 = OFF).  
With the default `top.v`, the CPU only starts after SPI sends a "start" command, so  
LEDs stay off until firmware runs.  Use the step-by-step debug builds below.

### Step 1 — Blink test (FPGA programming check)

Verifies that the FPGA is being programmed and the clock/LED pins work.  
No CPU, no SPI, no firmware required.

```
make blink
make prog_blink
```

**Expected result:** All 3 LEDs blink independently at different rates (~0.35 / 0.70 / 1.40 s).

| Observation | Diagnosis |
|-------------|-----------|
| LEDs blink as expected | FPGA programming OK → go to Step 2 |
| LEDs do NOT blink | Check USB/FTDI connection, `iceprog` version, and board power |

### Step 2 — Timer debug build (CPU execution check)

Automatically releases `cpu_reset` after ~2 s without SPI.  
LED colours show CPU health:

| LED | Meaning |
|-----|---------|
| 🔵 Blue ON (first ~2 s) | FPGA running, CPU in reset |
| 🔵 Blue OFF → 🟢 Green ON | Timer fired, cpu_reset released — FPGA clock/timer/LED all working |
| 🔵 Blue ON, no Green | Timer not firing — check bitstream or clock |

```
make debug
make prog_debug
```

The expected sequence is:

1. **0 – 2 s:** Blue ON  
2. **After 2 s:** Blue OFF, **Green ON** (timer fired → cpu_reset released → Green latches on permanently)

**"Blue OFF → Green ON (stays green)" means the FPGA clock, timer circuit, and LED path all work.**  
SPRAM starts with undefined contents, so the CPU may or may not generate an error —  
Green is driven directly by `timer_fired` for a reliable, deterministic indication.  
→ Proceed to Step 3 to confirm the CPU can run real instructions from ROM.

If Blue never lights → the timer logic or bitstream is not running.

### Step 3 — Self-test (CPU full-pipeline check, no SPI required)

A tiny firmware (5 RISC-V instructions) is hard-coded directly in the bitstream (`rom.v`).  
No SPI, no host computer, no SPRAM needed.  
The bus controller is identical to Step 2 (only the memory backend changes from SPRAM to ROM).

```
make selftest
make prog_selftest
```

**Expected result after programming:**

| LED sequence | Observation | Diagnosis |
|-------------|-------------|-----------|
| Blue ON for 2 s, then 🔴 **Red ON** | ✅ | CPU fetch, decode, execute, and GPIO write all work. Problem is **only** in the SPI firmware-load path. |
| Blue ON for 2 s, then 🟢 **Green ON** | ❌ | `error_instruction` fired — ROM encoding bug (should not happen). |
| Blue ON for 2 s, then **all OFF** | ❌ | CPU is not executing from ROM. |
| Blue never lights | ❌ | Timer or bitstream is not running (check FPGA programming). |

If Red lights after 2 s, the CPU pipeline is fully functional.  
Proceed to debug the SPI / host_server firmware-load path.

### Step 4 — SPI debug build (SPI firmware-load check)

Same hardware as the production build, but LEDs show the SPI and CPU status so you can
isolate exactly where the SPI path fails.

```
make spi_debug
make prog_spi_debug

cd host_server/firmware && make
cd host_server && make && ./host
```

**Expected result:**

| LED sequence | Observation | Diagnosis |
|-------------|-------------|-----------|
| 🔵 Blue ON, stays on | ❌ | host never sent `SPI_START_CPU`. Check USB/FTDI connection and `./host` output. |
| 🔵 Blue OFF → 🔴 **Red ON** | ✅ | Full SPI path works. SPI firmware load + CPU execute + GPIO write all work. Proceed to Step 5. |
| 🔵 Blue OFF → 🟢 **Green ON** | ❌ | `cpu_error_instruction` fired — firmware compiled or loaded incorrectly. Check `firmware/main` binary and SPI bit-error rate. |
| 🔵 Blue OFF, all LEDs OFF | ❌ | CPU started but firmware not writing to GPIO. Check firmware build. |

LED meaning (active-low: signal 0 = ON, signal 1 = OFF):

| LED | Meaning |
|-----|---------|
| 🔵 Blue ON | `cpu_reset=1` — SPI_START_CPU not yet received |
| 🔵 Blue OFF | CPU has been released from reset |
| 🔴 Red ON | Firmware wrote 1 to GPIO bit0 (gpio_mm address 0x8100) |
| 🟢 Green ON | `cpu_error_instruction` latched — bad opcode |

### Step 5 — Normal production build (SPI firmware load required)

Load firmware via the host server, then the CPU starts:

```
make build
make prog

cd host_server/firmware && make
cd host_server && make && ./host
```

---

## How to build

There are the following subsystems to compile:
- The FPGA bitstream itself (with the riscv cpu) using the icestorm tools
- The host program (`host server`) to be compiled with the gcc of the computer
- The firmware (`host_server/firmware`) to be compiled with the riscv toolchain

Build and run all the systems:
```
make # compiles the fpga
make prog # programs the fpga

# riscv firmware
cd host_server/firmware
make

# host program
cd host_server
make

# sends the firmware and run the example program (light leds, calculate a fibonacci and pow2 on the riscv chip)
cd host_server
./host
```

Build the gnu toolchain https://github.com/riscv/riscv-gnu-toolchain (works with commit afcc8bc655d30c, gcc 8.3.0) with
gcc and newlib with the following parameters:

```
./configure --prefix=/opt/riscv --with-arch=rv32i --with-abi=ilp32
make
```

### macOS (Homebrew) prerequisites

Install the required libraries for the host build:

```
brew install libftdi libusb pkg-config
```

The Homebrew RISC-V toolchain uses the `riscv64-unknown-elf-` prefix (despite targeting rv32i
via compiler flags).  The firmware `Makefile` defaults to this prefix automatically, so no
extra configuration is needed when using the Homebrew toolchain:

```
brew install riscv-gnu-toolchain
# builds rv32i firmware with the riscv64-unknown-elf- prefix
cd host_server/firmware
make
```

To use a custom prefix (e.g. the `/opt/riscv` toolchain built above with the `riscv32-` prefix):

```
make RISCV_PREFIX=riscv32-unknown-elf-
```

## System

The system has a rv32i riscv soft cpu, a 32KB memory, a gpio module to drive a rgb led, and a memory mapped SPI module.

memory map:
```
ram 32KB : 0x0000 - 0x7fff
SPI: 256B : 0x8000 - 0x80ff
GPIO: 256B : 0x8100 - 0x81ff
```

Utilization:
```
Info: 	         ICESTORM_LC:  3262/ 5280    61%
Info: 	        ICESTORM_RAM:     4/   30    13%
Info: 	               SB_IO:    12/   96    12%
Info: 	               SB_GB:     8/    8   100%
```

SPI commands
```
0x0     | Nop, does nothing (not seen from cpu)
0x1     | init spi (not seen from cpu)
0x2     | Sends 16bits of data (incremental address) (not seen from cpu)
0x3     | Starts the cpu (not seen from cpu)
0x4     | Set LED (param R, G, B)
0x5     | Run gradient (with soft pwm from the riscv)
0x6     | Run fibonacci, next read is result
0x7     | Pow, next read is result
0x8     | Matrix multiplication, write two matrix, and read one matrix
```

SPI MM module registers

```
0x0 status (bit 0: read buffer full, bit 1: write buffer full) (R only)
0x4 read data R only
0x8 assert read W only (write something else than 0)
0xC write data
```

Host sends the firmware to the fpga using spi, which will be saved to memory immediately
with an auto incrementing adress

<!-- ## CPU (describe the cpu, its capabilites and a small benchmark) -->
