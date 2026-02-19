module testbench;

reg clk;
reg reset;
wire [31:0] pc;
wire [31:0] instruction;
wire branch_taken;
wire prediction_correct;
wire is_branch_instr;
wire btb_hit;
wire prediction_taken;

// ??????
btb_processor cpu(
    .clk(clk),
    .reset(reset),
    .pc(pc),
    .instruction(instruction),
    .branch_taken(branch_taken),
    .prediction_correct(prediction_correct),
    .is_branch_instr(is_branch_instr),
    .btb_hit(btb_hit),
    .prediction_taken(prediction_taken)
);

// ????
initial begin
    clk = 0;
    forever #5 clk = ~clk; // 10ns??
end

// ????
initial begin
    // ???
    reset = 1;
    #20 reset = 0;
    
    // ????
    #2000;
    
    $display("????");
    $stop;
end

// ????
always @(posedge clk) begin
    if(!reset) begin
        $display("??=%0t PC=%h ??=%h ???=%b ????=%b ????=%b BTB??=%b ????=%b",
                 $time, pc, instruction, is_branch_instr, branch_taken, prediction_taken, btb_hit, prediction_correct);
    end
end

endmodule
