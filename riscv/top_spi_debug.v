// SPI debug build — Step 4 of the debug progression.
//
// Identical to top.v except:
//   - LED_B ON while cpu_reset=1 (waiting for SPI START_CPU)
//   - LED_G latches ON if cpu_error_instruction fires after CPU starts
//   - LED_R follows gpio_mm bit0 (firmware controlled, active-low)
//
// Expected behaviour after programming and running host/./host:
//
//   While host has NOT yet sent SPI_START_CPU:
//     LED_B ON  (CPU still in reset)
//
//   After SPI_START_CPU arrives:
//     LED_B OFF
//     LED_R ON  -> firmware running, GPIO write succeeded  ✓
//     LED_G ON  -> cpu_error_instruction fired (bad firmware) ✗
//     All OFF   -> CPU not executing (check spi_slave / spi_mm timing) ✗
//
// LED_B stays OFF forever once cpu_reset goes low, so you can distinguish:
//   "SPI never sent START_CPU" (Blue stays on)   from
//   "CPU started but firmware broken" (Blue off, Green on).
//
// Build and program:
//   make spi_debug
//   make prog_spi_debug
// Then on the host:
//   cd host_server && ./host

`include "gpio_mm.v"
`include "memory.v"
`include "spi_mm.v"
`include "simple_cpu.v"

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

   reg spi_reset;
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

   // Debug latches
   reg cpu_ever_error;
   reg firmware_loaded;   // latches after the first 32-bit word is written to SPRAM

   wire gpio_led_r, gpio_led_g, gpio_led_b;

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
      .LED_R(gpio_led_r), .LED_G(gpio_led_g), .LED_B(gpio_led_b),
      .rd_req(gpio_rd_req), .rd_addr(gpio_rd_addr), .rd_data(gpio_rd_data), .data_valid(gpio_rd_valid),
      .wr_req(gpio_wr_req), .wr_addr(gpio_wr_addr), .wr_data(gpio_wr_data)
   );

   memory memory_inst(.clk(clk), .reset(memory_reset),
      .rd_req(memory_rd_req), .rd_addr(memory_rd_addr[14:0]), .rd_data(memory_rd_data), .data_valid(memory_rd_valid),
      .wr_req(memory_wr_req), .wr_addr(memory_wr_addr[14:0]), .wr_data(memory_wr_data)
   );

   // LED (active-low: signal 0 = ON, signal 1 = OFF):
   //   Blue   : ON while cpu_reset=1 (no START_CPU yet)
   //   Red    : ON once firmware has been written to SPRAM (firmware_loaded=1)
   //   Green  : ON when CPU is running with firmware loaded (cpu_reset=0 && firmware_loaded=1)
   //
   // Sequence: Blue → Purple(R+B) → Yellow(R+G)
   //   Blue         : bitstream written, waiting for firmware
   //   Purple (R+B) : firmware loaded, CPU not yet started
   //   Yellow (R+G) : CPU started and running
   assign LED_B = cpu_reset ? 1'b0 : 1'b1;
   assign LED_R = firmware_loaded ? 1'b0 : 1'b1;
   assign LED_G = (!cpu_reset && firmware_loaded) ? 1'b0 : 1'b1;

   reg [31:0] state;
   parameter IDLE=0, REQ_READ_SPI_STATUS=2, WRITE_MEMORY=6, START_CPU=9, INIT_CPU=10;

   reg [15:0] counter_firmware_address;
   reg [15:0] firmware_data_buf;
   reg cpu_read_req_buf;

   initial begin
      cpu_reset = 1;
      spi_reset = 0;
      spi_wr_req = 0; spi_wr_data = 0;
      spi_firm_ack = 0; spi_cpu_start_ack = 0; spi_cpu_init_ack = 0;
      state = REQ_READ_SPI_STATUS;
      memory_reset = 0;
      memory_rd_req = 0; memory_rd_addr = 0;
      memory_wr_req = 0; memory_wr_addr = 0; memory_wr_data = 0;
      gpio_reset = 0;
      counter_firmware_address = 0;
      firmware_data_buf = 0;
      cpu_ever_error = 0;
      firmware_loaded = 0;
   end

   always @(posedge clk)
   begin
      spi_firm_ack <= 0;
      gpio_wr_req <= 0; gpio_wr_addr <= 0; gpio_wr_data <= 0;
      cpu_read_data <= 0; cpu_read_data_valid <= 0;
      cpu_read_req_buf <= cpu_read_req;
      spi_rd_req <= 0; spi_rd_addr <= 0;
      spi_cpu_start_ack <= 0; spi_cpu_init_ack <= 0;
      spi_wr_req <= 0; spi_wr_addr <= 0; spi_wr_data <= 0;
      memory_rd_req <= 0;
      memory_wr_req <= 0; memory_wr_addr <= 0; memory_wr_data <= 0;

      // Error latch (only meaningful after cpu_reset goes low)
      if(cpu_error_instruction == 1) cpu_ever_error <= 1;

      // SPI state machine (identical to top.v)
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
         if(counter_firmware_address[0] == 0) begin
            firmware_data_buf <= spi_firm_data[15:0];
         end else begin
            memory_wr_data <= {spi_firm_data[15:0], firmware_data_buf};
            memory_wr_req <= 1;
            memory_wr_addr <= {counter_firmware_address[13:1], 2'b00};
            firmware_loaded <= 1;   // at least one 32-bit word has been written
         end
         counter_firmware_address <= counter_firmware_address + 1;
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
         firmware_loaded <= 0;   // reset firmware indicator on re-init
         state <= REQ_READ_SPI_STATUS;
      end
      endcase

      if(cpu_read_req_buf == 0 && cpu_read_req == 1) begin
         if(cpu_read_addr[31:15] == 17'h0) begin
            memory_rd_req <= 1;
            memory_rd_addr <= cpu_read_addr[14:0];
         end
         if(cpu_read_addr[31:8] == 24'h000080) begin
            spi_rd_req <= 1;
            spi_rd_addr <= cpu_read_addr[7:0];
         end
         if(cpu_read_addr[31:8] == 24'h000081) begin
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

      if(cpu_write_req == 1) begin
         if(cpu_write_addr[31:15] == 17'h0) begin
            memory_wr_req <= 1;
            memory_wr_addr <= cpu_write_addr[14:0];
            memory_wr_data <= cpu_write_data;
         end
         if(cpu_write_addr[31:8] == 24'h000080) begin
            spi_wr_req <= 1;
            spi_wr_addr <= cpu_write_addr[7:0];
            spi_wr_data <= cpu_write_data;
         end
         if(cpu_write_addr[31:8] == 24'h000081) begin
            gpio_wr_req <= 1;
            gpio_wr_addr <= cpu_write_addr[7:0];
            gpio_wr_data <= cpu_write_data;
         end
      end
   end

endmodule
