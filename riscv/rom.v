/*
 * Instruction ROM for self-test: same interface and timing as memory.v
 * so the top-level bus controller needs zero changes.
 *
 * Interface is intentionally identical to memory.v:
 *   rd_req pulsed 1 cycle -> 2 cycles later: data_valid=1, rd_data=instruction
 *
 * Firmware (5 instructions):
 *   0x00: lui  a0, 8         -> a0 = 0x8000
 *   0x04: addi a0, a0, 256   -> a0 = 0x8100  (GPIO base)
 *   0x08: addi a1, x0, 1     -> a1 = 1
 *   0x0C: sw   a1, 0(a0)     -> gpio_reg[0] = 1  => LED_R ON (active-low)
 *   0x10: jal  x0, 0         -> infinite loop
 *
 * All other addresses return NOP (addi x0, x0, 0 = 0x00000013).
 *
 * Implementation note:
 *   rd_data is combinatorial from the registered buf_rd_addr to guarantee
 *   that data_valid and rd_data are always in sync regardless of how the
 *   synthesis tool maps the lookup (LUT or block RAM).  data_valid is
 *   registered (asserted one cycle after buf_rd_req is 1).
 *
 * Encoding verification:
 *   lui  a0, 8       : [31:12]=0x00008  rd=01010(a0)  op=0110111  = 0x00008537
 *   addi a0, a0, 256 : imm=0x100 rs1=01010 f3=000 rd=01010 op=0010011 = 0x10050513
 *   addi a1, x0, 1   : imm=0x001 rs1=00000 f3=000 rd=01011 op=0010011 = 0x00100593
 *   sw   a1, 0(a0)   : imm=0 rs2=01011 rs1=01010 f3=010 op=0100011    = 0x00B52023
 *   jal  x0, 0       : offset=0 rd=00000 op=1101111                   = 0x0000006F
 */
module rom(input wire clk, input wire reset,
      input wire rd_req, input wire [14:0] rd_addr,
      output reg [31:0] rd_data, output reg data_valid,
      input wire wr_req, input wire [14:0] wr_addr, input wire [31:0] wr_data
   );

   reg buf_rd_req;
   reg [14:0] buf_rd_addr;

   // Pipeline stage 1 (registered): latch request and address
   always @(posedge clk)
   begin
      if (reset == 1) begin
         buf_rd_req  <= 1'b0;
         buf_rd_addr <= 15'b0;
         data_valid  <= 1'b0;
      end else begin
         buf_rd_req  <= rd_req;
         buf_rd_addr <= rd_addr;
         data_valid  <= buf_rd_req; // 1 exactly one cycle after rd_req was 1
      end
   end

   // Pipeline stage 2 (combinatorial): instruction lookup from registered address.
   // Keeping rd_data combinatorial guarantees that rd_data is stable and correct
   // whenever data_valid is 1, with no risk of BRAM read-latency mismatches.
   always @(*)
   begin
      case (buf_rd_addr[4:2])
         3'd0: rd_data = 32'h00008537; // lui  a0, 8
         3'd1: rd_data = 32'h10050513; // addi a0, a0, 256 -> 0x8100
         3'd2: rd_data = 32'h00100593; // addi a1, x0, 1
         3'd3: rd_data = 32'h00B52023; // sw   a1, 0(a0)   -> LED_R ON
         3'd4: rd_data = 32'h0000006F; // jal  x0, 0       -> loop
         default: rd_data = 32'h00000013; // NOP
      endcase
   end

endmodule
