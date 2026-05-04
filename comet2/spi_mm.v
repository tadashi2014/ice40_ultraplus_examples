/*registers:
   0x0 status (bit 0: read buffer full, bit 1: write buffer full) (R only)
   0x4 read data R only
   0x8 assert read W only (write something else than 0)
   0xC write data

   opcodes
   0x0     | Nop
   0x1     | init (consumed by spi_slave, also forwarded to spi_mm for cpu_init)
   0x2     | Sends 16bits of data (incremental address) => write to ram
   0x3     | Starts the cpu (deassert reset)
   above   | put in register for CPU to read

   //byte2 | byte1 | byte0 | opcode/status
*/
`include "spi_slave.v"

module spi_mm(input wire clk, input wire reset,
      input wire SPI_SCK, input wire SPI_SS, input wire SPI_MOSI, output wire SPI_MISO,
      input wire rd_req, input wire [31:0] rd_addr, output reg [31:0] rd_data, output reg data_valid,
      input wire wr_req, input wire [31:0] wr_addr, input wire [31:0] wr_data,
      output reg firm_wr, output reg [15:0] firm_data, input wire firm_ack,
      output reg cpu_start, input wire cpu_start_ack,
      output reg cpu_init, input wire cpu_init_ack
   );

   wire wr_buffer_free;
   reg wr_en;
   reg [23:0] spi_module_wr_data;
   wire rd_data_available;
   reg rd_ack;
   wire [31:0] spi_module_rd_data;

   spi_slave spi_slave_inst(.clk(clk), .reset(reset),
      .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
      .wr_buffer_free(wr_buffer_free), .wr_en(wr_en), .wr_data(spi_module_wr_data),
      .rd_data_available(rd_data_available), .rd_ack(rd_ack), .rd_data(spi_module_rd_data)
   );

   reg[31:0] status_register;
   reg[31:0] read_data_register;
   reg[31:0] assert_read_register;
   reg[31:0] write_data_register;
   localparam OP_NOP               = 8'h0;
   localparam OP_INIT              = 8'h1;
   localparam OP_SEND_FIRMWARE     = 8'h2;
   localparam OP_START_CPU         = 8'h3;

   initial begin
      wr_en = 0;
      spi_module_wr_data = 0;
      rd_ack = 0;
      status_register = 0;
      read_data_register = 0;
      assert_read_register = 0;
      write_data_register = 0;
      firm_wr = 0;
      firm_data = 0;
      cpu_start = 0;
      cpu_init = 0;
   end

   always @(posedge clk)
   begin
      if(reset == 1) begin
         wr_en <= 0;
         spi_module_wr_data <= 0;
         rd_ack <= 0;
         cpu_start <= 0;
         cpu_init <= 0;
         status_register <= 32'h0;
         read_data_register <= 0;
         assert_read_register <= 0;
         write_data_register <= 0;
         firm_wr <= 0;
         firm_data <= 0;
      end else begin

         //defaults
         rd_ack <= 0;
         rd_data <= 0;
         data_valid <= 0;
         wr_en <= 0;

         status_register[1] <= ~wr_buffer_free;
         status_register[2] <= 1;

         if(rd_data_available == 1) begin
            if(spi_module_rd_data[7:0] == OP_INIT) begin //INIT
               // INIT は status_register[0] の値に関係なく常に処理する。
               // 前回の実行で status_register[0]==1 が残っている場合でも
               // 2 回目以降の実行で SPI_INIT がドロップされないようにするため。
               // CPU をリセットするため未処理の受信データは不要なので
               // status_register[0] もここでクリアする。
               cpu_init <= 1;
               rd_ack <= 1;
               status_register[0] <= 0;
               firm_wr <= 0;
            end else if(status_register[0] == 0) begin
               if(spi_module_rd_data[7:0] == OP_NOP) begin //NOP
                  rd_ack <= 1;
               end else if(spi_module_rd_data[7:0] == OP_SEND_FIRMWARE) begin //SEND FIRMWARE
                  // FIX: firm_wr==0 のときのみ firm_data を更新し rd_ack を返す。
                  // firm_wr==1 のときは rd_ack を返さない。spi_slave の
                  // rd_data_available が 1 のまま残るので、次の SPI トランザクションで
                  // ホストがステータスビット(bit6)=0 を受け取り自動的にリトライする。
                  // これにより WRITE_MEMORY がまだ処理中のときにパケットが
                  // サイレントに失われてカウンタがずれる問題を防ぐ。
                  if(firm_wr == 0) begin
                     firm_data <= spi_module_rd_data[23:8];
                     firm_wr <= 1;
                     rd_ack <= 1;
                  end
                  // firm_wr==1 の場合: rd_ack を返さない。ホストが自動リトライ。
               end else if(spi_module_rd_data[7:0] == OP_START_CPU) begin //START CPU
                  if(firm_wr == 0) begin
                     cpu_start <= 1;
                     rd_ack <= 1;
                  end
               end else begin //opcode >= 0x4: CPU コマンド
                  read_data_register <= spi_module_rd_data;
                  rd_ack <= 1;
                  status_register[0] <= 1;
               end
            end
         end

         if(cpu_start_ack == 1 && cpu_start == 1) begin
            cpu_start <= 0;
         end

         if(cpu_init_ack == 1 && cpu_init == 1) begin
            cpu_init <= 0;
         end

         if(firm_ack == 1 && firm_wr == 1) begin
            firm_wr <= 0;
         end

         if(rd_req == 1) begin
            if(rd_addr == 32'h0) begin
               rd_data <= status_register;
               data_valid <= 1;
            end else if(rd_addr == 32'h4) begin
               rd_data <= read_data_register;
               data_valid <= 1;
            end else if(rd_addr == 32'h8) begin
               rd_data <= assert_read_register;
               data_valid <= 1;
            end else if(rd_addr == 32'hC) begin
               rd_data <= write_data_register;
               data_valid <= 1;
            end
         end

         if(wr_req == 1) begin
            if(wr_addr == 32'h8) begin //assert read: CPU がデータを読んだ
               status_register[0] <= 0;
            end else if(wr_addr == 32'hC) begin //write data: CPU から host へ送信
               write_data_register <= wr_data;
               wr_en <= 1;
               spi_module_wr_data <= wr_data;
            end
         end
      end
   end
endmodule
