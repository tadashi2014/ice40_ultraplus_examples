// RV32I register file: 32 x 32-bit registers, x0 is hardwired to 0.
// Synchronous read (1-clock latency): output is registered so that
// the ID_BUF pipeline stage in simple_cpu.v has time to settle.

module register_file(
   input  wire        clk,
   input  wire        reset,
   input  wire        rd_1_en,
   input  wire        rd_2_en,
   input  wire  [4:0] read_reg_1,
   input  wire  [4:0] read_reg_2,
   input  wire  [4:0] write_reg,
   input  wire [31:0] write_data,
   input  wire        write_en,
   output reg  [31:0] data_out_1,
   output reg  [31:0] data_out_2
);

   reg [31:0] regs [0:31];

   integer i;
   initial begin
      for (i = 0; i < 32; i = i + 1)
         regs[i] = 32'b0;
      data_out_1 = 32'b0;
      data_out_2 = 32'b0;
   end

   always @(posedge clk) begin
      // Write port (x0 cannot be written)
      if (write_en && write_reg != 5'b0) begin
         regs[write_reg] <= write_data;
      end

      // Read port 1
      if (rd_1_en) begin
         data_out_1 <= (read_reg_1 == 5'b0) ? 32'b0 : regs[read_reg_1];
      end

      // Read port 2
      if (rd_2_en) begin
         data_out_2 <= (read_reg_2 == 5'b0) ? 32'b0 : regs[read_reg_2];
      end
   end

endmodule
