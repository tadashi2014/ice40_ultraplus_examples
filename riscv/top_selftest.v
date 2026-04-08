// Self-test: 5-instruction ROM firmware embedded in bitstream (rom.v).
// No SPI, no host computer required. SPI logic is held in reset so the
// result reflects only CPU + ROM + GPIO, not any external FTDI/SPI state.
//
// Derived from top_timer_debug.v — the ONLY difference is that memory.v
// (SPRAM) is replaced by rom.v (same interface, hardcoded instructions).
// The bus controller is byte-for-byte identical to top_timer_debug.v so
// we know the CPU<->memory timing is correct.
//
// Firmware (in rom.v):
//   0x00: lui  a0, 8         -> a0 = 0x8000
//   0x04: addi a0, a0, 256   -> a0 = 0x8100  (GPIO base)
//   0x08: addi a1, x0, 1     -> a1 = 1
//   0x0C: sw   a1, 0(a0)     -> gpio_reg[0]=1 => LED_R ON (active-low)
//   0x10: jal  x0, 0         -> infinite loop
//
// LED behaviour (active-low: signal 0 = ON, signal 1 = OFF):
//   0 – 2 s : LED_B ON (cpu_reset held by timer)
//   After 2 s:
//     LED_R ON              -> CPU + ROM + GPIO all work ✓
//                              Problem is ONLY in the SPI firmware-load path.
//     LED_G ON, LED_R OFF   -> cpu_error_instruction fired (bad opcode from ROM)
//     All OFF               -> CPU not executing (check rom.v / bus controller)
//
// Build and program:
//   make selftest
//   make prog_selftest

`include "gpio_mm.v"
`include "rom.v"
`include "spi_mm.v"
`include "simple_riscv_cpu/simple_cpu/simple_cpu.v"

