module btb_processor(
    input clk,
    input reset,
    output reg [31:0] pc,
    output reg [31:0] instruction,
    output reg branch_taken,
    output reg prediction_correct,
    output reg is_branch_instr,
    output reg btb_hit,
    output reg prediction_taken
);

// ??????
integer i;

// ??????
reg [31:0] instruction_memory [0:63];

// BTB? - 8???
reg [31:0] btb_pc [0:7];      // ???????PC
reg [31:0] btb_target [0:7];  // ????????
reg btb_valid [0:7];          // ???

// BHT - ??????8????2??????
reg [1:0] bht [0:7];

// ????
reg [31:0] next_pc;
reg [2:0] btb_index;
reg [2:0] bht_index;
reg predicted_taken;
reg actual_taken;
wire [5:0] opcode;
wire [25:0] jump_target;
wire [15:0] immediate;
wire signed [31:0] branch_offset;

// ????
assign opcode = instruction[31:26];
assign jump_target = instruction[25:0];
assign immediate = instruction[15:0];
assign branch_offset = {{14{immediate[15]}}, immediate, 2'b00};

// ??????? - ???????????
initial begin
    // ?????? - ??0-3
    instruction_memory[0] = 32'h20010000;  // addi $1, $0, 0    # ????? i=0
    instruction_memory[1] = 32'h20020000;  // addi $2, $0, 0    # ????? j=0
    instruction_memory[2] = 32'h20030003;  // addi $3, $0, 3    # ??????=3
    instruction_memory[3] = 32'h20040002;  // addi $4, $0, 2    # ??????=2
    
    // ?????? - ??4-7
    instruction_memory[4] = 32'h20420001;  // addi $2, $2, 1    # j++
    instruction_memory[5] = 32'h0044282A;  // slt $5, $2, $4    # $5 = (j < 2)
    instruction_memory[6] = 32'h14A0FFFD;  // bne $5, $0, -3    # if(j<2) goto 4
    instruction_memory[7] = 32'h00000000;  // nop
    
    // ?????? - ??8-11
    instruction_memory[8] = 32'h20020000;  // addi $2, $0, 0    # j=0 ???????
    instruction_memory[9] = 32'h20210001;  // addi $1, $1, 1    # i++
    instruction_memory[10] = 32'h0023282A; // slt $5, $1, $3    # $5 = (i < 3)
    instruction_memory[11] = 32'h14A0FFF8; // bne $5, $0, -8    # if(i<3) goto 4
    
    // ?????? - ??12-15
    instruction_memory[12] = 32'h20060001; // addi $6, $0, 1    # ?????
    instruction_memory[13] = 32'h00000000; // nop               # ?????
    instruction_memory[14] = 32'h10000001; // beq $0, $0, 1     # ???????
    instruction_memory[15] = 32'h00000000; // nop
    
    // ?????? - ??16-19
    instruction_memory[16] = 32'h20070001; // addi $7, $0, 1    # $7 = 1
    instruction_memory[17] = 32'h14E00001; // bne $7, $0, 1     # ???19(????)
    instruction_memory[18] = 32'h00000000; // nop (???)
    instruction_memory[19] = 32'h08000000; // j 0               # ?????
    
    // ???????nop
    for(i = 20; i < 64; i = i + 1) begin
        instruction_memory[i] = 32'h00000000; // nop
    end
end

// ???BTB?BHT
initial begin
    for(i = 0; i < 8; i = i + 1) begin
        btb_pc[i] = 32'h00000000;
        btb_target[i] = 32'h00000000;
        btb_valid[i] = 1'b0;
        bht[i] = 2'b01; // ??????? (01)
    end
end

// ????
always @(*) begin
    btb_index = pc[4:2]; // ??PC???3?????
    bht_index = pc[4:2];
end

// BTB?????
always @(*) begin
    btb_hit = btb_valid[btb_index] && (btb_pc[btb_index] == pc);
    predicted_taken = bht[bht_index][1]; // ??2??????????
    prediction_taken = predicted_taken;
end

// ???????????
always @(*) begin
    case(opcode)
        6'b000100: begin // beq
            is_branch_instr = 1'b1;
            // ???????? beq $0, $0 ????????????????
            if(instruction[25:21] == 5'b00000 && instruction[20:16] == 5'b00000)
                actual_taken = 1'b1; // beq $0, $0 ????
            else
                actual_taken = 1'b0; // ??beq???????
        end
        6'b000101: begin // bne  
            is_branch_instr = 1'b1;
            // ???????? bne $x, $0 (x!=0) ???
            if(instruction[25:21] != 5'b00000 && instruction[20:16] == 5'b00000)
                actual_taken = 1'b1; // bne $x, $0 ??
            else
                actual_taken = 1'b1; // ???????????????
        end
        6'b000010: begin // j
            is_branch_instr = 1'b1;
            actual_taken = 1'b1; // ?????????
        end
        default: begin
            is_branch_instr = 1'b0;
            actual_taken = 1'b0;
        end
    endcase
    
    branch_taken = actual_taken;
    prediction_correct = (predicted_taken == actual_taken);
end

// ???PC??
always @(*) begin
    if(is_branch_instr && actual_taken) begin
        case(opcode)
            6'b000100, 6'b000101: begin // beq, bne (PC????)
                next_pc = pc + 4 + branch_offset;
            end
            6'b000010: begin // j (????)
                next_pc = {pc[31:28], jump_target, 2'b00};
            end
            default: next_pc = pc + 4;
        endcase
    end else begin
        next_pc = pc + 4; // ????
    end
end

// ??????
always @(posedge clk or posedge reset) begin
    if(reset) begin
        pc <= 32'h00000000;
        instruction <= 32'h00000000;
    end else begin
        // ??????
        instruction <= instruction_memory[pc[7:2]];
        
        // ??PC
        pc <= next_pc;
        
        // ??BTB (?????????)
        if(is_branch_instr) begin
            btb_pc[btb_index] <= pc;
            if(actual_taken) begin
                btb_target[btb_index] <= next_pc;
            end else begin
                btb_target[btb_index] <= pc + 4;
            end
            btb_valid[btb_index] <= 1'b1;
        end
        
        // ??BHT (2??????: 00=????, 01=????, 10=???, 11=???)
        if(is_branch_instr) begin
            if(actual_taken) begin
                // ??????????(?????)
                if(bht[bht_index] < 2'b11)
                    bht[bht_index] <= bht[bht_index] + 1;
            end else begin
                // ???????????(??????)
                if(bht[bht_index] > 2'b00)
                    bht[bht_index] <= bht[bht_index] - 1;
            end
        end
    end
end

endmodule

