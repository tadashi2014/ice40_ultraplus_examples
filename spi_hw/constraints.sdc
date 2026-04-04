# iCE40UP5K-SG48 / iCE40 UltraPlus Breakout Board
# 12 MHz clock on port "clk" (see common/io.pcf)

create_clock -name clk -period 83.333 [get_ports {clk}]

