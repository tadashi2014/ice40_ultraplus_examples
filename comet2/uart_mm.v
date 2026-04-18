module uart_tx #(
      parameter integer CLK_HZ = 12000000,
      parameter integer BAUD = 115200
   )(
      input wire clk,
      input wire reset,
      input wire wr_en,
      input wire [7:0] wr_data,
      output reg busy,
      output reg tx
   );

   localparam integer CLKS_PER_BIT = CLK_HZ / BAUD;

   reg [3:0] bit_index;
   reg [13:0] clk_count;
   reg [9:0] shift_reg;

   initial begin
      busy = 1'b0;
      tx = 1'b1;
      bit_index = 4'd0;
      clk_count = 14'd0;
      shift_reg = 10'h3FF;
   end

   always @(posedge clk) begin
      if (reset) begin
         busy <= 1'b0;
         tx <= 1'b1;
         bit_index <= 4'd0;
         clk_count <= 14'd0;
         shift_reg <= 10'h3FF;
      end else if (!busy) begin
         tx <= 1'b1;
         clk_count <= 14'd0;
         bit_index <= 4'd0;
         if (wr_en) begin
            busy <= 1'b1;
            shift_reg <= {1'b1, wr_data, 1'b0};
            tx <= 1'b0;
         end
      end else begin
         if (clk_count == CLKS_PER_BIT - 1) begin
            clk_count <= 14'd0;
            if (bit_index == 4'd9) begin
               busy <= 1'b0;
               tx <= 1'b1;
               bit_index <= 4'd0;
            end else begin
               bit_index <= bit_index + 1'b1;
               shift_reg <= {1'b1, shift_reg[9:1]};
               tx <= shift_reg[1];
            end
         end else begin
            clk_count <= clk_count + 1'b1;
         end
      end
   end
endmodule

module uart_rx #(
      parameter integer CLK_HZ = 12000000,
      parameter integer BAUD = 115200
   )(
      input wire clk,
      input wire reset,
      input wire rx,
      output reg data_valid,
      output reg [7:0] data_out
   );

   localparam integer CLKS_PER_BIT = CLK_HZ / BAUD;
   localparam integer HALF_CLKS_PER_BIT = CLKS_PER_BIT / 2;
   localparam integer START_SAMPLE_DELAY = CLKS_PER_BIT + HALF_CLKS_PER_BIT;

   reg rx_sync_0;
   reg rx_sync_1;
   reg receiving;
   reg [3:0] bit_index;
   reg [13:0] clk_count;
   reg [7:0] shift_reg;

   initial begin
      data_valid = 1'b0;
      data_out = 8'h00;
      rx_sync_0 = 1'b1;
      rx_sync_1 = 1'b1;
      receiving = 1'b0;
      bit_index = 4'd0;
      clk_count = 14'd0;
      shift_reg = 8'h00;
   end

   always @(posedge clk) begin
      rx_sync_0 <= rx;
      rx_sync_1 <= rx_sync_0;

      if (reset) begin
         data_valid <= 1'b0;
         data_out <= 8'h00;
         receiving <= 1'b0;
         bit_index <= 4'd0;
         clk_count <= 14'd0;
         shift_reg <= 8'h00;
         rx_sync_0 <= 1'b1;
         rx_sync_1 <= 1'b1;
      end else begin
         data_valid <= 1'b0;

         if (!receiving) begin
            if (rx_sync_1 == 1'b0) begin
               receiving <= 1'b1;
               // Sample the first data bit at the center of bit0, i.e. 1.5 bit times
               // after the falling edge of the start bit.
               clk_count <= START_SAMPLE_DELAY;
               bit_index <= 4'd0;
            end
         end else if (clk_count != 0) begin
            clk_count <= clk_count - 1'b1;
         end else begin
            clk_count <= CLKS_PER_BIT - 1;
            if (bit_index < 4'd8) begin
               shift_reg[bit_index] <= rx_sync_1;
               bit_index <= bit_index + 1'b1;
            end else begin
               receiving <= 1'b0;
               if (rx_sync_1 == 1'b1) begin
                  data_out <= shift_reg;
                  data_valid <= 1'b1;
               end
            end
         end
      end
   end
endmodule

module uart_mm #(
      parameter integer CLK_HZ = 12000000,
      parameter integer BAUD = 115200,
      parameter integer FIFO_DEPTH = 16
   )(
      input wire clk,
      input wire reset,
      input wire uart_rx,
      output wire uart_tx,
      input wire rd_req,
      input wire [31:0] rd_addr,
      output reg [31:0] rd_data,
      output reg data_valid,
      input wire wr_req,
      input wire [31:0] wr_addr,
      input wire [31:0] wr_data
   );

   localparam integer FIFO_BITS = 4;

   wire rx_byte_valid;
   wire [7:0] rx_byte;
   reg tx_wr_en;
   reg [7:0] tx_wr_data;
   wire tx_busy;

   reg [7:0] rx_fifo [0:FIFO_DEPTH-1];
   reg [7:0] tx_fifo [0:FIFO_DEPTH-1];
   reg [FIFO_BITS-1:0] rx_wr_ptr;
   reg [FIFO_BITS-1:0] rx_rd_ptr;
   reg [FIFO_BITS-1:0] tx_wr_ptr;
   reg [FIFO_BITS-1:0] tx_rd_ptr;
   reg [FIFO_BITS:0] rx_count;
   reg [FIFO_BITS:0] tx_count;
   reg rx_overflow;
   reg [7:0] rx_data_latched;
   integer idx;

   uart_rx #(.CLK_HZ(CLK_HZ), .BAUD(BAUD)) uart_rx_inst(
      .clk(clk), .reset(reset), .rx(uart_rx), .data_valid(rx_byte_valid), .data_out(rx_byte)
   );

   uart_tx #(.CLK_HZ(CLK_HZ), .BAUD(BAUD)) uart_tx_inst(
      .clk(clk), .reset(reset), .wr_en(tx_wr_en), .wr_data(tx_wr_data), .busy(tx_busy), .tx(uart_tx)
   );

   initial begin
      rd_data = 32'h0;
      data_valid = 1'b0;
      tx_wr_en = 1'b0;
      tx_wr_data = 8'h00;
      rx_wr_ptr = {FIFO_BITS{1'b0}};
      rx_rd_ptr = {FIFO_BITS{1'b0}};
      tx_wr_ptr = {FIFO_BITS{1'b0}};
      tx_rd_ptr = {FIFO_BITS{1'b0}};
      rx_count = {(FIFO_BITS+1){1'b0}};
      tx_count = {(FIFO_BITS+1){1'b0}};
      rx_overflow = 1'b0;
      rx_data_latched = 8'h00;
      for (idx = 0; idx < FIFO_DEPTH; idx = idx + 1) begin
         rx_fifo[idx] = 8'h00;
         tx_fifo[idx] = 8'h00;
      end
   end

   always @(posedge clk) begin
      if (reset) begin
         rd_data <= 32'h0;
         data_valid <= 1'b0;
         tx_wr_en <= 1'b0;
         tx_wr_data <= 8'h00;
         rx_wr_ptr <= {FIFO_BITS{1'b0}};
         rx_rd_ptr <= {FIFO_BITS{1'b0}};
         tx_wr_ptr <= {FIFO_BITS{1'b0}};
         tx_rd_ptr <= {FIFO_BITS{1'b0}};
         rx_count <= {(FIFO_BITS+1){1'b0}};
         tx_count <= {(FIFO_BITS+1){1'b0}};
         rx_overflow <= 1'b0;
         rx_data_latched <= 8'h00;
      end else begin
         data_valid <= 1'b0;
         tx_wr_en <= 1'b0;

         if (rx_byte_valid) begin
            if (rx_count < FIFO_DEPTH) begin
               rx_fifo[rx_wr_ptr] <= rx_byte;
               rx_wr_ptr <= rx_wr_ptr + 1'b1;
               rx_count <= rx_count + 1'b1;
            end else begin
               rx_overflow <= 1'b1;
            end
         end

         if (!tx_busy && tx_count != 0) begin
            tx_wr_en <= 1'b1;
            tx_wr_data <= tx_fifo[tx_rd_ptr];
            tx_rd_ptr <= tx_rd_ptr + 1'b1;
            tx_count <= tx_count - 1'b1;
         end

         if (rd_req) begin
            data_valid <= 1'b1;
            if (rd_addr == 32'h0) begin
               rd_data <= {28'h0, (tx_count == FIFO_DEPTH), rx_overflow, tx_busy, (rx_count != 0)};
            end else if (rd_addr == 32'h4) begin
               if (rx_count != 0) begin
                  rx_data_latched <= rx_fifo[rx_rd_ptr];
                  rd_data <= {24'h0, rx_fifo[rx_rd_ptr]};
                  rx_rd_ptr <= rx_rd_ptr + 1'b1;
                  rx_count <= rx_count - 1'b1;
               end else begin
                  rd_data <= {24'h0, rx_data_latched};
               end
            end else if (rd_addr == 32'h8) begin
               rd_data <= {{(32-(FIFO_BITS+1)){1'b0}}, tx_count};
            end else begin
               rd_data <= 32'h0;
            end
         end

         if (wr_req) begin
            if (wr_addr == 32'h8) begin
               if (tx_count < FIFO_DEPTH) begin
                  tx_fifo[tx_wr_ptr] <= wr_data[7:0];
                  tx_wr_ptr <= tx_wr_ptr + 1'b1;
                  tx_count <= tx_count + 1'b1;
               end
            end else if (wr_addr == 32'hC) begin
               if (wr_data[0]) begin
                  rx_overflow <= 1'b0;
               end
            end
         end
      end
   end
endmodule
