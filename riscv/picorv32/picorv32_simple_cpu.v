`include "picorv32/picorv32.v"

/*
 * Compatibility wrapper for the original simple_cpu bus used by the riscv
 * example tops.  The public module name intentionally stays "simple_cpu" so
 * the memory, SPI, and GPIO glue can remain unchanged while the core inside is
 * PicoRV32.
 */
module simple_cpu(
   input wire clk,
   input wire reset,
   output reg read_req,
   output reg [31:0] read_addr,
   input wire [31:0] read_data,
   input wire read_data_valid,
   output reg write_req,
   output reg [31:0] write_addr,
   output reg [31:0] write_data,
   output reg [3:0] memory_mask,
   output wire error_instruction,
   output wire [31:0] debug
);

   wire resetn = !reset;

   wire        mem_valid;
   wire        mem_instr;
   reg         mem_ready;
   wire [31:0] mem_addr;
   wire [31:0] mem_wdata;
   wire [3:0]  mem_wstrb;
   reg  [31:0] mem_rdata;
   wire        trap;

   localparam BUS_IDLE      = 2'd0;
   localparam BUS_WAIT_READ = 2'd1;
   localparam BUS_ACK       = 2'd2;

   reg [1:0] bus_state;

   assign error_instruction = trap;
   assign debug = mem_addr;

   picorv32 #(
      .ENABLE_COUNTERS(0),
      .ENABLE_COUNTERS64(0),
      .ENABLE_REGS_16_31(1),
      .ENABLE_REGS_DUALPORT(0),
      .LATCHED_MEM_RDATA(0),
      .TWO_STAGE_SHIFT(0),
      .BARREL_SHIFTER(0),
      .TWO_CYCLE_COMPARE(0),
      .TWO_CYCLE_ALU(0),
      .COMPRESSED_ISA(0),
      .CATCH_MISALIGN(0),
      .CATCH_ILLINSN(0),
      .ENABLE_PCPI(0),
      .ENABLE_MUL(0),
      .ENABLE_FAST_MUL(0),
      .ENABLE_DIV(0),
      .ENABLE_IRQ(0),
      .REGS_INIT_ZERO(0),
      .PROGADDR_RESET(32'h0000_0000),
      .STACKADDR(32'h0000_3ffc)
   ) picorv32_inst (
      .clk(clk),
      .resetn(resetn),
      .trap(trap),
      .mem_valid(mem_valid),
      .mem_instr(mem_instr),
      .mem_ready(mem_ready),
      .mem_addr(mem_addr),
      .mem_wdata(mem_wdata),
      .mem_wstrb(mem_wstrb),
      .mem_rdata(mem_rdata),
      .mem_la_read(),
      .mem_la_write(),
      .mem_la_addr(),
      .mem_la_wdata(),
      .mem_la_wstrb(),
      .pcpi_valid(),
      .pcpi_insn(),
      .pcpi_rs1(),
      .pcpi_rs2(),
      .pcpi_wr(1'b0),
      .pcpi_rd(32'b0),
      .pcpi_wait(1'b0),
      .pcpi_ready(1'b0),
      .irq(32'b0),
      .eoi(),
      .trace_valid(),
      .trace_data()
   );

   initial begin
      read_req = 1'b0;
      read_addr = 32'b0;
      write_req = 1'b0;
      write_addr = 32'b0;
      write_data = 32'b0;
      memory_mask = 4'b0;
      mem_ready = 1'b0;
      mem_rdata = 32'b0;
      bus_state = BUS_IDLE;
   end

   always @(posedge clk) begin
      if (reset == 1'b1) begin
         read_req <= 1'b0;
         read_addr <= 32'b0;
         write_req <= 1'b0;
         write_addr <= 32'b0;
         write_data <= 32'b0;
         memory_mask <= 4'b0;
         mem_ready <= 1'b0;
         mem_rdata <= 32'b0;
         bus_state <= BUS_IDLE;
      end else begin
         read_req <= 1'b0;
         write_req <= 1'b0;
         mem_ready <= 1'b0;

         case (bus_state)
         BUS_IDLE: begin
            if (mem_valid == 1'b1) begin
               if (mem_wstrb == 4'b0000) begin
                  read_req <= 1'b1;
                  read_addr <= mem_addr;
                  memory_mask <= 4'b1111;
                  bus_state <= BUS_WAIT_READ;
               end else begin
                  write_req <= 1'b1;
                  write_addr <= mem_addr;
                  write_data <= mem_wdata;
                  memory_mask <= mem_wstrb;
                  mem_ready <= 1'b1;
                  bus_state <= BUS_ACK;
               end
            end
         end
         BUS_WAIT_READ: begin
            if (read_data_valid == 1'b1) begin
               mem_rdata <= read_data;
               mem_ready <= 1'b1;
               bus_state <= BUS_ACK;
            end
         end
         BUS_ACK: begin
            if (mem_valid == 1'b0) begin
               bus_state <= BUS_IDLE;
            end
         end
         default: begin
            bus_state <= BUS_IDLE;
         end
         endcase
      end
   end

endmodule
