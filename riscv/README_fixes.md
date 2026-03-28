# RISC-V on iCE40 UltraPlus – Fix Summary

## Problem Overview
The RISC-V CPU implementation on iCE40 UltraPlus FPGA failed to load firmware via SPI and start the CPU correctly.

---

## Modified Files

### 1. `spi/spi_slave.v` ★ Required

**FIX1: Set `rd_data_available=1` when INIT opcode is received**

Before the fix, `rd_data_available` was never set after receiving the INIT opcode (0x01), so `spi_mm` could not detect the INIT command.

```verilog
// Before: only state transition, rd_data_available stayed 0
if(counter_read >= 31) begin
    if(rd_data_local[8:1] == 8'h1) begin
        state_rd <= RD_WAIT_DATA;
    end
end

// After
if(counter_read >= 31) begin
    if(rd_data_local[8:1] == 8'h1) begin
        state_rd <= RD_WAIT_DATA;
        rd_data <= {mosi_reg[0], rd_data_local[31:1]};
        rd_data_available <= 1; // added
    end
end
```

**FIX2: Remove `counter_read==0` condition from `buffer_rd_ack` clear**

Previously, `rd_data_available` was only cleared when both `buffer_rd_ack==1` AND `counter_read==0`. If a new SPI transaction started at the same cycle as the idle condition, the clear was missed, blocking the next packet reception.

```verilog
// Before
if(buffer_rd_ack == 1 && counter_read == 0) begin

// After: clear immediately after rd_ack
if(buffer_rd_ack == 1) begin
```

---

### 2. `riscv/spi_mm.v` ★ Required

**Always return `rd_ack=1` for all opcodes to avoid blocking `spi_slave`**

```verilog
// opcode 0x02 (SEND FIRMWARE):
// Only update firm_data when firm_wr==0 to avoid overwriting data
// still being processed by top.v
if(firm_wr == 0) begin
    firm_data <= spi_module_rd_data[23:8];
end
firm_wr <= 1;
rd_ack <= 1; // always return ack

// opcode 0x03 (START CPU)
cpu_start <= 1;
rd_ack <= 1; // always return ack
```

---

### 3. `riscv/top.v` ★ Required

**Key changes:**

- Added `INIT_CPU` state to support repeated `./host` execution
- 1-second timer to auto-release `cpu_reset` (for FLASH boot and power-on)
- Removed `cpu_ever_error`/`cpu_ever_started` debug latches
- All LED control delegated to `gpio_mm` (firmware controls LEDs)

```verilog
// Timer: release cpu_reset after 1 second
reg [23:0] timer;
reg timer_fired;

// LEDs: fully controlled by gpio_mm (firmware)
assign LED_R = gpio_led_r;
assign LED_G = gpio_led_g;
assign LED_B = gpio_led_b;

// Timer logic
if(timer_fired == 0) begin
    timer <= timer + 1;
    if(timer == 24'd11999999) begin  // 1 second @ 12MHz
        timer_fired <= 1;
        cpu_reset <= 0;
    end
end

// INIT_CPU state: reset CPU and counter for re-loading firmware
INIT_CPU: begin
    cpu_reset <= 1;
    counter_firmware_address <= 0;
    firmware_data_buf <= 0;
    state <= REQ_READ_SPI_STATUS;
end
```

---

### 4. `riscv/host_server/firmware/start.S` ★ Required

```asm
.globl _start
_start:
    lui  sp, 0x4
    addi sp, sp, -4
    j main
loop: j loop
```
Note: trailing newline is required.

---

### 5. `riscv/host_server/firmware/main.c` ★ Required

```c
// Add wait after assert_read to allow status register to clear
*(spi_addr+2) = 1;
for(volatile int w = 0; w < 16; w++);

// Check write buffer is free before sending result
while((*spi_addr & 0x2) != 0){}
*(spi_addr+3) = ret;
```

---

### 6. `riscv/host_server/main.c` ★ Required

```c
// Wait 1 second before sending SPI_INIT.
// After the timer fires and cpu_reset goes low, the SPI interface
// needs ~0.5 seconds to stabilize before it can accept commands.
sleep(1);
spi_send(SPI_INIT, no_param, NULL);
```

---

## Approaches Tried but Not Required

| Approach | Reason |
|---|---|
| `(* keep *)` on state register | No effect against yosys FSM optimization |
| `(* fsm_encoding = "none" *)` | Not needed with timer-based design |
| Shrinking `state` to `[3:0]` | Not needed |
| Setting `REQ_READ_SPI_STATUS=0`, removing from `initial` | Timer is the fundamental fix |
| Direct timer reset via `spi_cpu_init` wire | Did not work (nonblocking assignment ordering issue) |
| Removing state machine, using direct logic | Broke firmware write |
| `-nofsm` yosys flag | Not supported in this version of yosys |

---

## Root Causes Summary

| # | Location | Root Cause |
|---|---|---|
| 1 | `spi_slave.v` | `rd_data_available` not set on INIT opcode → `spi_mm` could not detect INIT |
| 2 | `spi_slave.v` | `rd_data_available` clear condition too strict → next packet blocked |
| 3 | `spi_mm.v` | `rd_ack` not always returned → `spi_slave` blocked on subsequent packets |
| 4 | `top.v` | No auto-start mechanism → CPU never started without explicit SPI_START_CPU (required for FLASH boot) |
| 5 | `host/main.c` | SPI unstable for ~0.5s after timer fires → commands rejected without initial wait |

## Correct Usage

```bash
# Write bitstream to SRAM (volatile)
make prog

# Load firmware and run (can be executed immediately after make prog)
cd host_server && ./host
```

The 1-second timer in `top.v` auto-releases `cpu_reset` after power-on or bitstream load.
`./host` waits 1 second before sending SPI_INIT, ensuring the SPI interface is stable.
This means `make prog && cd host_server && ./host` works reliably in all cases.

## Note on FLASH Boot

`make prog_flash` (writing to FLASH) was tested but `status: 0xff` was returned,
indicating SPI communication failure. On iCE40 UltraPlus, the SPI pins are shared
between the FLASH and the user design, which requires additional handling.
FLASH-based boot should be addressed separately (e.g., via UART/serial loading).
