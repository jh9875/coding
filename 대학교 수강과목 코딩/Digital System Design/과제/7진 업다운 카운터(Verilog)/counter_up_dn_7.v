 
 module counter_up_dn_7(
	up, rst_n, clk,
	q
 );
 
 input up, rst_n, clk;
 output [2:0] q;

 
 reg [2:0] q;
 
 always @(posedge clk)
 begin
 if(rst_n ==0)
    q = 3'b000;
 else
    if(up ==1)   
        if( q == 3'b110)
            q = 3'b000;
        else
            q = q + 1;
            
    else
           
        if( q == 3'b000)
            q = 3'b110;
        else
            q = q - 1;
        
  
 end

 
 endmodule