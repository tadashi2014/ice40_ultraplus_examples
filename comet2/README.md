# iCE40up5K_comet2

COMET II soft CPU for the Lattice iCE40 UltraPlus Breakout Board.

This README lives under `COMET2_HW/comet2/`, the hardware example area for the
Lattice iCE40 UltraPlus Breakout Board in this repository.

This directory contains the COMET II implementation that was split out from the
mixed `riscv/` tree so the RISC-V and COMET II flows can be maintained
independently.

For repeated firmware-download work, see
[`RELOAD_REDESIGN.md`](RELOAD_REDESIGN.md).

## Layout

- `top.v`: default SPI loader + UART console top
- `tests/`: legacy SPI top, timer debug top, and self-test top/ROM
- `comet2_cpu/`: COMET II core, 16-bit memory, and CASL/sample support files
- `host_server/`: generic SPI loader for COMET II `.bin`
- `host_server/asm_tools/`: CASL2 assembler, linker, and obj2bin
- `host_server/firmware/`: CASL2 sample sources, generated `.bin`, and build `Makefile`
- `tests/host_server/`: legacy SPI IN/OUT echo utility and paired SPI debug firmware

## Build

### Default bitstream

```sh
cd comet2
make clean
make
make prog
```

This now builds the SPI loader + UART console top.

### Legacy SPI-only bitstream

```sh
cd comet2
make clean spi
make prog_spi
```

The `spi`, `debug`, and `selftest` targets are built from files under
[`tests/`](tests/).

### Self-test bitstream

```sh
cd comet2
make clean selftest
make prog_selftest
```

### SPI loader / UART console test

