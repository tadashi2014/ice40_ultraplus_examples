/*
 * 60K x 16-bit COMET II main memory.
 *
 * COMET word addresses:
 *   0x0000 - 0xEFFF : this RAM
 *
 * The implementation uses four iCE40 UltraPlus SPRAM blocks.  Each block is
 * used as 16K x 16-bit storage, with the upper 4K words left unmapped.
 */
module comet2_memory(input wire clk, input wire reset,
      input wire rd_req, input wire [15:0] rd_addr, output reg [15:0] rd_data, output reg data_valid,
      input wire wr_req, input wire [15:0] wr_addr, input wire [15:0] wr_data
   );

   reg [13:0] ram_addr_0;
   reg [15:0] ram_data_in_0;
   wire [15:0] ram_data_out_0;
   reg ram_wren_0;

   reg [13:0] ram_addr_1;
   reg [15:0] ram_data_in_1;
   wire [15:0] ram_data_out_1;
   reg ram_wren_1;

   reg [13:0] ram_addr_2;
   reg [15:0] ram_data_in_2;
   wire [15:0] ram_data_out_2;
   reg ram_wren_2;

   reg [13:0] ram_addr_3;
   reg [15:0] ram_data_in_3;
   wire [15:0] ram_data_out_3;
   reg ram_wren_3;

   reg buf_rd_req;
   reg [1:0] buf_bank;
   reg [15:0] buf_rd_addr;

   SB_SPRAM256KA spram0(
     .ADDRESS(ram_addr_0), .DATAIN(ram_data_in_0), .MASKWREN(4'b1111),
     .WREN(ram_wren_0), .CHIPSELECT(1'b1), .CLOCK(clk),
     .STANDBY(1'b0), .SLEEP(1'b0), .POWEROFF(1'b1), .DATAOUT(ram_data_out_0)
   );

   SB_SPRAM256KA spram1(
     .ADDRESS(ram_addr_1), .DATAIN(ram_data_in_1), .MASKWREN(4'b1111),
     .WREN(ram_wren_1), .CHIPSELECT(1'b1), .CLOCK(clk),
     .STANDBY(1'b0), .SLEEP(1'b0), .POWEROFF(1'b1), .DATAOUT(ram_data_out_1)
   );

   SB_SPRAM256KA spram2(
     .ADDRESS(ram_addr_2), .DATAIN(ram_data_in_2), .MASKWREN(4'b1111),
     .WREN(ram_wren_2), .CHIPSELECT(1'b1), .CLOCK(clk),
     .STANDBY(1'b0), .SLEEP(1'b0), .POWEROFF(1'b1), .DATAOUT(ram_data_out_2)
   );

   SB_SPRAM256KA spram3(
     .ADDRESS(ram_addr_3), .DATAIN(ram_data_in_3), .MASKWREN(4'b1111),
     .WREN(ram_wren_3), .CHIPSELECT(1'b1), .CLOCK(clk),
     .STANDBY(1'b0), .SLEEP(1'b0), .POWEROFF(1'b1), .DATAOUT(ram_data_out_3)
   );

   initial begin
      data_valid = 1'b0;
      buf_rd_req = 1'b0;
      buf_bank = 2'b0;
      buf_rd_addr = 16'b0;
   end

   always @(*)
   begin
      ram_addr_0 = rd_addr[13:0];
      ram_addr_1 = rd_addr[13:0];
      ram_addr_2 = rd_addr[13:0];
      ram_addr_3 = rd_addr[13:0];
      ram_data_in_0 = wr_data;
      ram_data_in_1 = wr_data;
      ram_data_in_2 = wr_data;
      ram_data_in_3 = wr_data;
      ram_wren_0 = 1'b0;
      ram_wren_1 = 1'b0;
      ram_wren_2 = 1'b0;
      ram_wren_3 = 1'b0;

      if (wr_req == 1'b1) begin
         if (wr_addr[15:14] == 2'b00) begin
            ram_addr_0 = wr_addr[13:0];
            ram_wren_0 = 1'b1;
         end else if (wr_addr[15:14] == 2'b01) begin
            ram_addr_1 = wr_addr[13:0];
            ram_wren_1 = 1'b1;
         end else if (wr_addr[15:14] == 2'b10) begin
            ram_addr_2 = wr_addr[13:0];
            ram_wren_2 = 1'b1;
         end else if ((wr_addr[15:14] == 2'b11) && (wr_addr[13:12] != 2'b11)) begin
            ram_addr_3 = wr_addr[13:0];
            ram_wren_3 = 1'b1;
         end
      end
   end

   always @(posedge clk)
   begin
      if (reset == 1'b1) begin
         buf_rd_req <= 1'b0;
         buf_rd_addr <= 16'b0;
         data_valid <= 1'b0;
      end else begin
         buf_rd_req <= rd_req;
         buf_bank <= rd_addr[15:14];
         buf_rd_addr <= rd_addr;
         data_valid <= 1'b0;

         if (buf_rd_req == 1'b1) begin
            if (buf_bank == 2'b00) begin
               rd_data <= ram_data_out_0;
            end else if (buf_bank == 2'b01) begin
               rd_data <= ram_data_out_1;
            end else if (buf_bank == 2'b10) begin
               rd_data <= ram_data_out_2;
            end else if (buf_rd_addr[13:12] != 2'b11) begin
               rd_data <= ram_data_out_3;
            end else begin
               // 0xF000-0xFFFF is not backed by RAM in this build.
               rd_data <= 16'h0000;
            end
            data_valid <= 1'b1;
         end
      end
   end
endmodule
