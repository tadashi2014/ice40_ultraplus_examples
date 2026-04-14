/*
 * COMET II self-test ROM, word-addressed.
 *
 * The program repeatedly lights R, G, B through GPIO at word address 0xC100.
 * After the last delay it executes SVC 0.  The CPU treats SVC 0 as a reset
 * vector jump plus SP reload from mem[31], so the pattern restarts.
 */
module comet2_selftest_rom(input wire clk, input wire reset,
      input wire rd_req, input wire [15:0] rd_addr,
      output reg [15:0] rd_data, output reg data_valid
   );

   reg buf_rd_req;
   reg [15:0] buf_rd_addr;

   initial begin
      buf_rd_req = 1'b0;
      buf_rd_addr = 16'b0;
      data_valid = 1'b0;
   end

   always @(posedge clk)
   begin
      if (reset == 1'b1) begin
         buf_rd_req <= 1'b0;
         buf_rd_addr <= 16'b0;
         data_valid <= 1'b0;
      end else begin
         buf_rd_req <= rd_req;
         buf_rd_addr <= rd_addr;
         data_valid <= buf_rd_req;
      end
   end

   always @(*)
   begin
      case (buf_rd_addr)
      16'h0000: rd_data = 16'h0020; // reset vector
      16'h001F: rd_data = 16'hBFF0; // initial SP

      // START:
      16'h0020: rd_data = 16'h1210; // LAD GR1,GPIO
      16'h0021: rd_data = 16'hC100;
      16'h0022: rd_data = 16'h1240; // LAD GR4,1 (loop decrement)
      16'h0023: rd_data = 16'h0001;

      16'h0024: rd_data = 16'h1220; // LAD GR2,1 (red)
      16'h0025: rd_data = 16'h0001;
      16'h0026: rd_data = 16'h1121; // ST GR2,0,GR1
      16'h0027: rd_data = 16'h0000;
      16'h0028: rd_data = 16'h1250; // LAD GR5,16 (outer wait)
      16'h0029: rd_data = 16'h0010;
      16'h002A: rd_data = 16'h1230; // WAIT_R_OUTER: LAD GR3,0xffff
      16'h002B: rd_data = 16'hFFFF;
      16'h002C: rd_data = 16'h2534; // WAIT_R_INNER: SUBA GR3,GR4
      16'h002D: rd_data = 16'h6200; // JNZ WAIT_R_INNER
      16'h002E: rd_data = 16'h002C;
      16'h002F: rd_data = 16'h2554; // SUBA GR5,GR4
      16'h0030: rd_data = 16'h6200; // JNZ WAIT_R_OUTER
      16'h0031: rd_data = 16'h002A;

      16'h0032: rd_data = 16'h1220; // LAD GR2,2 (green)
      16'h0033: rd_data = 16'h0002;
      16'h0034: rd_data = 16'h1121; // ST GR2,0,GR1
      16'h0035: rd_data = 16'h0000;
      16'h0036: rd_data = 16'h1250; // LAD GR5,16 (outer wait)
      16'h0037: rd_data = 16'h0010;
      16'h0038: rd_data = 16'h1230; // WAIT_G_OUTER: LAD GR3,0xffff
      16'h0039: rd_data = 16'hFFFF;
      16'h003A: rd_data = 16'h2534; // WAIT_G_INNER: SUBA GR3,GR4
      16'h003B: rd_data = 16'h6200; // JNZ WAIT_G_INNER
      16'h003C: rd_data = 16'h003A;
      16'h003D: rd_data = 16'h2554; // SUBA GR5,GR4
      16'h003E: rd_data = 16'h6200; // JNZ WAIT_G_OUTER
      16'h003F: rd_data = 16'h0038;

      16'h0040: rd_data = 16'h1220; // LAD GR2,4 (blue)
      16'h0041: rd_data = 16'h0004;
      16'h0042: rd_data = 16'h1121; // ST GR2,0,GR1
      16'h0043: rd_data = 16'h0000;
      16'h0044: rd_data = 16'h1250; // LAD GR5,16 (outer wait)
      16'h0045: rd_data = 16'h0010;
      16'h0046: rd_data = 16'h1230; // WAIT_B_OUTER: LAD GR3,0xffff
      16'h0047: rd_data = 16'hFFFF;
      16'h0048: rd_data = 16'h2534; // WAIT_B_INNER: SUBA GR3,GR4
      16'h0049: rd_data = 16'h6200; // JNZ WAIT_B_INNER
      16'h004A: rd_data = 16'h0048;
      16'h004B: rd_data = 16'h2554; // SUBA GR5,GR4
      16'h004C: rd_data = 16'h6200; // JNZ WAIT_B_OUTER
      16'h004D: rd_data = 16'h0046;

      16'h004E: rd_data = 16'hF000; // SVC 0
      16'h004F: rd_data = 16'h0000;
      16'h0050: rd_data = 16'h6400; // JUMP START (fallback)
      16'h0051: rd_data = 16'h0020;

      default: rd_data = 16'h0000; // NOP
      endcase
   end
endmodule
