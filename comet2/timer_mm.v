module timer_mm(
      input wire clk,
      input wire reset,
      input wire rd_req,
      input wire [31:0] rd_addr,
      output reg [31:0] rd_data,
      output reg data_valid,
      input wire wr_req,
      input wire [31:0] wr_addr,
      input wire [31:0] wr_data
   );

   reg [31:0] counter_reg;
   reg [31:0] compare_reg;
   reg enable_reg;
   reg periodic_reg;
   reg fired_reg;

   initial begin
      counter_reg = 32'h00000000;
      compare_reg = 32'h00000000;
      enable_reg = 1'b0;
      periodic_reg = 1'b0;
      fired_reg = 1'b0;
      rd_data = 32'h00000000;
      data_valid = 1'b0;
   end

   always @(posedge clk) begin
      if (reset) begin
         counter_reg <= 32'h00000000;
         compare_reg <= 32'h00000000;
         enable_reg <= 1'b0;
         periodic_reg <= 1'b0;
         fired_reg <= 1'b0;
         rd_data <= 32'h00000000;
         data_valid <= 1'b0;
      end else begin
         data_valid <= 1'b0;

         if (enable_reg) begin
            if (counter_reg >= compare_reg) begin
               fired_reg <= 1'b1;
               if (periodic_reg) begin
                  counter_reg <= 32'h00000000;
               end else begin
                  enable_reg <= 1'b0;
               end
            end else begin
               counter_reg <= counter_reg + 1'b1;
            end
         end

         if (rd_req) begin
            data_valid <= 1'b1;
            case (rd_addr)
            32'h0:  rd_data <= {29'b0, fired_reg, periodic_reg, enable_reg};
            32'h4:  rd_data <= counter_reg;
            32'h8:  rd_data <= compare_reg;
            32'hC:  rd_data <= 32'h00000000;
            32'h10: rd_data <= 32'h00000000;
            32'h14: rd_data <= {30'b0, periodic_reg, enable_reg};
            default: rd_data <= 32'h00000000;
            endcase
         end

         if (wr_req) begin
            case (wr_addr)
            32'h4: begin
               counter_reg <= wr_data;
            end
            32'hC: begin
               counter_reg <= {wr_data[31:16], counter_reg[15:0]};
            end
            32'h8: begin
               compare_reg <= wr_data;
            end
            32'h10: begin
               compare_reg <= {wr_data[31:16], compare_reg[15:0]};
            end
            32'h14: begin
               enable_reg <= wr_data[0];
               periodic_reg <= wr_data[1];
               if (wr_data[2]) begin
                  fired_reg <= 1'b0;
               end
               if (wr_data[3]) begin
                  counter_reg <= 32'h00000000;
               end
            end
            default: begin
            end
            endcase
         end
      end
   end
endmodule
