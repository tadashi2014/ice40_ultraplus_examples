# COMET2 Firmware Reload Redesign

This note describes a safer redesign for repeated COMET2 firmware downloads
without rewriting the FPGA bitstream each time.

The immediate goal is:

- keep the current first-load path working
- allow back-to-back firmware reloads in one FPGA session
- avoid stale SPI output packets from the previous firmware run
- avoid large simultaneous host/FPGA protocol changes

## Current Stable Baseline

The currently stable baseline is the `3a2dfe8` host/FPGA behavior:

- `comet2_spi_loader -f ...` works for the first firmware load
- `comet2_spi_run -f ...` works for interactive SPI I/O
- `--burst` based on batching 8 legacy `SPI_SEND_FIRMWARE` packets works
- SPI status handling remains the original `STATUS_FPGA_RECV_MASK` /
  `STATUS_FPGA_SEND_MASK` handshake

This baseline must remain untouched while reload support is added.

## What Failed In The First Reload Attempt

The earlier reload attempt changed too many layers at once:

- `spi_send()` treated `0x10/0x20/0x30` as hard send failures
- loader/run added aggressive host-side session recovery
- `spi_mm.v` and `spi_slave.v` were modified to special-case `SPI_INIT`
  and clear internal queues

That broke even the very first firmware transfer:

- `failed to send SPI_INIT`
- `firmware transfer failed at byte 0 (status=20)`

The key lesson is:

- do not change the normal firmware send/ack path while debugging reload
- do not make `spi_send()` interpret payload bytes as transport errors

## Root Cause Model

The likely root problem is not that `SPI_INIT` itself cannot be sent.
The likely problem is:

1. the previous firmware leaves host-bound SPI output pending
2. the next host transaction sees those stale bytes first
3. the host interprets them as the response for the new command

So the redesign should focus on:

- detecting whether stale output exists before a new load
- draining that output in a controlled way
- only then issuing the next `SPI_INIT`

without changing the normal `spi_send()` semantics.

## Redesign Principles

1. Preserve first-load behavior
2. Change one layer at a time
3. Add observability before adding recovery
4. Keep the legacy protocol as the primary path
5. Add reload support as an opt-in path first

## Proposed Architecture

### Phase 1: Observe, Do Not Recover

Add a small diagnostic mode in host tools:

- `comet2_spi_loader --probe-reload`
- `comet2_spi_run --probe-reload`

Behavior:

1. perform a bounded `spi_read()` drain loop before `SPI_INIT`
2. print what was drained
3. send normal checked `SPI_INIT`
4. print the first returned status byte
5. do not retry or reinterpret status in `spi_send()`

This phase answers:

- Is stale data actually present?
- Is it always `0x20`?
- Does it appear before `SPI_INIT`, after `SPI_INIT`, or at first firmware word?

### Phase 2: Explicit Pre-Init Drain Only

Once Phase 1 confirms the stale-output pattern, add a minimal reload path:

- `comet2_spi_loader --reload-safe`
- `comet2_spi_run --reload-safe`

Behavior:

1. bounded `spi_read()` drain loop before load
2. normal checked `SPI_INIT`
3. normal firmware transfer
4. normal `SPI_START_CPU`

Important restrictions:

- do not change `spi_send()`
- do not add unchecked `SPI_INIT`
- do not modify FPGA logic yet

This isolates whether a pure host-side pre-drain is sufficient.

### Phase 3: Add A Dedicated Host-Visible Reset Command

If Phase 2 is not enough, add one new command:

- `SPI_RESET_SESSION = 0x04`

This command is not a firmware word write and not a CPU command.
Its only job is to reset the SPI transport/session state.

FPGA-side contract:

- accepted regardless of current CPU state
- clears host-bound write queue
- clears unread host-to-FPGA command queue
- does not change firmware RAM contents
- does not start CPU
- does not write firmware

Host-side sequence:

1. `SPI_RESET_SESSION`
2. short drain
3. normal checked `SPI_INIT`
4. normal firmware transfer
5. normal `SPI_START_CPU`

This is safer than overloading `SPI_INIT` because it separates:

- transport reset
- CPU init

### Phase 4: Optional Auto-Reload Integration

Only after the above works should the hardware regression helper call it:

- `verify_hardware_cases.sh --suite runtime`

Auto-load policy:

- use plain loader by default
- enable reload-safe path only when explicitly requested
- record whether a case used plain load or reload-safe load

## Recommended Implementation Order

1. Restore and keep the current stable baseline
2. Add host-side probe mode only
3. Validate probe output on hardware
4. Add host-side pre-init drain only
5. Validate repeated loads on hardware
6. If still needed, add `SPI_RESET_SESSION`
7. Only then connect it to `verify_hardware_cases.sh`

## Non-Goals For The Next Attempt

The next reload attempt should explicitly avoid:

- changing `spi_send()` retry semantics
- treating `0x10/0x20/0x30` as transport failure inside `spi_send()`
- unchecked `SPI_INIT` retries
- changing both `spi_mm.v` and `spi_slave.v` before host-side evidence exists
- modifying burst packet semantics

## Success Criteria

Minimum success:

- first load still works exactly as before
- second load works without rewriting the bitstream

Stronger success:

- three consecutive loads succeed:
  - `uart_tx_test.bin`
  - `uart_echo.bin`
  - `scanf_demo.bin`

Regression success:

- `verify_hardware_cases.sh --suite runtime`
  can reuse one bitstream across the suite when reload-safe mode is enabled

## Suggested First Hardware Sequence

When implementing the redesign, validate in this order:

1. `./comet2_spi_loader -f firmware/uart_tx_test.bin`
2. `./comet2_spi_loader -f firmware/uart_echo.bin`
3. `./comet2_spi_loader -f ../../CCompiler/out/scanf_demo.bin`

Then repeat the same three with the reload-safe path.

If the second load fails, log:

- drained byte count before `SPI_INIT`
- first status after `SPI_INIT`
- first status at firmware byte 0

That gives enough information to decide whether Phase 3 is needed.
