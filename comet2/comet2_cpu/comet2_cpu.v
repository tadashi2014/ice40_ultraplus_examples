// COMET II compatible CPU core with the same memory bus as simple_cpu.
// COMET memory addresses are 16-bit word addresses.  The external bus uses
// the same word address for COMET builds; SoC glue converts lanes as needed.

/* verilator lint_off UNUSED */

module comet2_cpu(
clk, reset, read_req, read_addr, read_data, read_data_valid, write_req, write_addr, write_data, memory_mask, error_instruction, debug
);

input wire clk;
input wire reset;
output reg read_req;
output reg [31:0] read_addr;
input wire [31:0] read_data;
input wire read_data_valid;
output reg write_req;
output reg [31:0] write_addr;
output reg [31:0] write_data;
output reg [3:0] memory_mask;
output reg error_instruction;
output reg [31:0] debug;

reg [15:0] gr [0:7];
reg [15:0] pr;
reg [15:0] sp;
reg of;
reg sf;
reg zf;
reg [15:0] svc_vector_addr;
reg [7:0] state;
reg [15:0] ir;
reg [15:0] operand;
reg [15:0] effective_addr;

reg [15:0] write_word;
reg [3:0] mem_read_kind;
reg [3:0] mem_write_kind;
reg [3:0] dest_reg;

integer i;

parameter ST_RESET_PR_REQ   = 8'd0;
parameter ST_RESET_PR_WAIT  = 8'd1;
parameter ST_RESET_SP_REQ   = 8'd2;
parameter ST_RESET_SP_WAIT  = 8'd3;
parameter ST_FETCH_REQ      = 8'd4;
parameter ST_FETCH_WAIT     = 8'd5;
parameter ST_DECODE         = 8'd6;
parameter ST_OPERAND_REQ    = 8'd7;
parameter ST_OPERAND_WAIT   = 8'd8;
parameter ST_EXECUTE        = 8'd9;
parameter ST_MEM_READ_REQ   = 8'd10;
parameter ST_MEM_READ_WAIT  = 8'd11;
parameter ST_MEM_WRITE      = 8'd12;
parameter ST_SVC_VECTOR_REQ = 8'd13;
parameter ST_SVC_VECTOR_WAIT= 8'd14;
parameter ST_SVC0_SP_REQ    = 8'd15;
parameter ST_SVC0_SP_WAIT   = 8'd16;
parameter ST_HALT           = 8'd17;

parameter MR_NONE = 4'd0;
parameter MR_LD   = 4'd1;
parameter MR_ADDA = 4'd2;
parameter MR_SUBA = 4'd3;
parameter MR_ADDL = 4'd4;
parameter MR_SUBL = 4'd5;
parameter MR_AND  = 4'd6;
parameter MR_OR   = 4'd7;
parameter MR_XOR  = 4'd8;
parameter MR_CPA  = 4'd9;
parameter MR_CPL  = 4'd10;
parameter MR_POP  = 4'd11;
parameter MR_RET  = 4'd12;
parameter MR_SVC  = 4'd13;

parameter MW_NONE = 4'd0;
parameter MW_ST   = 4'd1;
parameter MW_PUSH = 4'd2;
parameter MW_CALL = 4'd3;
parameter MW_SVC  = 4'd4;

function [15:0] bus_word;
   input [31:0] data;
   begin
      bus_word = data[15:0];
   end
endfunction