```sh
cd comet2
make clean
make
make prog
cd tests/host_server
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
UART MMIO   : 0xf200 - 0xf2ff
Timer MMIO  : 0xf300 - 0xf3ff
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

## SPI firmware loader

The default `top.v` bitstream lets the host write a `.bin` image into COMET II RAM and
then start execution. This build also exposes a UART console at `115200 8N1`.

```sh
cd host_server
make
./comet2_spi_loader -f ./firmware/hanoi.bin
```

For faster firmware loading, use `--burst`. This batches 8 legacy firmware
packets into one SPI transaction, reducing host-side FTDI/USB round trips.
It requires a bitstream built with the updated `spi_slave.v` receive queue.

```sh
cd host_server
make
./comet2_spi_loader --burst -f ./firmware/hanoi.bin
./comet2_spi_run --burst -f ./firmware/hanoi.bin
```

For repeated-download investigation, the host tools also support two
conservative reload helpers:

- `--probe-reload`: keep the normal load path, but print a bounded pre-INIT
  drain report and the `SPI_INIT` acknowledgement status
- `--reload-safe`: replace the fixed pre-INIT drain with a bounded
  "drain until quiet" pass, while keeping the normal checked `SPI_INIT`
  and normal firmware transfer path

Examples:

```sh
./comet2_spi_loader --probe-reload -f ./firmware/hanoi.bin
./comet2_spi_loader --reload-safe -f ./firmware/hanoi.bin
./comet2_spi_run --probe-reload -f ./firmware/hanoi.bin
```

`comet2_spi_run` and `comet2_spi_loader` both support interactive line editing
for firmware that uses SPI-backed `SVC 1`.

- `Backspace` / `Delete`: remove one not-yet-sent input character from the
  current host-side line buffer
- `Ctrl-D` at the beginning of a line: send a single `0x04` byte so the
  SPI/UART SVC handlers can return `-1` for `IN`
- `Ctrl-D` after one or more characters: treat it as end-of-line and send the
  buffered line

`comet2_spi_run` also exits immediately on `Ctrl-C`.

Both tools support local echo control in interactive mode:

- `--local-echo` / `--no-local-echo`
- `--echo-newline` / `--no-echo-newline`

The current defaults differ slightly:

- `comet2_spi_run`: local echo on, newline echo on
- `comet2_spi_loader`: local echo on, newline echo off

`comet2_spi_run` still supports `-e <ms>` to tune the post-EOF drain window for
non-interactive EOF cases.

Optional input bytes can be sent through `SVC 1`.

```sh
cd tests/host_server
make
./comet2_inout_host "HELLO\n"
```

The host collects bytes produced by `SVC 2` into a 256-byte receive buffer. If
the program outputs more than that, excess bytes are drained from SPI and
discarded.

## Firmware Build Variants

`host_server/firmware/Makefile` now selects the SVC vector/runtime pair with
`IO_VARIANT`.

- `IO_VARIANT=uart`
  Uses `vector_uart.cas` and `svc_handler_uart.cas`
- `IO_VARIANT=spi`
  Uses `vector_spi.cas` and `svc_handler_spi.cas`

Example:

```sh
cd host_server/firmware
make IO_VARIANT=uart echo.bin
make IO_VARIANT=spi echo.bin
```

The generated output filename is the same in both modes, so building the second
variant overwrites the first unless you rename or copy the `.bin`.

## UART Console MMIO

In the default `top.v` build, UART is mapped at `0xF200`-`0xF203`.

- `0xF200`: status
  `bit0=RX ready`, `bit1=TX busy`, `bit2=RX overflow`, `bit3=TX FIFO full`
- `0xF201`: RX data, one byte per read
- `0xF202`: TX data, write low byte to send
- `0xF203`: control, write bit0=`1` to clear RX overflow

Current UART-backed SVC handlers are in
[`host_server/firmware/svc_handler_uart.cas`](host_server/firmware/svc_handler_uart.cas).

Current sample programs in `host_server/firmware/`:

- [`echo.cas`](host_server/firmware/echo.cas): `IN` / `OUT` echo loop
- [`tx_test.cas`](host_server/firmware/tx_test.cas): repeated `OUT` test
- [`mmio_uart_tx_test.cas`](host_server/firmware/mmio_uart_tx_test.cas): direct UART TX MMIO test
- [`mmio_uart_rxtx_test.cas`](host_server/firmware/mmio_uart_rxtx_test.cas): direct UART RX/TX MMIO echo test
- [`mmio_timer_test.cas`](host_server/firmware/mmio_timer_test.cas): timer + UART MMIO polling test
- [`hanoi.cas`](host_server/firmware/hanoi.cas): larger sample program

`host_server/firmware` では `make` で主要サンプルの `.bin` をまとめて生成できます。

Example build flow:

```sh
cd host_server/firmware
make IO_VARIANT=uart echo.bin
```

`echo.bin` を UART コンソール向けに作るならこちらです。

```sh
cd host_server/firmware
make IO_VARIANT=uart echo.bin
```

TX 単体確認ならこちらです。

```sh
cd host_server/firmware
make IO_VARIANT=uart tx_test.bin
```

`OUT` / `SVC 2` を通さず UART MMIO を直叩きして切り分けるならこちらです。

```sh
cd host_server/firmware
make IO_VARIANT=uart mmio_uart_tx_test.bin
```

RX も含めて MMIO 直結で echo を確認するならこちらです。

```sh
cd host_server/firmware
make IO_VARIANT=uart mmio_uart_rxtx_test.bin
```

SPI-backed `IN` / `OUT` handlers are in
[`host_server/firmware/svc_handler_spi.cas`](host_server/firmware/svc_handler_spi.cas).
SPI variant firmware can be built with the same sample sources by selecting
`IO_VARIANT=spi`.

```sh
cd host_server/firmware
make IO_VARIANT=spi echo.bin
```

## Timer MMIO

In the default `top.v` build, a polling timer is mapped at `0xF300`-`0xF305`.

- `0xF300`: status
  `bit0=enable`, `bit1=periodic`, `bit2=fired`
- `0xF301`: current counter low 16 bits
- `0xF302`: compare value low 16 bits
- `0xF303`: current counter high 16 bits
- `0xF304`: compare value high 16 bits
- `0xF305`: control
  `bit0=enable`, `bit1=periodic`, `bit2=clear fired`, `bit3=clear counter`

Typical one-shot flow:

1. write compare value to `0xF302`
2. write `0x000C` to `0xF305` to clear `fired` and reset the counter
3. write `0x0001` to `0xF305` to start the timer
4. poll `0xF300` until `bit2` becomes `1`

This timer does not generate CPU interrupts yet. It is intended for busy-wait
or periodic polling from COMET II software.

A simple timer polling sample is in
[`host_server/firmware/mmio_timer_test.cas`](host_server/firmware/mmio_timer_test.cas).
It programs the timer for about 1 second, waits for `fired`, prints `TICK`,
and repeats forever.

```sh
cd host_server/firmware
make IO_VARIANT=uart mmio_timer_test.bin
```
