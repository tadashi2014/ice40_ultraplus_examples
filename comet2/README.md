# iCE40up5K_comet2

COMET II soft CPU for the Lattice iCE40 UltraPlus Breakout Board.

This directory contains the COMET II implementation that was split out from the
mixed `riscv/` tree so the RISC-V and COMET II flows can be maintained
independently.

## Layout

- `top.v`: production COMET II top
- `top_spi_debug.v`: SPI-oriented debug top
- `top_timer_debug.v`: timer/debug top
- `top_selftest.v`: ROM-based self-test top
- `comet2_selftest_rom.v`: self-test ROM contents
- `comet2_cpu/`: COMET II core, 16-bit memory, and CASL/sample support files
- `host_server/`: host-side SPI echo utility and its paired SPI debug firmware

## Build

### Production bitstream

```sh
cd comet2
make clean
make
make prog
```

### Self-test bitstream

```sh
cd comet2
make clean selftest
make prog_selftest
```

### SPI echo test

```sh
cd comet2
make clean
make
make prog
cd host_server
make
./comet2_inout_host
```

### CPU unit test

`comet2/comet2_cpu/verilator_test/` contains a small Verilator-based unit test
for the COMET II core itself.

```sh
cd comet2/comet2_cpu/verilator_test
make test
```

## COMET II memory map

The COMET II implementation uses 16-bit word addresses.

```text
Main memory : 0x0000 - 0xefff
SPI MMIO    : 0xf000 - 0xf0ff
GPIO MMIO   : 0xf100 - 0xf1ff
```

## Low memory usage

The low-memory area `0x0000` to `0x001f` is reserved for reset and supervisor
entry points.

```text
0x0000 : initial PR value (reset vector)
0x0001 : SVC 1 vector address
0x0002 : SVC 2 vector address
...
0x000f : SVC 15 vector address
0x0010 - 0x001e : reserved
0x001f : initial SP value
```

In other words, the SVC vector table occupies `0x0001` through `0x000f`.

## SVC 0

`SVC 0` is treated as reset-like control flow in this implementation.

- the CPU reloads `SP` from memory word `0x001f`
- the CPU uses vector `0x0000` as the next `PR`
- execution restarts from the reset vector

This matches the board self-test flow, where `SVC 0` is used to restart the ROM
program after a visible LED sequence.

## COMET II assumptions used here

- little-endian CPU
- `SVC` uses the low byte of its operand as a vector number
- SPI-loaded firmware is written as 16-bit words in ascending COMET address order
- the first transmitted 16-bit word becomes COMET memory word `0x0000`
- the value at `0x001f` is used as the initial `SP`
- valid RAM-backed stack/data addresses are `0x0000` through `0xefff`

## SPI IN/OUT echo test

The sample program in `host_server/spi_debug_firmware/` uses:

- `SVC 1` for IN
- `SVC 2` for OUT

The host utility sends ASCII bytes to the COMET II program and expects them to
be echoed back over SPI.