function [31:0] bus_addr;
   input [15:0] word_addr;
   begin
      bus_addr = {16'b0, word_addr};
   end
endfunction

function is_two_word_instruction;
   input [7:0] op;
   begin
      case (op)
      8'h10, 8'h11, 8'h12,
      8'h20, 8'h21, 8'h22, 8'h23,
      8'h30, 8'h31, 8'h32,
      8'h40, 8'h41,
      8'h50, 8'h51, 8'h52, 8'h53,
      8'h61, 8'h62, 8'h63, 8'h64, 8'h65, 8'h66,
      8'h70, 8'h80, 8'hF0:
         is_two_word_instruction = 1'b1;
      default:
         is_two_word_instruction = 1'b0;
      endcase
   end
endfunction

function uses_r_field;
   input [7:0] op;
   begin
      case (op)
      8'h10, 8'h11, 8'h12, 8'h14,
      8'h20, 8'h21, 8'h22, 8'h23, 8'h24, 8'h25, 8'h26, 8'h27,
      8'h30, 8'h31, 8'h32, 8'h34, 8'h35, 8'h36,
      8'h40, 8'h41, 8'h44, 8'h45,
      8'h50, 8'h51, 8'h52, 8'h53,
      8'h71:
         uses_r_field = 1'b1;
      default:
         uses_r_field = 1'b0;
      endcase
   end
endfunction

function uses_x_field;
   input [7:0] op;
   begin
      case (op)
      8'h10, 8'h11, 8'h12,
      8'h20, 8'h21, 8'h22, 8'h23,
      8'h30, 8'h31, 8'h32,
      8'h40, 8'h41,
      8'h50, 8'h51, 8'h52, 8'h53,
      8'h61, 8'h62, 8'h63, 8'h64, 8'h65, 8'h66,
      8'h70, 8'h80, 8'hF0,
      8'h14, 8'h24, 8'h25, 8'h26, 8'h27,
      8'h34, 8'h35, 8'h36, 8'h44, 8'h45:
         uses_x_field = 1'b1;
      default:
         uses_x_field = 1'b0;
      endcase
   end
endfunction

function [15:0] indexed_addr;
   input [15:0] addr;
   input [3:0] x;
   begin
      if (x == 4'd0) begin
         indexed_addr = addr;
      end else if (x < 4'd8) begin
         indexed_addr = addr + gr[x];
      end else begin
         indexed_addr = addr;
      end
   end
endfunction

function signed_add_overflow;
   input [15:0] a;
   input [15:0] b;
   input [15:0] result;
   begin
      signed_add_overflow = (a[15] == b[15]) && (result[15] != a[15]);
   end
endfunction

function signed_sub_overflow;
   input [15:0] a;
   input [15:0] b;
   input [15:0] result;
   begin
      signed_sub_overflow = (a[15] != b[15]) && (result[15] != a[15]);
   end
endfunction

function [15:0] sla_result;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount == 16'd0) begin
         sla_result = value;
      end else if (amount >= 16'd15) begin
         sla_result = {value[15], 15'b0};
      end else begin
         sla_result = {value[15], value[14:0] << amount[3:0]};
      end
   end
endfunction

function sla_overflow;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount == 16'd0) begin
         sla_overflow = 1'b0;
      end else if (amount >= 16'd15) begin
         sla_overflow = value[0];
      end else begin
         sla_overflow = value[15 - amount[3:0]];
      end
   end
endfunction

function [15:0] sra_result;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount == 16'd0) begin
         sra_result = value;
      end else if (amount >= 16'd15) begin
         sra_result = {16{value[15]}};
      end else begin
         sra_result = $signed(value) >>> amount[3:0];
      end
   end
endfunction

function sra_overflow;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount == 16'd0) begin
         sra_overflow = 1'b0;
      end else if (amount >= 16'd16) begin
         sra_overflow = value[15];
      end else begin
         sra_overflow = value[amount[3:0] - 1'b1];
      end
   end
endfunction

function [15:0] sll_result;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount >= 16'd16) begin
         sll_result = 16'h0000;
      end else begin
         sll_result = value << amount[3:0];
      end
   end
endfunction

function sll_overflow;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount == 16'd0) begin
         sll_overflow = 1'b0;
      end else if (amount >= 16'd16) begin
         sll_overflow = value[0];
      end else begin
         sll_overflow = value[16 - amount[3:0]];
      end
   end
endfunction

function [15:0] srl_result;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount >= 16'd16) begin
         srl_result = 16'h0000;
      end else begin
         srl_result = value >> amount[3:0];
      end
   end
endfunction

function srl_overflow;
   input [15:0] value;
   input [15:0] amount;
   begin
      if (amount == 16'd0) begin
         srl_overflow = 1'b0;
      end else if (amount >= 16'd16) begin
         srl_overflow = value[15];
      end else begin
         srl_overflow = value[amount[3:0] - 1'b1];
      end
   end
endfunction

task set_fr;
   input [15:0] result;
   input new_of;
   begin
      of <= new_of;
      sf <= result[15];
      zf <= (result == 16'h0000);
   end
endtask

task fault_instruction;
   begin
      error_instruction <= 1'b1;
      state <= ST_FETCH_REQ;
   end
endtask

initial begin
   read_req = 1'b0;
   read_addr = 32'b0;
   write_req = 1'b0;
   write_addr = 32'b0;
   write_data = 32'b0;
   memory_mask = 4'b0011;
   error_instruction = 1'b0;
   debug = 32'b0;
   pr = 16'b0;
   sp = 16'b0;
   of = 1'b0;
   sf = 1'b0;
   zf = 1'b0;
   ir = 16'b0;
   operand = 16'b0;
   effective_addr = 16'b0;
   svc_vector_addr = 16'b0;
   write_word = 16'b0;
   mem_read_kind = MR_NONE;
   mem_write_kind = MW_NONE;
   dest_reg = 4'b0;
   state = ST_RESET_PR_REQ;
   for (i = 0; i < 8; i = i + 1) begin
      gr[i] = 16'b0;
   end
end

always @(posedge clk)
begin : main
   reg [7:0] op;
   reg [3:0] r;
   reg [3:0] x;
   reg [15:0] rhs;
   reg [15:0] result;
   reg [16:0] wide;
   reg shifted_out;
   reg stack_faulted;

   if (reset == 1'b1) begin
      read_req <= 1'b0;
      read_addr <= 32'b0;
      write_req <= 1'b0;
      write_addr <= 32'b0;
      write_data <= 32'b0;
      memory_mask <= 4'b0011;
      error_instruction <= 1'b0;
      debug <= 32'b0;
      pr <= 16'b0;
      sp <= 16'b0;
      of <= 1'b0;
      sf <= 1'b0;
      zf <= 1'b0;
      ir <= 16'b0;
      operand <= 16'b0;
      effective_addr <= 16'b0;
      svc_vector_addr <= 16'b0;
      write_word <= 16'b0;
      mem_read_kind <= MR_NONE;
      mem_write_kind <= MW_NONE;
      dest_reg <= 4'b0;
      for (i = 0; i < 8; i = i + 1) begin
         gr[i] <= 16'b0;
      end
      state <= ST_RESET_PR_REQ;
   end else begin
      read_req <= 1'b0;
      write_req <= 1'b0;
      memory_mask <= 4'b0011;
      write_data <= {16'b0, write_word};
      debug <= {8'b0, state, pr};

      op = ir[15:8];
      r = ir[7:4];
      x = ir[3:0];
      rhs = 16'h0000;
      result = 16'h0000;
      wide = 17'h00000;
      shifted_out = 1'b0;
      stack_faulted = 1'b0;

      case (state)
      ST_RESET_PR_REQ: begin
         read_addr <= bus_addr(16'h0000);
         read_req <= 1'b1;
         state <= ST_RESET_PR_WAIT;
      end
      ST_RESET_PR_WAIT: begin
         read_addr <= bus_addr(16'h0000);
         if (read_data_valid == 1'b1) begin
            pr <= bus_word(read_data);
            state <= ST_RESET_SP_REQ;
         end else begin
            read_req <= 1'b1;
         end
      end
      ST_RESET_SP_REQ: begin
         read_addr <= bus_addr(16'd31);
         read_req <= 1'b1;
         state <= ST_RESET_SP_WAIT;
      end
      ST_RESET_SP_WAIT: begin
         read_addr <= bus_addr(16'd31);
         if (read_data_valid == 1'b1) begin
            if (bus_word(read_data) > 16'hEFFF) begin
               error_instruction <= 1'b1;
               state <= ST_HALT;
            end else begin
               sp <= bus_word(read_data);
               state <= ST_FETCH_REQ;
            end
         end else begin
            read_req <= 1'b1;
         end
      end
      ST_FETCH_REQ: begin
         read_addr <= bus_addr(pr);
         read_req <= 1'b1;
         state <= ST_FETCH_WAIT;
      end
      ST_FETCH_WAIT: begin
         read_addr <= bus_addr(pr);
         if (read_data_valid == 1'b1) begin
            ir <= bus_word(read_data);
            pr <= pr + 16'd1;
            state <= ST_DECODE;
         end else begin
            read_req <= 1'b1;
         end
      end
      ST_DECODE: begin
         op = ir[15:8];
         if (is_two_word_instruction(op)) begin
            state <= ST_OPERAND_REQ;
         end else begin
            state <= ST_EXECUTE;
         end
      end
      ST_OPERAND_REQ: begin
         read_addr <= bus_addr(pr);
         read_req <= 1'b1;
         state <= ST_OPERAND_WAIT;
      end
      ST_OPERAND_WAIT: begin
         read_addr <= bus_addr(pr);
         if (read_data_valid == 1'b1) begin
            operand <= bus_word(read_data);
            pr <= pr + 16'd1;
            state <= ST_EXECUTE;
         end else begin
            read_req <= 1'b1;
         end
      end
      ST_EXECUTE: begin
         op = ir[15:8];
         r = ir[7:4];
         x = ir[3:0];
         mem_read_kind <= MR_NONE;
         mem_write_kind <= MW_NONE;

         if ((uses_r_field(op) && (r > 4'd7)) || (uses_x_field(op) && (x > 4'd7))) begin
            fault_instruction();
         end else begin
            effective_addr <= indexed_addr(operand, x);
            dest_reg <= r;

            case (op)
            8'h00: begin
               state <= ST_FETCH_REQ;
            end
            8'h10: begin
               mem_read_kind <= MR_LD;
               state <= ST_MEM_READ_REQ;
            end
            8'h11: begin
               write_word <= gr[r];
               mem_write_kind <= MW_ST;
               state <= ST_MEM_WRITE;
            end
            8'h12: begin
               gr[r] <= indexed_addr(operand, x);
               state <= ST_FETCH_REQ;
            end
            8'h14: begin
               gr[r] <= gr[x];
               set_fr(gr[x], 1'b0);
               state <= ST_FETCH_REQ;
            end
            8'h20: begin
               mem_read_kind <= MR_ADDA;
               state <= ST_MEM_READ_REQ;
            end
            8'h21: begin
               mem_read_kind <= MR_SUBA;
               state <= ST_MEM_READ_REQ;
            end
            8'h22: begin
               mem_read_kind <= MR_ADDL;
               state <= ST_MEM_READ_REQ;
            end
            8'h23: begin
               mem_read_kind <= MR_SUBL;
               state <= ST_MEM_READ_REQ;
            end
            8'h24: begin
               result = gr[r] + gr[x];
               gr[r] <= result;
               set_fr(result, signed_add_overflow(gr[r], gr[x], result));
               state <= ST_FETCH_REQ;
            end
            8'h25: begin
               result = gr[r] - gr[x];
               gr[r] <= result;
               set_fr(result, signed_sub_overflow(gr[r], gr[x], result));
               state <= ST_FETCH_REQ;
            end
            8'h26: begin
               wide = {1'b0, gr[r]} + {1'b0, gr[x]};
               gr[r] <= wide[15:0];
               set_fr(wide[15:0], wide[16]);
               state <= ST_FETCH_REQ;
            end
            8'h27: begin
               result = gr[r] - gr[x];
               gr[r] <= result;
               set_fr(result, gr[r] < gr[x]);
               state <= ST_FETCH_REQ;
            end
            8'h30: begin
               mem_read_kind <= MR_AND;
               state <= ST_MEM_READ_REQ;
            end
            8'h31: begin
               mem_read_kind <= MR_OR;
               state <= ST_MEM_READ_REQ;
            end
            8'h32: begin
               mem_read_kind <= MR_XOR;
               state <= ST_MEM_READ_REQ;
            end
            8'h34: begin
               result = gr[r] & gr[x];
               gr[r] <= result;
               set_fr(result, 1'b0);
               state <= ST_FETCH_REQ;
            end
            8'h35: begin
               result = gr[r] | gr[x];
               gr[r] <= result;
               set_fr(result, 1'b0);
               state <= ST_FETCH_REQ;
            end
            8'h36: begin
               result = gr[r] ^ gr[x];
               gr[r] <= result;
               set_fr(result, 1'b0);
               state <= ST_FETCH_REQ;
            end
            8'h40: begin
               mem_read_kind <= MR_CPA;
               state <= ST_MEM_READ_REQ;
            end
            8'h41: begin
               mem_read_kind <= MR_CPL;
               state <= ST_MEM_READ_REQ;
            end
            8'h44: begin
               result = gr[r] - gr[x];
               set_fr(result, signed_sub_overflow(gr[r], gr[x], result));
               state <= ST_FETCH_REQ;
            end
            8'h45: begin
               result = gr[r] - gr[x];
               set_fr(result, gr[r] < gr[x]);
               state <= ST_FETCH_REQ;
            end
            8'h50, 8'h51, 8'h52, 8'h53: begin
               rhs = indexed_addr(operand, x);
               if (op == 8'h50) begin
                  result = sla_result(gr[r[2:0]], rhs);
                  shifted_out = sla_overflow(gr[r[2:0]], rhs);
               end else if (op == 8'h51) begin
                  result = sra_result(gr[r[2:0]], rhs);
                  shifted_out = sra_overflow(gr[r[2:0]], rhs);
               end else if (op == 8'h52) begin
                  result = sll_result(gr[r[2:0]], rhs);
                  shifted_out = sll_overflow(gr[r[2:0]], rhs);
               end else begin
                  result = srl_result(gr[r[2:0]], rhs);
                  shifted_out = srl_overflow(gr[r[2:0]], rhs);
               end
               gr[r] <= result;
               set_fr(result, shifted_out);
               state <= ST_FETCH_REQ;
            end
            8'h61: begin
               if (sf == 1'b1) pr <= indexed_addr(operand, x);
               state <= ST_FETCH_REQ;
            end
            8'h62: begin
               if (zf == 1'b0) pr <= indexed_addr(operand, x);
               state <= ST_FETCH_REQ;
            end
            8'h63: begin
               if (zf == 1'b1) pr <= indexed_addr(operand, x);
               state <= ST_FETCH_REQ;
            end
            8'h64: begin
               pr <= indexed_addr(operand, x);
               state <= ST_FETCH_REQ;
            end
            8'h65: begin
               if ((sf == 1'b0) && (zf == 1'b0)) pr <= indexed_addr(operand, x);
               state <= ST_FETCH_REQ;
            end
            8'h66: begin
               if (of == 1'b1) pr <= indexed_addr(operand, x);
               state <= ST_FETCH_REQ;
            end
            8'h70: begin
               result = sp - 16'd1;
               if (result > 16'hEFFF) begin
                  error_instruction <= 1'b1;
                  state <= ST_HALT;
               end else begin
                  sp <= result;
                  effective_addr <= result;
                  write_word <= indexed_addr(operand, x);
                  mem_write_kind <= MW_PUSH;
                  state <= ST_MEM_WRITE;
               end
            end
            8'h71: begin
               if (r > 4'd7) begin
                  fault_instruction();
               end else if (sp > 16'hEFFF) begin
                  error_instruction <= 1'b1;
                  state <= ST_HALT;
               end else begin
                  effective_addr <= sp;
                  dest_reg <= r;
                  mem_read_kind <= MR_POP;
                  state <= ST_MEM_READ_REQ;
               end
            end
            8'h80: begin
               result = sp - 16'd1;
               if (result > 16'hEFFF) begin
                  error_instruction <= 1'b1;
                  state <= ST_HALT;
               end else begin
                  sp <= result;
                  effective_addr <= result;
                  write_word <= pr;
                  mem_write_kind <= MW_CALL;
                  state <= ST_MEM_WRITE;
               end
            end
            8'h81: begin
               if (sp > 16'hEFFF) begin
                  error_instruction <= 1'b1;
                  state <= ST_HALT;
               end else begin
                  effective_addr <= sp;
                  mem_read_kind <= MR_RET;
                  state <= ST_MEM_READ_REQ;
               end
            end
            8'hF0: begin
               result = indexed_addr(operand, x);
               if (result[7:0] == 8'h00) begin
                  effective_addr <= 16'h0000;
                  mem_read_kind <= MR_SVC;
                  state <= ST_MEM_READ_REQ;
               end else begin
                  rhs = sp - 16'd1;
                  if (rhs > 16'hEFFF) begin
                     error_instruction <= 1'b1;
                     state <= ST_HALT;
                  end else begin
                     sp <= rhs;
                     effective_addr <= rhs;
                     svc_vector_addr <= {8'h00, result[7:0]};
                     write_word <= pr;
                     mem_read_kind <= MR_SVC;
                     mem_write_kind <= MW_SVC;
                     state <= ST_MEM_WRITE;
                  end
               end
            end
            default: begin
               fault_instruction();
            end
            endcase
         end
      end
      ST_MEM_READ_REQ: begin
         read_addr <= bus_addr(effective_addr);
         read_req <= 1'b1;
         state <= ST_MEM_READ_WAIT;
      end
      ST_MEM_READ_WAIT: begin
         read_addr <= bus_addr(effective_addr);
         if (read_data_valid == 1'b1) begin
            rhs = bus_word(read_data);
            case (mem_read_kind)
            MR_LD: begin
               gr[dest_reg] <= rhs;
               set_fr(rhs, 1'b0);
            end
            MR_ADDA: begin
               result = gr[dest_reg] + rhs;
               gr[dest_reg] <= result;
               set_fr(result, signed_add_overflow(gr[dest_reg], rhs, result));
            end
            MR_SUBA: begin
               result = gr[dest_reg] - rhs;
               gr[dest_reg] <= result;
               set_fr(result, signed_sub_overflow(gr[dest_reg], rhs, result));
            end
            MR_ADDL: begin
               wide = {1'b0, gr[dest_reg]} + {1'b0, rhs};
               gr[dest_reg] <= wide[15:0];
               set_fr(wide[15:0], wide[16]);
            end
            MR_SUBL: begin
               result = gr[dest_reg] - rhs;
               gr[dest_reg] <= result;
               set_fr(result, gr[dest_reg] < rhs);
            end
            MR_AND: begin
               result = gr[dest_reg] & rhs;
               gr[dest_reg] <= result;
               set_fr(result, 1'b0);
            end
            MR_OR: begin
               result = gr[dest_reg] | rhs;
               gr[dest_reg] <= result;
               set_fr(result, 1'b0);
            end
            MR_XOR: begin
               result = gr[dest_reg] ^ rhs;
               gr[dest_reg] <= result;
               set_fr(result, 1'b0);
            end
            MR_CPA: begin
               result = gr[dest_reg] - rhs;
               set_fr(result, signed_sub_overflow(gr[dest_reg], rhs, result));
            end
            MR_CPL: begin
               result = gr[dest_reg] - rhs;
               set_fr(result, gr[dest_reg] < rhs);
            end
            MR_POP: begin
               gr[dest_reg] <= rhs;
               result = sp + 16'd1;
               if (result > 16'hEFFF) begin
                  error_instruction <= 1'b1;
                  state <= ST_HALT;
                  stack_faulted = 1'b1;
               end else begin
                  sp <= result;
               end
            end
            MR_RET: begin
               pr <= rhs;
               result = sp + 16'd1;
               if (result > 16'hEFFF) begin
                  error_instruction <= 1'b1;
                  state <= ST_HALT;
                  stack_faulted = 1'b1;
               end else begin
                  sp <= result;
               end
            end
            MR_SVC: begin
               pr <= rhs;
               if (effective_addr == 16'h0000) begin
                  state <= ST_SVC0_SP_REQ;
               end
            end
            default: begin
               error_instruction <= 1'b1;
            end
            endcase
            if (!stack_faulted && !((mem_read_kind == MR_SVC) && (effective_addr == 16'h0000))) begin
               state <= ST_FETCH_REQ;
            end
         end else begin
            read_req <= 1'b1;
         end
      end
      ST_MEM_WRITE: begin
         write_addr <= bus_addr(effective_addr);
         write_data <= {16'b0, write_word};
         memory_mask <= 4'b0011;
         write_req <= 1'b1;
         if (mem_write_kind == MW_CALL) begin
            pr <= indexed_addr(operand, ir[3:0]);
            state <= ST_FETCH_REQ;
         end else if (mem_write_kind == MW_SVC) begin
            state <= ST_SVC_VECTOR_REQ;
         end else begin
            state <= ST_FETCH_REQ;
         end
      end
      ST_SVC_VECTOR_REQ: begin
         read_addr <= bus_addr(svc_vector_addr);
         read_req <= 1'b1;
         state <= ST_SVC_VECTOR_WAIT;
      end
      ST_SVC_VECTOR_WAIT: begin
         read_addr <= bus_addr(svc_vector_addr);
         if (read_data_valid == 1'b1) begin
            pr <= bus_word(read_data);
            state <= ST_FETCH_REQ;
         end else begin
            read_req <= 1'b1;
         end
      end
      ST_SVC0_SP_REQ: begin
         read_addr <= bus_addr(16'd31);
         read_req <= 1'b1;
         state <= ST_SVC0_SP_WAIT;
      end
      ST_SVC0_SP_WAIT: begin
         read_addr <= bus_addr(16'd31);
         if (read_data_valid == 1'b1) begin
            if (bus_word(read_data) > 16'hEFFF) begin
               error_instruction <= 1'b1;
               state <= ST_HALT;
            end else begin
               sp <= bus_word(read_data);
               state <= ST_FETCH_REQ;
            end
         end else begin
            read_req <= 1'b1;
         end
      end
      ST_HALT: begin
      end
      default: begin
         error_instruction <= 1'b1;
         state <= ST_RESET_PR_REQ;
      end
      endcase
   end
end

endmodule
