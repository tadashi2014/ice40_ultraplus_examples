// Blink test: no CPU, no SPI, no firmware required.
// Use this first to verify that the FPGA bitstream is being programmed
// correctly and that the clock/LED pins are working.
//
// At 12 MHz with a 26-bit counter:
//   LED_R blinks every ~0.35 s  (counter[22])
//   LED_G blinks every ~0.70 s  (counter[23])
//   LED_B blinks every ~1.40 s  (counter[24])
//
// LEDs are active-low: signal=0 -> LED ON, signal=1 -> LED OFF.
// All three LEDs blink independently, confirming clock is alive.
//
// Build and program:
//   make blink
//   make prog_blink

module top(input [3:0] SW, input clk,
           output LED_R, output LED_G, output LED_B,
           input SPI_SCK, input SPI_SS, input SPI_MOSI, output SPI_MISO);

   reg [25:0] counter;

   assign LED_R  = ~counter[22];
   assign LED_G  = ~counter[23];
   assign LED_B  = ~counter[24];
   assign SPI_MISO = 1'b1;  // tri-state / idle

   initial counter = 26'b0;

   always @(posedge clk)
      counter <= counter + 1;

endmodule
