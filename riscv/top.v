// `include "../spi/spi_slave.v"
`include "gpio_mm.v"
`include "memory.v"
`include "spi_mm.v"
`ifdef CPU_SIMPLE
`include "simple_riscv_cpu/simple_cpu/simple_cpu.v"
`else
`include "picorv32/picorv32_simple_cpu.v"
`endif

module top(input [3:0] SW, input clk, output LED_R, output LED_G, output LED_B, input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO);

   (* syn_maxfan = 40 *) reg cpu_reset = 1;
   wire cpu_read_req;
   wire [31:0] cpu_read_addr;
   reg [31:0] cpu_read_data;
   reg cpu_read_data_valid;
   wire cpu_write_req;
   wire [31:0] cpu_write_addr;
   wire [31:0] cpu_write_data;
   wire [3:0] cpu_memory_mask;
   wire cpu_error_instruction;
   wire [31:0] cpu_debug;

   //signals for the spi mm
   reg spi_reset = 1;
   reg spi_rd_req;
   reg [31:0] spi_rd_addr;
   wire [31:0] spi_rd_data;
   wire spi_rd_valid;
   reg spi_wr_req;
   reg [31:0] spi_wr_addr;
   reg [31:0] spi_wr_data;
   wire spi_firm_wr;
   wire [15:0] spi_firm_data;
   reg spi_firm_ack;
   wire spi_cpu_start;
   reg spi_cpu_start_ack;
   wire spi_cpu_init;
   reg spi_cpu_init_ack;

   //signals for the gpio_mm
   wire gpio_reset;
   assign gpio_reset = 0;
   reg gpio_rd_req;
   reg [31:0] gpio_rd_addr;
   wire [31:0] gpio_rd_data;
   wire gpio_rd_valid;
   reg gpio_wr_req;
   reg [31:0] gpio_wr_addr;
   reg [31:0] gpio_wr_data;

   wire memory_reset;
   assign memory_reset = 0;
   reg memory_rd_req;
   reg [31:0] memory_rd_addr;
   wire [31:0] memory_rd_data;
   wire memory_rd_valid;
   reg memory_wr_req;
   reg [31:0] memory_wr_addr;
   reg [31:0] memory_wr_data;
   reg [3:0] memory_wr_mask;

   spi_mm spi_mm_inst(.clk(clk), .reset(spi_reset),
      .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
      .rd_req(spi_rd_req), .rd_addr(spi_rd_addr), .rd_data(spi_rd_data), .data_valid(spi_rd_valid),
      .wr_req(spi_wr_req), .wr_addr(spi_wr_addr), .wr_data(spi_wr_data),
      .firm_wr(spi_firm_wr), .firm_data(spi_firm_data), .firm_ack(spi_firm_ack),
      .cpu_start(spi_cpu_start), .cpu_start_ack(spi_cpu_start_ack),
      .cpu_init(spi_cpu_init), .cpu_init_ack(spi_cpu_init_ack)
   );

   simple_cpu simple_cpu_inst(.clk(clk), .reset(cpu_reset),
      .read_req(cpu_read_req), .read_addr(cpu_read_addr), .read_data(cpu_read_data), .read_data_valid(cpu_read_data_valid),
      .write_req(cpu_write_req), .write_addr(cpu_write_addr), .write_data(cpu_write_data), .memory_mask(cpu_memory_mask),
      .error_instruction(cpu_error_instruction), .debug(cpu_debug)
   );

   gpio_mm gpio_mm_inst(.clk(clk), .reset(gpio_reset),
      .LED_R(LED_R), .LED_G(LED_G), .LED_B(LED_B),
      .rd_req(gpio_rd_req), .rd_addr(gpio_rd_addr), .rd_data(gpio_rd_data), .data_valid(gpio_rd_valid),
      .wr_req(gpio_wr_req), .wr_addr(gpio_wr_addr), .wr_data(gpio_wr_data)
   );

   memory memory_inst(.clk(clk), .reset(memory_reset),
      .rd_req(memory_rd_req), .rd_addr(memory_rd_addr[14:0]), .rd_data(memory_rd_data), .data_valid(memory_rd_valid),
      .wr_req(memory_wr_req), .wr_addr(memory_wr_addr[14:0]), .wr_data(memory_wr_data), .wr_mask(memory_wr_mask)
   );

   //register file investigation
   reg [31:0] state;

   parameter IDLE=0, INIT=IDLE+1, REQ_READ_SPI_STATUS=INIT+1, READ_SPI_STATUS=REQ_READ_SPI_STATUS+1,
            REQ_SPI_READ_DATA=READ_SPI_STATUS+1, SPI_READ_DATA=REQ_SPI_READ_DATA+1,
            WRITE_MEMORY=SPI_READ_DATA+1, READ_REQ_MEMORY=WRITE_MEMORY+1, READ_MEMORY=READ_REQ_MEMORY+1,
            START_CPU=READ_MEMORY+1, INIT_CPU=START_CPU+1;

   reg [31:0] spi_recv_data_reg;
   reg handle_data;

   reg [15:0] counter_firmware_address; //address to write firmware to
   reg [15:0] firmware_data_buf; //since we receive 16bits data and want to write 32, keep buffer

   reg [23:0] reg_bits_inversion;

   reg cpu_read_req_buf; //to detect rising edge

   initial begin

      cpu_reset = 1; //cpu in reset at start

      spi_wr_req = 0;
      spi_wr_data = 0;

      spi_recv_data_reg = 0;
      spi_firm_ack = 0;
      spi_cpu_start_ack = 0;
      spi_cpu_init_ack = 0;
      handle_data = 0;

      state = REQ_READ_SPI_STATUS;

      memory_rd_req = 0;
      memory_rd_addr = 0;
      memory_wr_req = 0;
      memory_wr_addr = 0;
      memory_wr_data = 0;
      memory_wr_mask = 4'b1111;
      gpio_rd_req = 0;
      gpio_rd_addr = 0;

      counter_firmware_address = 0;
      firmware_data_buf = 0;
      cpu_read_req_buf = 0;
   end

   always @(posedge clk)
   begin
      spi_reset <= 0;

      //defaults
      spi_firm_ack <= 0;

      gpio_wr_req <= 0;
      gpio_wr_addr <= 0;
      gpio_wr_data <= 0;
      gpio_rd_req <= 0;
      gpio_rd_addr <= 0;

      cpu_read_data <= 0;
      cpu_read_data_valid <= 0;
      cpu_read_req_buf <= cpu_read_req;

      spi_rd_req <= 0;
      spi_rd_addr <= 0;
      spi_cpu_start_ack <= 0;
      spi_cpu_init_ack <= 0;

      spi_wr_req <= 0;
      spi_wr_addr <= 0;
      spi_wr_data <= 0;

      memory_rd_req <= 0;
      memory_wr_req <= 0;
      memory_wr_addr <= 0;
      memory_wr_data <= 0;
      memory_wr_mask <= 4'b1111;

      //handling of SPI messages from host
      case (state)
      IDLE: begin
      end
      INIT: begin
      end
      REQ_READ_SPI_STATUS: begin
         if(spi_cpu_init == 1) begin
            spi_cpu_init_ack <= 1;
            state <= INIT_CPU;
         end else if(spi_firm_wr == 1) begin
            spi_firm_ack <= 1;
            state <= WRITE_MEMORY;
         end else if (spi_cpu_start == 1) begin
            spi_cpu_start_ack <= 1;
            state <= START_CPU;
         end
      end
      WRITE_MEMORY: begin
         if(counter_firmware_address[0] == 0) begin
            firmware_data_buf <= spi_firm_data[15:0];
         end else begin
            memory_wr_data <= {spi_firm_data[15:0], firmware_data_buf};
            memory_wr_mask <= 4'b1111;
            memory_wr_req <= 1;
            memory_wr_addr <= {counter_firmware_address[13:1], 2'b00};
         end

         counter_firmware_address <= counter_firmware_address + 1;
         // Release spi_mm.firm_wr after every accepted 16-bit firmware word.
         // Without this ack, only the first word is ever consumed and the host
         // keeps retrying because spi_mm still reports the firmware FIFO busy.
         spi_firm_ack <= 1;

         state <= REQ_READ_SPI_STATUS;
      end
      START_CPU: begin
         cpu_reset <= 0;
         state <= REQ_READ_SPI_STATUS;
      end
      INIT_CPU: begin
         cpu_reset <= 1;
         counter_firmware_address <= 0;
         firmware_data_buf <= 0;
         // FIX: 残留 firm_wr を確実にクリアする。
         // 電源投入直後やノイズで firm_wr が 1 になっていた場合、
         // INIT 後の最初の WRITE_MEMORY が偽パケットを処理し
         // counter が 1 にずれてしまう。spi_firm_ack を出して
         // firm_wr を確実に 0 に戻す。
         spi_firm_ack <= 1;
         state <= REQ_READ_SPI_STATUS;
      end

      endcase

      // cpu makes a read request
      if(cpu_read_req_buf == 0 && cpu_read_req == 1) begin //only rising edge

         //memory
         if(cpu_read_addr[31:15] == 17'h0 ) begin //0x0000 - 0x7fff
            memory_rd_req <= 1;
            memory_rd_addr <= cpu_read_addr[14:0];
         end
         //SPI
         if(cpu_read_addr[31:8] == 24'h000080 ) begin //0x8000 - 0x80ff
            spi_rd_req <= 1;
            spi_rd_addr <= cpu_read_addr[7:0];
         end
         //gpio
         if(cpu_read_addr[31:8] == 24'h000081) begin //0x8100 - 0x81ff
            gpio_rd_req <= 1;
            gpio_rd_addr <= cpu_read_addr[7:0];
         end

      end

      if(memory_rd_valid == 1) begin
         cpu_read_data <= memory_rd_data;
         cpu_read_data_valid <= 1;
      end

      if(spi_rd_valid == 1) begin
         cpu_read_data <= spi_rd_data;
         cpu_read_data_valid <= 1;
      end

      if(gpio_rd_valid == 1) begin
         cpu_read_data <= gpio_rd_data;
         cpu_read_data_valid <= 1;
      end

      //cpu makes a write request
      if(cpu_write_req == 1) begin
         //memory
         if(cpu_write_addr[31:15] == 17'h0 ) begin
            memory_wr_req <= 1;
            memory_wr_addr <= cpu_write_addr[14:0];
            memory_wr_data <= cpu_write_data;
            memory_wr_mask <= cpu_memory_mask;
         end
         //SPI
         if(cpu_write_addr[31:8] == 24'h000080 ) begin
            spi_wr_req <= 1;
            spi_wr_addr <= cpu_write_addr[7:0];
            spi_wr_data <= cpu_write_data;
         end
         //gpio
         if(cpu_write_addr[31:8] == 24'h000081) begin
            gpio_wr_req <= 1;
            gpio_wr_addr <= cpu_write_addr[7:0];
            gpio_wr_data <= cpu_write_data;
         end

      end
   end

endmodule
