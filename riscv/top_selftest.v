// Self-test: a 5-instruction firmware ROM is embedded directly in this file.
// No SPI, no SPRAM, no host computer required.
//
// The ROM drives the complete CPU pipeline:
//   IF (fetch from ROM) -> ID (decode) -> EX (execute) -> WB -> GPIO write
//
// Firmware (hard-coded in ROM):
//   0x00: lui  a0, 8         (0x00008537)  a0 = 0x8000
//   0x04: addi a0, a0, 256   (0x10050513)  a0 = 0x8100  (GPIO base addr)
//   0x08: addi a1, x0, 1     (0x00100593)  a1 = 1
//   0x0C: sw   a1, 0(a0)     (0x00B52023)  gpio_reg[0] = 1  -> LED_R ON
//   0x10: jal  x0, 0         (0x0000006F)  infinite loop
//
// LED behaviour (active-low: signal 0 = ON, signal 1 = OFF):
//   LED_R ON  : CPU fetched, decoded, and executed the firmware correctly.
//               GPIO path also works. Problem is in the SPI firmware-load path.
//   LED_G ON  : cpu_error_instruction fired.  The ROM encoding has a bug
//               (should never happen with the encodings above).
//   LED_B ON  : CPU is still in power-on reset (only lasts a few clock cycles,
//               effectively invisible to the naked eye).
//
// Build and program:
//   make selftest
//   make prog_selftest

`include "gpio_mm.v"
`include "simple_cpu.v"

module top(input [3:0] SW, input clk,
           output LED_R, output LED_G, output LED_B,
           input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO);

   // --- CPU interface ---
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

   // --- GPIO interface ---
   reg gpio_rd_req;
   reg [31:0] gpio_rd_addr;
   wire [31:0] gpio_rd_data;
   wire gpio_rd_valid;
   reg gpio_wr_req;
   reg [31:0] gpio_wr_addr;
   reg [31:0] gpio_wr_data;
   wire gpio_led_r, gpio_led_g, gpio_led_b;

   // --- Debug latch: latches on if any error_instruction occurs ---
   reg cpu_ever_error;

   // --- Power-on reset: hold reset for 16 clock cycles (~1.3 us at 12 MHz) ---
   reg [3:0] rst_ctr;

   simple_cpu simple_cpu_inst(
      .clk(clk), .reset(cpu_reset),
      .read_req(cpu_read_req), .read_addr(cpu_read_addr),
      .read_data(cpu_read_data), .read_data_valid(cpu_read_data_valid),
      .write_req(cpu_write_req), .write_addr(cpu_write_addr),
      .write_data(cpu_write_data), .memory_mask(cpu_memory_mask),
      .error_instruction(cpu_error_instruction), .debug(cpu_debug)
   );

   gpio_mm gpio_mm_inst(
      .clk(clk), .reset(1'b0),
      .LED_R(gpio_led_r), .LED_G(gpio_led_g), .LED_B(gpio_led_b),
      .rd_req(gpio_rd_req), .rd_addr(gpio_rd_addr),
      .rd_data(gpio_rd_data), .data_valid(gpio_rd_valid),
      .wr_req(gpio_wr_req), .wr_addr(gpio_wr_addr), .wr_data(gpio_wr_data)
   );

   // LED assignments (active-low)
   assign LED_R = gpio_led_r;                           // firmware-controlled
   assign LED_G = cpu_ever_error ? 1'b0 : gpio_led_g;  // error latch
   assign LED_B = cpu_reset     ? 1'b0 : 1'b1;         // reset indicator
   assign SPI_MISO = 1'b1;

   initial begin
      cpu_reset    = 1;
      rst_ctr      = 0;
      cpu_ever_error = 0;
   end

   // ================================================================
   // Instruction ROM (combinatorial, word-addressed by cpu_read_addr)
   // ================================================================
   // Encoding verification:
   //   lui  a0, 8       : [31:12]=0x00008  [11:7]=01010(a0)  [6:0]=0110111 = 0x00008537
   //   addi a0, a0, 256 : imm=0x100 rs1=01010 f3=000 rd=01010 op=0010011  = 0x10050513
   //   addi a1, x0, 1   : imm=0x001 rs1=00000 f3=000 rd=01011 op=0010011  = 0x00100593
   //   sw   a1, 0(a0)   : imm=0 rs2=01011 rs1=01010 f3=010 imm=0 op=0100011 = 0x00B52023
   //   jal  x0, 0       : offset=0 rd=00000 op=1101111                      = 0x0000006F
   reg [31:0] rom_out;
   always @(*) begin
      case (cpu_read_addr[4:2])
         3'h0: rom_out = 32'h00008537; // lui  a0, 8
         3'h1: rom_out = 32'h10050513; // addi a0, a0, 256  -> a0 = 0x8100
         3'h2: rom_out = 32'h00100593; // addi a1, x0, 1
         3'h3: rom_out = 32'h00B52023; // sw   a1, 0(a0)    -> LED_R ON
         3'h4: rom_out = 32'h0000006F; // jal  x0, 0        -> infinite loop
         default: rom_out = 32'h00000013; // NOP (addi x0, x0, 0)
      endcase
   end

   // ================================================================
   // Memory bus controller
   // ================================================================
   // ROM read latency: 2 cycles after the rising edge of cpu_read_req.
   // This matches memory.v so the CPU (which waits for data_valid) works unchanged.
   reg cpu_read_req_buf; // used for rising-edge detection
   reg rom_rd_pending;   // pipelined: asserted one cycle before data_valid

   always @(posedge clk) begin

      // --- Power-on reset (16 cycles) ---
      if (rst_ctr != 4'hF) begin
         rst_ctr   <= rst_ctr + 1;
         cpu_reset <= 1;
      end else begin
         cpu_reset <= 0;
      end

      // --- Error latch ---
      if (cpu_error_instruction) cpu_ever_error <= 1;

      // --- Defaults (overridden below if needed) ---
      cpu_read_data_valid <= 0;
      rom_rd_pending      <= 0;
      gpio_wr_req         <= 0;
      gpio_rd_req         <= 0;

      // --- Rising edge of cpu_read_req -> dispatch to ROM or GPIO ---
      cpu_read_req_buf <= cpu_read_req;
      if (cpu_read_req_buf == 0 && cpu_read_req == 1) begin
         if (cpu_read_addr[31:8] == 24'h000081) begin
            // GPIO memory-mapped read (0x8100 - 0x81ff)
            gpio_rd_req  <= 1;
            gpio_rd_addr <= cpu_read_addr[7:0];
         end else begin
            // ROM read (address 0x0000 - 0x7fff and any other range)
            rom_rd_pending <= 1;
         end
      end

      // --- ROM: return data on the cycle after rom_rd_pending ---
      // rom_out is combinatorial from cpu_read_addr, which stays stable
      // while the CPU is in ID state waiting for data_valid.
      if (rom_rd_pending) begin
         cpu_read_data       <= rom_out;
         cpu_read_data_valid <= 1;
      end

      // --- GPIO read response ---
      if (gpio_rd_valid) begin
         cpu_read_data       <= gpio_rd_data;
         cpu_read_data_valid <= 1;
      end

      // --- CPU write dispatch ---
      if (cpu_write_req) begin
         if (cpu_write_addr[31:8] == 24'h000081) begin
            // GPIO memory-mapped write (0x8100 - 0x81ff)
            gpio_wr_req  <= 1;
            gpio_wr_addr <= cpu_write_addr[7:0];
            gpio_wr_data <= cpu_write_data;
         end
         // Writes to ROM addresses are silently ignored
      end

   end

endmodule