module top(input [3:0] SW, input clk,
           output LED_R, output LED_G, output LED_B,
           input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO);

   reg cpu_reset;
   wire cpu_read_req;
   wire [31:0] cpu_read_addr;
   reg [31:0] cpu_read_data;
   reg cpu_read_data_valid;
   wire cpu_write_req;
   wire [31:0] cpu_write_addr;
   wire [31:0] cpu_write_data;
   wire [3:0] cpu_memory_mask;
   wire cpu_error_instruction;
   wire [31:0] cpu_debug;

   reg spi_reset = 1;
   reg spi_rd_req;
   reg [31:0] spi_rd_addr;
   wire [31:0] spi_rd_data;
   wire spi_rd_valid;
   reg spi_wr_req;
   reg [31:0] spi_wr_addr;
   reg [31:0] spi_wr_data;
   wire spi_firm_wr;
   wire [15:0] spi_firm_data;
   reg spi_firm_ack;
   wire spi_cpu_start;
   reg spi_cpu_start_ack;
   wire spi_cpu_init;
   reg spi_cpu_init_ack;

   reg gpio_reset;
   reg gpio_rd_req;
   reg [31:0] gpio_rd_addr;
   wire [31:0] gpio_rd_data;
   wire gpio_rd_valid;
   reg gpio_wr_req;
   reg [31:0] gpio_wr_addr;
   reg [31:0] gpio_wr_data;

   // ROM (replaces memory.v — same interface, hardcoded instructions)
   reg rom_rd_req;
   reg [31:0] rom_rd_addr;
   wire [31:0] rom_rd_data;
   wire rom_rd_valid;

   // Timer: 12MHz clock, ~2 seconds = 24_000_000 counts
   reg [24:0] timer;
   reg timer_fired;

   // Debug latches
   reg cpu_ever_error;
   reg cpu_ever_started;

   wire gpio_led_r, gpio_led_g, gpio_led_b;

   spi_mm spi_mm_inst(.clk(clk), .reset(spi_reset),
      .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
      .rd_req(spi_rd_req), .rd_addr(spi_rd_addr), .rd_data(spi_rd_data), .data_valid(spi_rd_valid),
      .wr_req(spi_wr_req), .wr_addr(spi_wr_addr), .wr_data(spi_wr_data),
      .firm_wr(spi_firm_wr), .firm_data(spi_firm_data), .firm_ack(spi_firm_ack),
      .cpu_start(spi_cpu_start), .cpu_start_ack(spi_cpu_start_ack),
      .cpu_init(spi_cpu_init), .cpu_init_ack(spi_cpu_init_ack)
   );

   simple_cpu simple_cpu_inst(.clk(clk), .reset(cpu_reset),
      .read_req(cpu_read_req), .read_addr(cpu_read_addr),
      .read_data(cpu_read_data), .read_data_valid(cpu_read_data_valid),
      .write_req(cpu_write_req), .write_addr(cpu_write_addr),
      .write_data(cpu_write_data), .memory_mask(cpu_memory_mask),
      .error_instruction(cpu_error_instruction), .debug(cpu_debug)
   );

   gpio_mm gpio_mm_inst(.clk(clk), .reset(gpio_reset),
      .LED_R(gpio_led_r), .LED_G(gpio_led_g), .LED_B(gpio_led_b),
      .rd_req(gpio_rd_req), .rd_addr(gpio_rd_addr),
      .rd_data(gpio_rd_data), .data_valid(gpio_rd_valid),
      .wr_req(gpio_wr_req), .wr_addr(gpio_wr_addr), .wr_data(gpio_wr_data)
   );

   rom rom_inst(.clk(clk), .reset(cpu_reset),
      .rd_req(rom_rd_req), .rd_addr(rom_rd_addr[14:0]),
      .rd_data(rom_rd_data), .data_valid(rom_rd_valid),
      .wr_req(1'b0), .wr_addr(15'b0), .wr_data(32'b0)
   );

   // LED:
   // Blue  : ON while cpu_reset=1 (waiting for timer ~2s), OFF after reset release
   // Red   : controlled by ROM self-test firmware via gpio_mm (gpio_reg[0])
   // Green : latches ON if cpu_error_instruction fires after reset release
   assign LED_B = cpu_reset ? 1'b0 : 1'b1;
   assign LED_R = gpio_led_r;
   assign LED_G = cpu_ever_error ? 1'b0 : 1'b1;

   reg [31:0] state;
   parameter IDLE=0, REQ_READ_SPI_STATUS=2, WRITE_MEMORY=6, START_CPU=9, INIT_CPU=10;

   reg [15:0] counter_firmware_address;
   reg [15:0] firmware_data_buf;
   reg cpu_read_req_buf;

   initial begin
      cpu_reset = 1;
      spi_reset = 1;
      spi_wr_req = 0; spi_wr_data = 0;
      spi_firm_ack = 0; spi_cpu_start_ack = 0; spi_cpu_init_ack = 0;
      state = REQ_READ_SPI_STATUS;
      gpio_reset = 0;
      gpio_rd_req = 0; gpio_rd_addr = 0;
      counter_firmware_address = 0;
      firmware_data_buf = 0;
      timer = 0;
      timer_fired = 0;
      cpu_ever_error = 0;
      cpu_ever_started = 0;
      cpu_read_req_buf = 0;
   end

   always @(posedge clk)
   begin
      spi_firm_ack <= 0;
      gpio_rd_req <= 0; gpio_rd_addr <= 0;
      gpio_wr_req <= 0; gpio_wr_addr <= 0; gpio_wr_data <= 0;
      cpu_read_data <= 0; cpu_read_data_valid <= 0;
      cpu_read_req_buf <= cpu_read_req;
      spi_rd_req <= 0; spi_rd_addr <= 0;
      spi_cpu_start_ack <= 0; spi_cpu_init_ack <= 0;
      spi_wr_req <= 0; spi_wr_addr <= 0; spi_wr_data <= 0;
      rom_rd_req <= 0;

      // Error latch
      if(cpu_error_instruction == 1) cpu_ever_error <= 1;

      // ========== TIMER: auto-release cpu_reset after ~2s ==========
      if(timer_fired == 0) begin
         timer <= timer + 1;
         if(timer == 25'h17FFFFF) begin  // ~2s at 12MHz
            timer_fired <= 1;
            cpu_reset <= 0;              // FORCE release reset
         end
      end
      // =============================================================

      // Hold SPI permanently in reset during self-test so floating/external
      // SPI pins cannot affect the CPU reset or control path.
      spi_reset <= 1;

      // CPU read request dispatch (identical to top_timer_debug.v / top.v)
      if(cpu_read_req_buf == 0 && cpu_read_req == 1) begin
         if(cpu_read_addr[31:15] == 17'h0) begin  // 0x0000 - 0x7fff -> ROM
            rom_rd_req  <= 1;
            rom_rd_addr <= cpu_read_addr[14:0];
         end
         if(cpu_read_addr[31:8] == 24'h000080) begin  // SPI
            spi_rd_req  <= 1;
            spi_rd_addr <= cpu_read_addr[7:0];
         end
         if(cpu_read_addr[31:8] == 24'h000081) begin  // GPIO
            gpio_rd_req  <= 1;
            gpio_rd_addr <= cpu_read_addr[7:0];
         end
      end

      if(rom_rd_valid == 1) begin
         cpu_read_data       <= rom_rd_data;
         cpu_read_data_valid <= 1;
      end
      if(spi_rd_valid == 1) begin
         cpu_read_data       <= spi_rd_data;
         cpu_read_data_valid <= 1;
      end
      if(gpio_rd_valid == 1) begin
         cpu_read_data       <= gpio_rd_data;
         cpu_read_data_valid <= 1;
      end

      // CPU write dispatch (identical to top_timer_debug.v / top.v)
      if(cpu_write_req == 1) begin
         if(cpu_write_addr[31:8] == 24'h000080) begin
            spi_wr_req  <= 1;
            spi_wr_addr <= cpu_write_addr[7:0];
            spi_wr_data <= cpu_write_data;
         end
         if(cpu_write_addr[31:8] == 24'h000081) begin
            gpio_wr_req  <= 1;
            gpio_wr_addr <= cpu_write_addr[7:0];
            gpio_wr_data <= cpu_write_data;
         end
      end
   end

endmodule
