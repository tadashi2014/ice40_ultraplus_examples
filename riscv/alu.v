// RV32I ALU
// control[3:0] = {instruction[30], funct3[2:0]}
//
// control | operation
// --------+----------
//  0000   | ADD
//  1000   | SUB
//  0001   | SLL  (shift left logical)
//  0010   | SLT  (signed less-than)
//  0011   | SLTU (unsigned less-than)
//  0100   | XOR
//  0101   | SRL  (shift right logical)
//  1101   | SRA  (shift right arithmetic)
//  0110   | OR
//  0111   | AND

module alu(
   input  wire [31:0] in1,
   input  wire [31:0] in2,
   input  wire  [3:0] control,
   output reg  [31:0] out,
   output wire        zero,
   output wire        neg
);

   assign zero = (out == 32'b0);
   assign neg  = out[31];

   always @(*) begin
      case (control)
         4'b0000: out = in1 + in2;
         4'b1000: out = in1 - in2;
         4'b0001: out = in1 << in2[4:0];
         4'b0010: out = ($signed(in1) < $signed(in2)) ? 32'b1 : 32'b0;
         4'b0011: out = (in1 < in2)                   ? 32'b1 : 32'b0;
         4'b0100: out = in1 ^ in2;
         4'b0101: out = in1 >> in2[4:0];
         4'b1101: out = $signed(in1) >>> in2[4:0];
         4'b0110: out = in1 | in2;
         4'b0111: out = in1 & in2;
         default: out = 32'b0;
      endcase
   end

endmodule
