// Timer debug version:
// - Automatically releases cpu_reset after ~2 seconds (no SPI needed)
// - LED_B: ON while cpu_reset=1 (waiting), OFF when cpu_reset=0
// - LED_R: follows gpio_mm bit0 (set by firmware sw to 0x8100)
// - LED_G: latches ON when timer fires (cpu_reset released); stays ON permanently
//          SPRAM has undefined contents on power-up, so error_instruction is not
//          a reliable trigger — timer_fired gives a deterministic Blue→Green transition.
//
// Expected: Blue (~2s) → Green ON and stays on.
// This confirms the FPGA clock, timer circuit, and LED path all work.

`include "../gpio_mm.v"
`include "../spi_mm.v"
`include "../comet2_cpu/comet2_cpu.v"
`include "../comet2_cpu/comet2_memory.v"

module top(input [3:0] SW, input clk,
           output LED_R, output LED_G, output LED_B,
           input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO);

   reg cpu_reset;
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

   reg gpio_reset;
   reg gpio_rd_req;
   reg [31:0] gpio_rd_addr;
   wire [31:0] gpio_rd_data;
   wire gpio_rd_valid;
   reg gpio_wr_req;
   reg [31:0] gpio_wr_addr;
   reg [31:0] gpio_wr_data;

   reg memory_reset;
   reg memory_rd_req;
   reg [31:0] memory_rd_addr;
   wire [31:0] memory_rd_data;
   wire memory_rd_valid;
   reg memory_wr_req;
   reg [31:0] memory_wr_addr;
   reg [31:0] memory_wr_data;
   reg [3:0] memory_wr_mask;
   reg [15:0] comet_spi_write_lo;
   reg comet_spi_read_hi;

   // Timer: 12MHz clock, ~2 seconds = 24_000_000 counts
   // Use 25-bit counter (max ~2.8s)
   reg [24:0] timer;
   reg timer_fired;

   // Debug latches
   reg cpu_ever_error;
   reg cpu_ever_started;

   wire gpio_led_r, gpio_led_g, gpio_led_b;

   spi_mm spi_mm_inst(.clk(clk), .reset(spi_reset),
      .SPI_SCK(SPI_SCK), .SPI_SS(SPI_SS), .SPI_MOSI(SPI_MOSI), .SPI_MISO(SPI_MISO),
      .rd_req(spi_rd_req), .rd_addr(spi_rd_addr), .rd_data(spi_rd_data), .data_valid(spi_rd_valid),
      .wr_req(spi_wr_req), .wr_addr(spi_wr_addr), .wr_data(spi_wr_data),
      .firm_wr(spi_firm_wr), .firm_data(spi_firm_data), .firm_ack(spi_firm_ack),
      .cpu_start(spi_cpu_start), .cpu_start_ack(spi_cpu_start_ack),
      .cpu_init(spi_cpu_init), .cpu_init_ack(spi_cpu_init_ack)
   );

   comet2_cpu comet2_cpu_inst(.clk(clk), .reset(cpu_reset),
      .read_req(cpu_read_req), .read_addr(cpu_read_addr), .read_data(cpu_read_data), .read_data_valid(cpu_read_data_valid),
      .write_req(cpu_write_req), .write_addr(cpu_write_addr), .write_data(cpu_write_data), .memory_mask(cpu_memory_mask),
      .error_instruction(cpu_error_instruction), .debug(cpu_debug)
   );

   gpio_mm gpio_mm_inst(.clk(clk), .reset(gpio_reset),
      .LED_R(gpio_led_r), .LED_G(gpio_led_g), .LED_B(gpio_led_b),
      .rd_req(gpio_rd_req), .rd_addr(gpio_rd_addr), .rd_data(gpio_rd_data), .data_valid(gpio_rd_valid),
      .wr_req(gpio_wr_req), .wr_addr(gpio_wr_addr), .wr_data(gpio_wr_data)
   );

   wire [15:0] comet_memory_rd_data;
   wire comet_memory_rd_valid;
   comet2_memory comet2_memory_inst(.clk(clk), .reset(memory_reset),
      .rd_req(memory_rd_req), .rd_addr(memory_rd_addr[15:0]), .rd_data(comet_memory_rd_data), .data_valid(comet_memory_rd_valid),
      .wr_req(memory_wr_req), .wr_addr(memory_wr_addr[15:0]), .wr_data(memory_wr_data[15:0])
   );
   assign memory_rd_data = {16'b0, comet_memory_rd_data};
   assign memory_rd_valid = comet_memory_rd_valid;

   // LED:
   // 青: cpu_reset=1(まだリセット中) -> 点灯, cpu_reset=0 -> 消灯
   // 赤: gpio_mm の LED_R (firmware が制御)
   // 緑: タイマー発火後(cpu_reset解除後)に点灯、そのまま維持
   //     (SPRAM は不定値のため error_instruction 頼みでは不確実)
   assign LED_B = cpu_reset ? 1'b0 : 1'b1;   // リセット中=青点灯
   assign LED_R = gpio_led_r;                  // firmware 制御
   assign LED_G = timer_fired ? 1'b0 : gpio_led_g; // タイマー発火で緑点灯、そのまま

   reg [31:0] state;
   parameter IDLE=0, REQ_READ_SPI_STATUS=2, WRITE_MEMORY=6, START_CPU=9, INIT_CPU=10;

   reg [15:0] counter_firmware_address;
   reg [15:0] firmware_data_buf;
   reg cpu_read_req_buf;

   initial begin
      cpu_reset = 1;
      spi_reset = 1;
      spi_wr_req = 0; spi_wr_data = 0;
      spi_firm_ack = 0; spi_cpu_start_ack = 0; spi_cpu_init_ack = 0;
      state = REQ_READ_SPI_STATUS;
      memory_reset = 0;
      memory_rd_req = 0; memory_rd_addr = 0;
      memory_wr_req = 0; memory_wr_addr = 0; memory_wr_data = 0;
      memory_wr_mask = 4'b1111;
      gpio_reset = 0;
      gpio_rd_req = 0; gpio_rd_addr = 0;
      counter_firmware_address = 0;
      firmware_data_buf = 0;
      comet_spi_write_lo = 0;
      comet_spi_read_hi = 0;
      timer = 0;
      timer_fired = 0;
      cpu_ever_error = 0;
      cpu_ever_started = 0;
      cpu_read_req_buf = 0;
   end

   always @(posedge clk)
   begin
      spi_reset <= 0;
      spi_firm_ack <= 0;
      gpio_rd_req <= 0; gpio_rd_addr <= 0;
      gpio_wr_req <= 0; gpio_wr_addr <= 0; gpio_wr_data <= 0;
      cpu_read_data <= 0; cpu_read_data_valid <= 0;
      cpu_read_req_buf <= cpu_read_req;
      spi_rd_req <= 0; spi_rd_addr <= 0;
      spi_cpu_start_ack <= 0; spi_cpu_init_ack <= 0;
      spi_wr_req <= 0; spi_wr_addr <= 0; spi_wr_data <= 0;
      memory_rd_req <= 0;
      memory_wr_req <= 0; memory_wr_addr <= 0; memory_wr_data <= 0; memory_wr_mask <= 4'b1111;

      // Error latch
      if(cpu_error_instruction == 1) cpu_ever_error <= 1;

      // ========== TIMER: auto-release cpu_reset after ~2s ==========
      if(timer_fired == 0) begin
         timer <= timer + 1;
         if(timer == 25'h17FFFFF) begin  // ~2s at 12MHz
            timer_fired <= 1;
            cpu_reset <= 0;              // FORCE release reset
         end
      end
      // =============================================================

      // SPI state machine (still runs to handle firmware loading)
      case (state)
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
         memory_wr_data <= {16'b0, spi_firm_data[15:0]};
         memory_wr_req <= 1;
         memory_wr_addr <= {16'b0, counter_firmware_address};
         counter_firmware_address <= counter_firmware_address + 1;
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
         spi_firm_ack <= 1;
         state <= REQ_READ_SPI_STATUS;
      end
      endcase

      if(cpu_read_req_buf == 0 && cpu_read_req == 1) begin
         if(cpu_read_addr[15:12] != 4'hF) begin
            memory_rd_req <= 1;
            memory_rd_addr <= cpu_read_addr;
         end
         if(cpu_read_addr[15:8] == 8'hF0) begin
            spi_rd_req <= 1;
            if(cpu_read_addr[3:0] == 4'h0) begin
               spi_rd_addr <= 32'h0;
               comet_spi_read_hi <= 1'b0;
            end else if(cpu_read_addr[3:0] == 4'h1 || cpu_read_addr[3:0] == 4'h2) begin
               spi_rd_addr <= 32'h4;
               comet_spi_read_hi <= cpu_read_addr[3:0] == 4'h2;
            end else if(cpu_read_addr[3:0] == 4'h3) begin
               spi_rd_addr <= 32'h8;
               comet_spi_read_hi <= 1'b0;
            end else begin
               spi_rd_addr <= 32'hC;
               comet_spi_read_hi <= 1'b0;
            end
         end
         if(cpu_read_addr[15:8] == 8'hF1) begin
            gpio_rd_req <= 1;
            gpio_rd_addr <= 0;
         end
      end

      if(memory_rd_valid == 1) begin
         cpu_read_data <= memory_rd_data;
         cpu_read_data_valid <= 1;
      end
      if(spi_rd_valid == 1) begin
         cpu_read_data <= comet_spi_read_hi ? {16'b0, spi_rd_data[31:16]} : {16'b0, spi_rd_data[15:0]};
         cpu_read_data_valid <= 1;
      end
      if(gpio_rd_valid == 1) begin
         cpu_read_data <= {16'b0, gpio_rd_data[15:0]};
         cpu_read_data_valid <= 1;
      end

      if(cpu_write_req == 1) begin
         if(cpu_write_addr[15:12] != 4'hF) begin
            memory_wr_req <= 1;
            memory_wr_addr <= cpu_write_addr;
            memory_wr_data <= {16'b0, cpu_write_data[15:0]};
         end
         if(cpu_write_addr[15:8] == 8'hF0) begin
            if(cpu_write_addr[3:0] == 4'h3) begin
               spi_wr_req <= 1;
               spi_wr_addr <= 32'h8;
               spi_wr_data <= {16'b0, cpu_write_data[15:0]};
            end else if(cpu_write_addr[3:0] == 4'h4) begin
               comet_spi_write_lo <= cpu_write_data[15:0];
            end else if(cpu_write_addr[3:0] == 4'h5) begin
               spi_wr_req <= 1;
               spi_wr_addr <= 32'hC;
               spi_wr_data <= {cpu_write_data[15:0], comet_spi_write_lo};
            end
         end
         if(cpu_write_addr[15:8] == 8'hF1) begin
            gpio_wr_req <= 1;
            gpio_wr_addr <= 0;
            gpio_wr_data <= {16'b0, cpu_write_data[15:0]};
         end
      end
   end

endmodule
