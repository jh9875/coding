`timescale 10ns / 1ps
module counter_up_dn_7_tb;
// input
reg up, rst_n, clk;
//output
wire [2:0] q;


counter_up_dn_7 U1(
 up, rst_n, clk,
 q
);

initial begin
 rst_n = 0; clk = 0;
 #10 rst_n = 1;
 #15 up = 1;
end
always begin
 clk = 0;
 #1;
 clk = 1;
 #1;
end
endmodule