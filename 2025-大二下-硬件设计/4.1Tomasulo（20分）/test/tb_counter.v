// tb/tb_counter.v
`timescale 1ns/1ps

module tb_counter();

    // ????
    parameter WIDTH = 8;
    parameter CLK_PERIOD = 10; // 10ns????
    
    // ????
    reg clk;
    reg rst_n;
    wire [WIDTH-1:0] count;
    
    // ???????
    counter #(
        .WIDTH(WIDTH)
    ) uut (
        .clk(clk),
        .rst_n(rst_n),
        .count(count)
    );
    
    // ????
    initial begin
        clk = 0;
        forever #(CLK_PERIOD/2) clk = ~clk;
    end
    
    // ????
    initial begin
        // ?????
        rst_n = 0;
        #20;
        
        // ????
        rst_n = 1;
        #200;
        
        // ????
        $finish;
    end
    
    // ????
    integer cycle_count = 0;
    always @(posedge clk) begin
        if (rst_n) begin
            cycle_count <= cycle_count + 1;
            if (count !== cycle_count) begin
                $display("Error: count = %d, expected = %d at cycle %0t", 
                         count, cycle_count, $time);
                $finish;
            end
        end
    end
    
    // ????
    initial begin
        $dumpfile("waveforms.vcd");
        $dumpvars(0, tb_counter);
    end

endmodule