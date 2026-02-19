// tomasulo_tb.v - ??? 
`timescale 1ns/1ps 
 
module tomasulo_tb; 
    reg clk; 
    reg reset; 
    wire [31:0] cycle_count; 
     
    // ??????? 
    tomasulo_top dut ( 
        .clk(clk), 
        .reset(reset), 
        .cycle_count(cycle_count) 
    ); 
     
    // ?????? 
    always begin 
        #5 clk = ~clk;  // ?????10ns 
    end 
     
    // ????? 
    initial begin 
        // ????? 
        clk = 0; 
        reset = 1; 
         
        // ???? 
        #20 reset = 0; 
         
        // ?????????????? 
        #1000; 
         
        // ???? 
        $display("Simulation finished after %d cycles", cycle_count); 
        $finish; 
    end 
     
endmodule 
