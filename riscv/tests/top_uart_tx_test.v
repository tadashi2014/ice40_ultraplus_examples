`include "../uart_mm.v"

module top(
   input [3:0] SW,
   input clk,
   output LED_R,
   output LED_G,
   output LED_B,
   input SPI_SCK,
   input SPI_SS,
   input SPI_MOSI,
   output SPI_MISO,
   input UART_RX,
   output UART_TX
);

   reg reset = 1'b0;
   reg tx_wr_en = 1'b0;
   reg [7:0] tx_wr_data = 8'h00;
   wire tx_busy;

   reg [31:0] delay_counter = 32'd0;
   reg [2:0] msg_index = 3'd0;

   uart_tx #(.CLK_HZ(12000000), .BAUD(115200)) uart_tx_inst(
      .clk(clk),
      .reset(reset),
      .wr_en(tx_wr_en),
      .wr_data(tx_wr_data),
      .busy(tx_busy),
      .tx(UART_TX)
   );

   assign LED_R = ~delay_counter[22];
   assign LED_G = ~delay_counter[23];
   assign LED_B = ~delay_counter[24];
   assign SPI_MISO = 1'b1;

   function [7:0] msg_byte;
      input [2:0] index;
      begin
         case (index)
         3'd0: msg_byte = "H";
         3'd1: msg_byte = "E";
         3'd2: msg_byte = "L";
         3'd3: msg_byte = "L";
         3'd4: msg_byte = "O";
         3'd5: msg_byte = 8'h0D;
         3'd6: msg_byte = 8'h0A;
         default: msg_byte = 8'h00;
         endcase
      end
   endfunction

   always @(posedge clk) begin
      tx_wr_en <= 1'b0;
      delay_counter <= delay_counter + 1'b1;

      if (!tx_busy && delay_counter[18:0] == 19'd0) begin
         tx_wr_en <= 1'b1;
         tx_wr_data <= msg_byte(msg_index);
         if (msg_index == 3'd6) begin
            msg_index <= 3'd0;
         end else begin
            msg_index <= msg_index + 1'b1;
         end
      end
   end
endmodule
