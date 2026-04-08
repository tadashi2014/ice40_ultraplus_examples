# ice40_riscv

RISC-V (`rv32i`) soft CPU for the Lattice iCE40 UltraPlus Breakout Board, based on the `riscv` sample from [damdoy/ice40_ultraplus_examples](https://github.com/damdoy/ice40_ultraplus_examples).

This fork keeps the original idea and architecture:

- a simple RV32I CPU
- 32 KB SPRAM
- memory-mapped GPIO for the RGB LED
- memory-mapped SPI link to a host computer

The current version has been debugged on real hardware and is now working end-to-end.

## Verified Status

The following flow has been confirmed on hardware:

- bitstream programming
- firmware upload over SPI
- CPU start from host command
- RGB LED control
- `pow(125) = 15625`
- `fib(20) = 6765`
- 2x2 matrix multiplication result `20, 4, 35, 2`

Example host output:

```text
calulating 125^2 read: 15625, status: 0xc0
calculating fib(20) read: 6765, status: 0xc0
received 20, status: 0xc0
received 4, status: 0xc0
received 35, status: 0xc0
received 2, status: 0xc0
```

### Functional wiring diagram

```mermaid
flowchart LR
    HOST["Host Computer"] --> HOSTAPP["host_server/host"]
    HOSTAPP --> FTDI["FTDI USB Interface"]
    FTDI --> SPI["SPI link"]

    subgraph FPGA["iCE40 UltraPlus Breakout Board"]
        SPI --> SPIMM["SPI MMIO / firmware loader"]
        SPIMM --> CPU["RV32I CPU"]
        CPU --> RAM["32 KB SPRAM"]
        CPU --> GPIO["GPIO MMIO"]
        GPIO --> LED["RGB LED"]
    end
```

### Bring-up checklist

1. Program `blink` first to confirm the board and LED pins are correct.
2. Program `selftest` next to validate CPU, ROM, and GPIO without SPI.
3. Program `spi_debug` to validate firmware upload and `START_CPU`.
4. Program the production `top` bitstream after the debug path is confirmed.

## Repository Layout

- `riscv/`: production top, debug tops, CPU, memory, firmware loader
- `riscv/host_server/`: host utility and RISC-V firmware
- `spi/`: SPI slave and shared FTDI host library
- `common/`: board constraint file

## Quick Start

### 1. Build and program the production bitstream

```sh
cd riscv
make clean build
make prog
```

### 2. Build the firmware

```sh
cd host_server/firmware
make clean
make
```

### 3. Build and run the host utility

```sh
cd ..
make clean
make
./host
```

## Toolchain Notes

### FPGA toolchain

This project uses the open-source iCE40 flow:

- `yosys`
- `nextpnr-ice40`
- `icepack`
- `iceprog`

### RISC-V firmware toolchain

This repository uses `riscv64-unknown-elf-*` tools with `-march=rv32i -mabi=ilp32`.

That is intentional: Homebrew and several newer toolchains ship the 64-bit prefix, while still supporting RV32 targets.

### Host FTDI library

The host build detects `libftdi1` or `libftdi` via `pkg-config`.

## Debug Builds

Several hardware debug tops were added to make bring-up and regression checks easier.

### Blink test

Confirms basic FPGA programming and LED wiring.

```sh
cd riscv
make clean blink
make prog_blink
```

### Self-test

Runs a tiny ROM program embedded in the bitstream, without SPI firmware loading.

```sh
make clean selftest
make prog_selftest
```

Expected result:

- blue during reset
- red after reset release

This confirms:

- CPU instruction fetch
- decode/execute
- ROM timing
- GPIO MMIO

### SPI debug

Confirms firmware upload and CPU start over SPI.

```sh
make clean spi_debug
make prog_spi_debug
cd host_server/firmware && make clean && make
cd .. && make clean && make
./host
```

Expected LED sequence:

- blue: waiting for `START_CPU`
- purple: firmware loaded, CPU still held in reset
- red: CPU started and firmware-controlled GPIO is working

## Memory Map

```text
RAM   : 0x0000 - 0x7fff
SPI   : 0x8000 - 0x80ff
GPIO  : 0x8100 - 0x81ff
```

## SPI Commands

```text
0x0  NOP
0x1  INIT
0x2  SEND_FIRMWARE (16-bit chunks, auto-incrementing address)
0x3  START_CPU
0x4  SET_LED
0x5  RUN_GRADIENT
0x6  FIBONACCI
0x7  POW
0x8  MATRIX_MULT
```

## Changes From Upstream `damdoy/ice40_ultraplus_examples`

This repository started from the upstream `riscv` sample and adds both portability fixes and hardware-debugging fixes.

### Build and portability changes

- switched firmware build from `riscv32-unknown-elf-*` to configurable `riscv64-unknown-elf-*`
- made the firmware build freestanding with `-nostdlib` and `-lgcc`
- updated the host build to use `pkg-config` for `libftdi1` / `libftdi`
- simplified the RTL build inputs to this repository layout

### New debug infrastructure

- added `top_blink.v`
- added `top_timer_debug.v`
- added `top_spi_debug.v`
- added `top_selftest.v`
- added `rom.v` for CPU-only ROM self-test
- added make targets for `blink`, `debug`, `spi_debug`, and `selftest`

### Hardware fixes

- fixed top-level startup issues caused by uninitialized control registers
- initialized `cpu_read_req_buf` so the first instruction fetch is not missed on real FPGA hardware
- initialized and default-cleared GPIO read-side control signals
- fixed self-test ROM reset/init handling
- held SPI logic in reset during CPU-only self-test

### SPI / firmware-path fixes

- stabilized SPI packet handoff
- fixed firmware write acknowledge handling in the top-level path
- kept firmware words from being silently overwritten or dropped during transfer
- added firmware-side pacing for `assert_read` and write-buffer availability

### Firmware updates

- fixed LED color encoding
- removed the unnecessary `math.h` dependency
- kept software multiply working through `libgcc`
- verified `pow`, `fib`, and matrix-multiply commands on hardware

## Current Result

Compared with the upstream sample, this fork is focused on:

- reproducible hardware bring-up
- real-board debug visibility
- modern macOS/Homebrew-friendly build flow
- confirmed end-to-end operation on the iCE40 UltraPlus Breakout Board
