// receive: byte2 | byte1 | byte0 | opcode/status
//read all the data, but can write only the two bytes as opcode contains metadata

module spi_slave(input wire clk, input wire reset,
      input wire SPI_SCK, input wire SPI_SS, input wire SPI_MOSI, output wire SPI_MISO,
      output wire wr_buffer_free, input wire wr_en, input wire [23:0] wr_data,
      output reg rd_data_available = 0, input wire rd_ack, output reg [31:0] rd_data
   );

   reg [4:0] counter_read = 0; //max 32

   reg [1:0] spi_clk_reg = 0;
   reg [1:0] spi_ss_reg = 0;
   wire spi_ss_falling_edge;
   wire spi_ss_rising_edge;

   reg [1:0] mosi_reg = 0;
   reg miso_out_reg = 0;
   reg [7:0] state_rd = 1;

   reg wr_reg_full = 0;
   reg [23:0] wr_data_reg; //written data to send to spi/miso
   reg wr_queue_full = 0;
   reg [23:0] wr_data_queue = 0; //waiting to be written in the register

   reg buffer_rd_ack = 0;
   reg [31:0] rd_data_local = 0;
   reg [31:0] rx_fifo [0:7];
   reg [2:0] rx_fifo_wr_ptr = 0;
   reg [2:0] rx_fifo_rd_ptr = 0;
   reg [3:0] rx_fifo_count = 0;

   //states
   parameter IDLE = 0, INIT=IDLE+1, RD_WAIT_DATA=INIT+1, RD_WAIT_ACK=RD_WAIT_DATA+1, WR_WAIT_DATA=RD_WAIT_ACK+1, WR_WAIT_ACK=WR_WAIT_DATA+1;

   assign SPI_MISO = miso_out_reg;
   wire spi_clk_rising_edge;
   wire spi_clk_falling_edge;
   assign spi_clk_rising_edge = (spi_clk_reg[1:0] == 2'b01);
   assign spi_clk_falling_edge = (spi_clk_reg[1:0] == 2'b10);
   assign spi_ss_rising_edge = (spi_ss_reg[1:0] == 2'b01);
   assign spi_ss_falling_edge = (spi_ss_reg[1:0] == 2'b10);

   initial begin
      counter_read = 0;
      spi_clk_reg = 0;
      spi_ss_reg = 0;
      mosi_reg = 0;
      miso_out_reg = 0;
      state_rd = INIT;
      wr_reg_full = 0;
      wr_data_reg = 24'hcafe77;
      wr_queue_full = 0;
      wr_data_queue = 0;

      buffer_rd_ack = 0;
      rd_data = 0;
      rd_data_local = 0;
      rx_fifo_wr_ptr = 0;
      rx_fifo_rd_ptr = 0;
      rx_fifo_count = 0;

      rd_data_available = 0;
   end

   assign wr_buffer_free = ((~wr_queue_full) & (~wr_reg_full) & (~wr_en));

   always @(posedge clk)
   begin
      if(reset == 1) begin
         rd_data <= 0;
         rd_data_local <= 0;
         rd_data_available <= 0;
         buffer_rd_ack <= 0;
         rx_fifo_wr_ptr <= 0;
         rx_fifo_rd_ptr <= 0;
         rx_fifo_count <= 0;
         state_rd <= INIT;
         counter_read <= 0;
         spi_clk_reg <= 0;
         spi_ss_reg <= 0;
         mosi_reg <= 0;
         miso_out_reg <= 0;
         wr_reg_full <= 0;
         wr_queue_full <= 0;
         wr_data_queue <= 0;
      end else begin

         spi_clk_reg <= {spi_clk_reg[0], SPI_SCK};
         mosi_reg <= {mosi_reg[0], SPI_MOSI};
         spi_ss_reg <= {spi_ss_reg[0], SPI_SS};

         if (spi_ss_falling_edge == 1 || spi_ss_rising_edge == 1) begin
            counter_read <= 0;
         end

         if(spi_clk_rising_edge == 1'b1) begin //default on spi clk
            miso_out_reg <= 0; //default
         end

         case (state_rd)
         INIT : begin // wait the init opcode from host (0x1) and nothing else
            if(spi_clk_rising_edge == 1'b1) begin
               rd_data_local[31:0] <= {mosi_reg[0], rd_data_local[31:1]};
               counter_read <= counter_read + 1;

               if(counter_read == 5) begin //status, write master to slave successful
                  miso_out_reg <= 1;
               end

               if(counter_read >= 31) begin //finish recv
                  if(rd_data_local[8:1] == 8'h1 && rx_fifo_count < 8) begin //received init opcode
                     state_rd <= RD_WAIT_DATA;
                     // Start a fresh session on INIT. Old host-visible output
                     // packets from the previous run would otherwise leak into
                     // the next SPI transaction and corrupt the observed
                     // status byte during firmware reload.
                     wr_reg_full <= 0;
                     wr_data_reg <= 24'h0;
                     wr_queue_full <= 0;
                     wr_data_queue <= 24'h0;
                     rx_fifo_wr_ptr <= 0;
                     rx_fifo_rd_ptr <= 0;
                     rx_fifo_count <= 0;
                     // FIX1: INIT opcode を spi_mm に通知するため rd_data_available をセット。
                     // 修正前はここで rd_data_available が 0 のままで、spi_mm が
                     // cpu_init を立てられず、INIT_CPU ステートに入れなかった。
                     rx_fifo[0] <= {mosi_reg[0], rd_data_local[31:1]};
                     rx_fifo_wr_ptr <= 1;
                     rx_fifo_count <= 1;
                     rd_data <= {mosi_reg[0], rd_data_local[31:1]};
                     rd_data_available <= 1;
                  end
                  counter_read <= 0;
               end
            end
         end
         RD_WAIT_DATA : begin
            if(spi_clk_rising_edge == 1'b1) begin
               if(counter_read == 5) begin //status OK
                  miso_out_reg <= 1;
               end

               if (wr_reg_full == 1) begin //something ready to be written back to host
                  if(counter_read == 6) begin
                     miso_out_reg <= 1;
                  end else if(counter_read >= 7 && counter_read < 31) begin
                     miso_out_reg <= wr_data_reg[0];
                     wr_data_reg[23:0] <= {wr_data_reg[0], wr_data_reg[23:1]};
                  end
               end

               rd_data_local[31:0] <= {mosi_reg[0], rd_data_local[31:1]};
               counter_read <= counter_read + 1;

               if(counter_read >= 31) begin //finish recv
                  if (wr_reg_full == 1) begin
                     wr_reg_full <= 0;
                     wr_data_reg <= 24'h00;
                  end

                  if (rd_data_local[8:1] == 8'h01) begin
                     // INIT starts a new loader session. Flush any queued
                     // host-bound output and stale host commands from the
                     // previous run, then present only the INIT packet to
                     // spi_mm.
                     wr_reg_full <= 0;
                     wr_data_reg <= 24'h0;
                     wr_queue_full <= 0;
                     wr_data_queue <= 24'h0;
                     rx_fifo_wr_ptr <= 0;
                     rx_fifo_rd_ptr <= 0;
                     rx_fifo_count <= 1;
                     rx_fifo[0] <= {mosi_reg[0], rd_data_local[31:1]};
                     rd_data <= {mosi_reg[0], rd_data_local[31:1]};
                     rd_data_available <= 1;
                  end else if(rx_fifo_count < 8) begin
                     rx_fifo[rx_fifo_wr_ptr] <= {mosi_reg[0], rd_data_local[31:1]};
                     rx_fifo_wr_ptr <= rx_fifo_wr_ptr + 1'b1;
                     rx_fifo_count <= rx_fifo_count + 1'b1;
                     if(rd_data_available == 0) begin
                        rd_data <= {mosi_reg[0], rd_data_local[31:1]};
                     end
                     rd_data_available <= 1;
                  end
                  state_rd <= RD_WAIT_DATA;
                  counter_read <= 0;
               end
            end
         end
         default : begin
         end
         endcase

         // rd_ack 受信を記録
         if(rd_ack == 1 && buffer_rd_ack == 0) begin
            buffer_rd_ack <= 1;
         end

         if(buffer_rd_ack == 1) begin
            if(rx_fifo_count > 0) begin
               rx_fifo_rd_ptr <= rx_fifo_rd_ptr + 1'b1;
               rx_fifo_count <= rx_fifo_count - 1'b1;
               if(rx_fifo_count > 1) begin
                  rd_data <= rx_fifo[rx_fifo_rd_ptr + 1'b1];
                  rd_data_available <= 1;
               end else begin
                  rd_data_available <= 0;
               end
            end else begin
               rd_data_available <= 0;
            end
            buffer_rd_ack <= 0;
         end

         //write to the queue
         if (wr_en == 1 && (~wr_reg_full) && (~wr_queue_full) ) begin
            wr_queue_full <= 1;
            wr_data_queue <= wr_data;
         end

         //move from queue to reg only when no com (counter_read == 0)
         if(wr_queue_full == 1 && counter_read == 0) begin
            wr_data_reg <= wr_data_queue;
            wr_queue_full <= 0;
            wr_reg_full <= 1;
         end
      end
   end
endmodule
