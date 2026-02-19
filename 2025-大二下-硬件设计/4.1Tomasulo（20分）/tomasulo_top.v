module tomasulo_top( 
    input clk, 
    input reset, 
    output [31:0] cycle_count 
); 
    // ??????? 
    reg [31:0] register_file[0:31];  // ????? 
    reg [31:0] memory[0:1023];       // ?? 
     
    // ???? 
    reg [31:0] instruction_queue[0:15]; 
    reg [3:0] queue_head, queue_tail; 
    reg [3:0] queue_size; 
     
    // ??? 
    parameter ADD_RS_SIZE = 3; 
    parameter MUL_RS_SIZE = 2; 
    parameter LD_RS_SIZE = 3; 
    parameter ST_RS_SIZE = 3; 
     
    // ????? 
    reg add_rs_busy[0:ADD_RS_SIZE-1]; 
    reg [31:0] add_rs_vj[0:ADD_RS_SIZE-1]; 
    reg [31:0] add_rs_vk[0:ADD_RS_SIZE-1]; 
    reg [4:0] add_rs_qj[0:ADD_RS_SIZE-1]; 
    reg [4:0] add_rs_qk[0:ADD_RS_SIZE-1]; 
    reg [5:0] add_rs_op[0:ADD_RS_SIZE-1]; 
    reg [4:0] add_rs_dest[0:ADD_RS_SIZE-1]; 
     
    // ????? 
    reg mul_rs_busy[0:MUL_RS_SIZE-1]; 
    reg [31:0] mul_rs_vj[0:MUL_RS_SIZE-1]; 
    reg [31:0] mul_rs_vk[0:MUL_RS_SIZE-1]; 
    reg [4:0] mul_rs_qj[0:MUL_RS_SIZE-1]; 
    reg [4:0] mul_rs_qk[0:MUL_RS_SIZE-1]; 
    reg [5:0] mul_rs_op[0:MUL_RS_SIZE-1]; 
    reg [4:0] mul_rs_dest[0:MUL_RS_SIZE-1]; 
     
    // Load??? 
    reg ld_rs_busy[0:LD_RS_SIZE-1]; 
    reg [31:0] ld_rs_addr[0:LD_RS_SIZE-1]; 
    reg [31:0] ld_rs_vj[0:LD_RS_SIZE-1]; 
    reg [4:0] ld_rs_qj[0:LD_RS_SIZE-1]; 
    reg [4:0] ld_rs_dest[0:LD_RS_SIZE-1]; 
     
    // Store??? 
    reg st_rs_busy[0:ST_RS_SIZE-1]; 
    reg [31:0] st_rs_addr[0:ST_RS_SIZE-1]; 
    reg [31:0] st_rs_vj[0:ST_RS_SIZE-1]; 
    reg [31:0] st_rs_vk[0:ST_RS_SIZE-1]; 
    reg [4:0] st_rs_qj[0:ST_RS_SIZE-1]; 
    reg [4:0] st_rs_qk[0:ST_RS_SIZE-1]; 
     
    // ???? 
    reg [3:0] add_timer; 
    reg [3:0] mul_timer; 
    reg [3:0] div_timer; 
    reg [3:0] ld_timer; 
    reg [3:0] st_timer; 
     
    // ???????? 
    reg add_unit_busy; 
    reg mul_unit_busy; 
    reg ld_unit_busy; 
    reg st_unit_busy; 
     
    // ??????????? 
    reg [2:0] add_rs_index; 
    reg [2:0] mul_rs_index; 
    reg [2:0] ld_rs_index; 
    reg [2:0] st_rs_index; 
     
    // ?????? 
    reg cdb_valid; 
    reg [4:0] cdb_tag; 
    reg [31:0] cdb_value; 
     
    // ?????? 
    reg [4:0] reg_status[0:31]; 
     
    // ????? 
    reg [31:0] cycles; 
    assign cycle_count = cycles; 
     
    // ???? 
    parameter OP_ADD = 6'b000000; 
    parameter OP_SUB = 6'b000001; 
    parameter OP_MUL = 6'b000010; 
    parameter OP_DIV = 6'b000011; 
    parameter OP_LD  = 6'b000100; 
    parameter OP_ST  = 6'b000101; 
     
    // ????? 
    parameter TAG_NONE = 5'b00000; 
    parameter TAG_ADD_BASE = 5'b00001;  // 01-03: ADD ??? 
    parameter TAG_MUL_BASE = 5'b00100;  // 04-05: MUL ??? 
    parameter TAG_LD_BASE  = 5'b00110;  // 06-08: LD ??? 
    parameter TAG_ST_BASE  = 5'b01001;  // 09-11: ST ??? 
     
    // ??????? 
    task init_test_program; 
        begin 
            // ??????? 
            // ???????: [31:26] op, [25:21] rs, [20:16] rt, [15:11] rd 
            // ????: 
            // LD R1, 100(R0)     // ????100???R1 
            instruction_queue[0] = {OP_LD, 5'd0, 5'd1, 16'd100}; 
             
            // LD R2, 104(R0)     // ????104???R2 
            instruction_queue[1] = {OP_LD, 5'd0, 5'd2, 16'd104}; 
             
            // ADD R3, R1, R2     // R3 = R1 + R2 (RAW??) 
            instruction_queue[2] = {OP_ADD, 5'd1, 5'd2, 5'd3, 11'd0}; 
             
            // MUL R4, R3, R1     // R4 = R3 * R1 (RAW??) 
            instruction_queue[3] = {OP_MUL, 5'd3, 5'd1, 5'd4, 11'd0}; 
             
            // SUB R5, R3, R4     // R5 = R3 - R4 (RAW??) 
            instruction_queue[4] = {OP_SUB, 5'd3, 5'd4, 5'd5, 11'd0}; 
             
            // ST R5, 108(R0)     // ??R5?????108 
            instruction_queue[5] = {OP_ST, 5'd0, 5'd5, 16'd108}; 
             
            // DIV R6, R5, R3     // R6 = R5 / R3 
            instruction_queue[6] = {OP_DIV, 5'd5, 5'd3, 5'd6, 11'd0}; 
             
            // ??????? 
            queue_head = 0; 
            queue_tail = 7;  // 7??? 
            queue_size = 7; 
             
            // ????? 
            memory[100] = 10;  // ????100???10 
            memory[104] = 20;  // ????104???20 
        end 
    endtask 
     
    // ????? 
    task init_system; 
        integer i; 
        begin 
            // ?????? 
            for (i = 0; i < 32; i = i + 1) begin 
                register_file[i] = 0; 
                reg_status[i] = TAG_NONE; 
            end 
             
            // ???????? 
            for (i = 0; i < ADD_RS_SIZE; i = i + 1) begin 
                add_rs_busy[i] = 0; 
                add_rs_vj[i] = 0; 
                add_rs_vk[i] = 0; 
                add_rs_qj[i] = TAG_NONE; 
                add_rs_qk[i] = TAG_NONE; 
                add_rs_op[i] = 0; 
                add_rs_dest[i] = 0; 
            end 
             
            // ???????? 
            for (i = 0; i < MUL_RS_SIZE; i = i + 1) begin 
                mul_rs_busy[i] = 0; 
                mul_rs_vj[i] = 0; 
                mul_rs_vk[i] = 0; 
                mul_rs_qj[i] = TAG_NONE; 
                mul_rs_qk[i] = TAG_NONE; 
                mul_rs_op[i] = 0; 
                mul_rs_dest[i] = 0; 
            end 
             
            // ???Load??? 
            for (i = 0; i < LD_RS_SIZE; i = i + 1) begin 
                ld_rs_busy[i] = 0; 
                ld_rs_addr[i] = 0; 
                ld_rs_vj[i] = 0; 
                ld_rs_qj[i] = TAG_NONE; 
                ld_rs_dest[i] = 0; 
            end 
             
            // ???Store??? 
            for (i = 0; i < ST_RS_SIZE; i = i + 1) begin 
                st_rs_busy[i] = 0; 
                st_rs_addr[i] = 0; 
                st_rs_vj[i] = 0; 
                st_rs_vk[i] = 0; 
                st_rs_qj[i] = TAG_NONE; 
                st_rs_qk[i] = TAG_NONE; 
            end 
             
            // ??????? 
            add_timer = 0; 
            mul_timer = 0; 
            div_timer = 0; 
            ld_timer = 0; 
            st_timer = 0; 
             
            add_unit_busy = 0; 
            mul_unit_busy = 0; 
            ld_unit_busy = 0; 
            st_unit_busy = 0; 
             
            // ???CDB 
            cdb_valid = 0; 
            cdb_tag = TAG_NONE; 
            cdb_value = 0; 
             
            // ???????? 
            cycles = 0; 
        end 
    endtask 
     
    // ???? 
    task issue_instruction; 
        reg [31:0] instr; 
        reg [5:0] op; 
        reg [4:0] rs, rt, rd; 
        reg [15:0] imm; 
        reg found; 
        integer i; 
        begin 
            if (queue_size > 0) begin 
                instr = instruction_queue[queue_head]; 
                op = instr[31:26]; 
                rs = instr[25:21]; 
                rt = instr[20:16]; 
                rd = instr[15:11]; 
                imm = instr[15:0]; 
                 
                found = 0; 
                 
                // ??????????????? 
                case (op) 
                    OP_ADD, OP_SUB: begin 
                        // ?????????? 
                        for (i = 0; i < ADD_RS_SIZE && !found; i = i + 1) begin 
                            if (!add_rs_busy[i]) begin 
                                add_rs_busy[i] = 1; 
                                add_rs_op[i] = op; 
                                add_rs_dest[i] = rd; 
                                 
                                // ??????????? 
                                if (reg_status[rs] == TAG_NONE) begin 
                                    add_rs_vj[i] = register_file[rs]; 
                                    add_rs_qj[i] = TAG_NONE; 
                                end else begin 
                                    add_rs_qj[i] = reg_status[rs]; 
                                end 
                                 
                                if (reg_status[rt] == TAG_NONE) begin 
                                    add_rs_vk[i] = register_file[rt]; 
                                    add_rs_qk[i] = TAG_NONE; 
                                end else begin 
                                    add_rs_qk[i] = reg_status[rt]; 
                                end 
                                 
                                // ???????? 
                                reg_status[rd] = TAG_ADD_BASE + i; 
                                found = 1; 
                                 
                                // ???? 
                                queue_head = (queue_head + 1) % 16; 
                                queue_size = queue_size - 1; 
                            end 
                        end 
                    end 
                     
                    OP_MUL, OP_DIV: begin 
                        // ?????????? 
                        for (i = 0; i < MUL_RS_SIZE && !found; i = i + 1) begin 
                            if (!mul_rs_busy[i]) begin 
                                mul_rs_busy[i] = 1; 
                                mul_rs_op[i] = op; 
                                mul_rs_dest[i] = rd; 
                                 
                                // ??????????? 
                                if (reg_status[rs] == TAG_NONE) begin 
                                    mul_rs_vj[i] = register_file[rs]; 
                                    mul_rs_qj[i] = TAG_NONE; 
                                end else begin 
                                    mul_rs_qj[i] = reg_status[rs]; 
                                end 
                                 
                                if (reg_status[rt] == TAG_NONE) begin 
                                    mul_rs_vk[i] = register_file[rt]; 
                                    mul_rs_qk[i] = TAG_NONE; 
                                end else begin 
                                    mul_rs_qk[i] = reg_status[rt]; 
                                end 
                                 
                                // ???????? 
                                reg_status[rd] = TAG_MUL_BASE + i; 
                                found = 1; 
                                 
                                // ???? 
                                queue_head = (queue_head + 1) % 16; 
                                queue_size = queue_size - 1; 
                            end 
                        end 
                    end 
                     
                    OP_LD: begin 
                        // ?????Load??? 
                        for (i = 0; i < LD_RS_SIZE && !found; i = i + 1) begin 
                            if (!ld_rs_busy[i]) begin 
                                ld_rs_busy[i] = 1; 
                                ld_rs_dest[i] = rt; 
                                 
                                // ?????? 
                                if (reg_status[rs] == TAG_NONE) begin 
                                    ld_rs_addr[i] = register_file[rs] + imm; 
                                    ld_rs_qj[i] = TAG_NONE; 
                                end else begin 
                                    ld_rs_qj[i] = reg_status[rs]; 
                                    ld_rs_vj[i] = imm;  // ????? 
                                end 
                                 
                                // ???????? 
                                reg_status[rt] = TAG_LD_BASE + i; 
                                found = 1; 
                                 
                                // ???? 
                                queue_head = (queue_head + 1) % 16; 
                                queue_size = queue_size - 1; 
                            end 
                        end 
                    end 
                     
                    OP_ST: begin 
                        // ?????Store??? 
                        for (i = 0; i < ST_RS_SIZE && !found; i = i + 1) begin 
                            if (!st_rs_busy[i]) begin 
                                st_rs_busy[i] = 1; 
                                 
                                // ?????? 
                                if (reg_status[rs] == TAG_NONE) begin 
                                    st_rs_addr[i] = register_file[rs] + imm; 
                                    st_rs_qj[i] = TAG_NONE; 
                                end else begin 
                                    st_rs_qj[i] = reg_status[rs]; 
                                    st_rs_vj[i] = imm;  // ????? 
                                end 
                                 
                                // ???????????? 
                                if (reg_status[rt] == TAG_NONE) begin 
                                    st_rs_vk[i] = register_file[rt]; 
                                    st_rs_qk[i] = TAG_NONE; 
                                end else begin 
                                    st_rs_qk[i] = reg_status[rt]; 
                                end 
                                 
                                found = 1; 
                                 
                                // ???? 
                                queue_head = (queue_head + 1) % 16; 
                                queue_size = queue_size - 1; 
                            end 
                        end 
                    end 
                endcase 
            end 
        end 
    endtask 
     
    // ???? 
    task execute_stage; 
        reg [31:0] result; 
        integer i, j; 
        begin 
            // ????????????????? 
            if (!add_unit_busy) begin 
                for (i = 0; i < ADD_RS_SIZE; i = i + 1) begin 
                    if (add_rs_busy[i] && add_rs_qj[i] == TAG_NONE && add_rs_qk[i] == TAG_NONE) begin 
                        add_unit_busy = 1; 
                        add_rs_index = i; 
                         
                        // ????? 
                        case (add_rs_op[i]) 
                            OP_ADD: add_timer = 2;  // ADD????2??? 
                            OP_SUB: add_timer = 2;  // SUB????2??? 
                            default: add_timer = 1; 
                        endcase 
                         
                        // ???????????? 
                        i = ADD_RS_SIZE;  // ??break?? 
                    end 
                end 
            end else if (add_timer > 0) begin 
                add_timer = add_timer - 1; 
                 
                // ???? 
                if (add_timer == 0) begin 
                    case (add_rs_op[add_rs_index]) 
                        OP_ADD: result = add_rs_vj[add_rs_index] + add_rs_vk[add_rs_index]; 
                        OP_SUB: result = add_rs_vj[add_rs_index] - add_rs_vk[add_rs_index]; 
                        default: result = 0; 
                    endcase 
                     
                    // ?????CDB 
                    cdb_valid = 1; 
                    cdb_tag = TAG_ADD_BASE + add_rs_index; 
                    cdb_value = result; 
                     
                    // ???????? 
                    add_rs_busy[add_rs_index] = 0; 
                    add_unit_busy = 0; 
                end 
            end 
             
            // ????????????????? 
            if (!mul_unit_busy) begin 
                for (j = 0; j < MUL_RS_SIZE; j = j + 1) begin 
                    if (mul_rs_busy[j] && mul_rs_qj[j] == TAG_NONE && mul_rs_qk[j] == TAG_NONE) begin 
                        mul_unit_busy = 1; 
                        mul_rs_index = j; 
                         
                        // ????? 
                        case (mul_rs_op[j]) 
                            OP_MUL: mul_timer = 5;  // MUL????5??? 
                            OP_DIV: div_timer = 10; // DIV????10??? 
                            default: mul_timer = 1; 
                        endcase 
                         
                        // ???????????? 
                        j = MUL_RS_SIZE;  // ??break?? 
                    end 
                end 
            end else if (mul_timer > 0) begin 
                mul_timer = mul_timer - 1; 
                 
                // ???? 
                if (mul_timer == 0) begin 
                    case (mul_rs_op[mul_rs_index]) 
                        OP_MUL: result = mul_rs_vj[mul_rs_index] * mul_rs_vk[mul_rs_index]; 
                        default: result = 0; 
                    endcase 
                     
                    // ?????CDB 
                    cdb_valid = 1; 
                    cdb_tag = TAG_MUL_BASE + mul_rs_index; 
                    cdb_value = result; 
                     
                    // ???????? 
                    mul_rs_busy[mul_rs_index] = 0; 
                    mul_unit_busy = 0; 
                end 
            end else if (div_timer > 0) begin 
                div_timer = div_timer - 1; 
                 
                // ???? 
                if (div_timer == 0) begin 
                    result = mul_rs_vj[mul_rs_index] / mul_rs_vk[mul_rs_index]; 
                     
                    // ?????CDB 
                    cdb_valid = 1; 
                    cdb_tag = TAG_MUL_BASE + mul_rs_index; 
                    cdb_value = result; 
                     
                    // ???????? 
                    mul_rs_busy[mul_rs_index] = 0; 
                    mul_unit_busy = 0; 
                end 
            end 
             
            // ??Load????????????? 
            if (!ld_unit_busy) begin 
                for (i = 0; i < LD_RS_SIZE; i = i + 1) begin 
                    if (ld_rs_busy[i] && ld_rs_qj[i] == TAG_NONE) begin 
                        ld_unit_busy = 1; 
                        ld_rs_index = i; 
                        ld_timer = 3;  // LD????3??? 
                         
                        // ???????????? 
                        i = LD_RS_SIZE;  // ??break?? 
                    end 
                end 
            end else if (ld_timer > 0) begin 
                ld_timer = ld_timer - 1; 
                 
                // ???? 
                if (ld_timer == 0) begin 
                    // ??????? 
                    result = memory[ld_rs_addr[ld_rs_index]]; 
                     
                    // ?????CDB 
                    cdb_valid = 1; 
                    cdb_tag = TAG_LD_BASE + ld_rs_index; 
                    cdb_value = result; 
                     
                    // ???????? 
                    ld_rs_busy[ld_rs_index] = 0; 
                    ld_unit_busy = 0; 
                end 
            end 
             
            // ??Store????????????? 
            if (!st_unit_busy) begin 
                for (i = 0; i < ST_RS_SIZE; i = i + 1) begin 
                    if (st_rs_busy[i] && st_rs_qj[i] == TAG_NONE && st_rs_qk[i] == TAG_NONE) begin 
                        st_unit_busy = 1; 
                        st_rs_index = i; 
                        st_timer = 3;  // ST????3??? 
                         
                        // ???????????? 
                        i = ST_RS_SIZE;  // ??break?? 
                    end 
                end 
            end else if (st_timer > 0) begin 
                st_timer = st_timer - 1; 
                 
                // ???? 
                if (st_timer == 0) begin 
                    // ???????? 
                    memory[st_rs_addr[st_rs_index]] = st_rs_vk[st_rs_index]; 
                     
                    // ???????? 
                    st_rs_busy[st_rs_index] = 0; 
                    st_unit_busy = 0; 
                end 
            end 
        end 
    endtask 
     
    // ???? 
    task writeback_stage; 
        integer i; 
        begin 
            if (cdb_valid) begin 
                // ???????? 
                for (i = 0; i < 32; i = i + 1) begin 
                    if (reg_status[i] == cdb_tag) begin 
                        register_file[i] = cdb_value; 
                        reg_status[i] = TAG_NONE; 
                    end 
                end 
                 
                // ??????? 
                for (i = 0; i < ADD_RS_SIZE; i = i + 1) begin 
                    if (add_rs_busy[i]) begin 
                        if (add_rs_qj[i] == cdb_tag) begin 
                            add_rs_vj[i] = cdb_value; 
                            add_rs_qj[i] = TAG_NONE; 
                        end 
                        if (add_rs_qk[i] == cdb_tag) begin 
                            add_rs_vk[i] = cdb_value; 
                            add_rs_qk[i] = TAG_NONE; 
                        end 
                    end 
                end 
                 
                // ??????? 
                for (i = 0; i < MUL_RS_SIZE; i = i + 1) begin 
                    if (mul_rs_busy[i]) begin 
                        if (mul_rs_qj[i] == cdb_tag) begin 
                            mul_rs_vj[i] = cdb_value; 
                            mul_rs_qj[i] = TAG_NONE; 
                        end 
                        if (mul_rs_qk[i] == cdb_tag) begin 
                            mul_rs_vk[i] = cdb_value; 
                            mul_rs_qk[i] = TAG_NONE; 
                        end 
                    end 
                end 
                 
                // ??Load??? 
                for (i = 0; i < LD_RS_SIZE; i = i + 1) begin 
                    if (ld_rs_busy[i]) begin 
                        if (ld_rs_qj[i] == cdb_tag) begin 
                            ld_rs_addr[i] = cdb_value + ld_rs_vj[i];  // ?? + ??? 
                            ld_rs_qj[i] = TAG_NONE; 
                        end 
                    end 
                end 
                 
                // ??Store??? 
                for (i = 0; i < ST_RS_SIZE; i = i + 1) begin 
                    if (st_rs_busy[i]) begin 
                        if (st_rs_qj[i] == cdb_tag) begin 
                            st_rs_addr[i] = cdb_value + st_rs_vj[i];  // ?? + ??? 
                            st_rs_qj[i] = TAG_NONE; 
                        end 
                        if (st_rs_qk[i] == cdb_tag) begin 
                            st_rs_vk[i] = cdb_value; 
                            st_rs_qk[i] = TAG_NONE; 
                        end 
                    end 
                end 
                 
                // ??CDB 
                cdb_valid = 0; 
                cdb_tag = TAG_NONE; 
            end 
        end 
    endtask 
     
    // ?????? 
    task print_status; 
        integer i; 
        begin 
            $display("======== Cycle: %d ========", cycles); 
             
            // ??????? 
            $display("Register File:"); 
            for (i = 0; i < 8; i = i + 1) begin 
                $display("R%0d: %d\tR%0d: %d\tR%0d: %d\tR%0d: %d",  
                    i, register_file[i],  
                    i+8, register_file[i+8],  
                    i+16, register_file[i+16],  
                    i+24, register_file[i+24]); 
            end 
             
            // ??????? 
            $display("\nAdd Reservation Stations:"); 
            for (i = 0; i < ADD_RS_SIZE; i = i + 1) begin 
                if (add_rs_busy[i]) begin 
                    $display("RS%0d: Busy=1, Op=%d, Vj=%d, Vk=%d, Qj=%d, Qk=%d, Dest=%d",  
                        i, add_rs_op[i], add_rs_vj[i], add_rs_vk[i],  
                        add_rs_qj[i], add_rs_qk[i], add_rs_dest[i]); 
                end else begin 
                    $display("RS%0d: Busy=0", i); 
                end 
            end 
             
            $display("\nMul Reservation Stations:"); 
            for (i = 0; i < MUL_RS_SIZE; i = i + 1) begin 
                if (mul_rs_busy[i]) begin 
                    $display("RS%0d: Busy=1, Op=%d, Vj=%d, Vk=%d, Qj=%d, Qk=%d, Dest=%d",  
                        i, mul_rs_op[i], mul_rs_vj[i], mul_rs_vk[i],  
                        mul_rs_qj[i], mul_rs_qk[i], mul_rs_dest[i]); 
                end else begin 
                    $display("RS%0d: Busy=0", i); 
                end 
            end 
             
            $display("\nLoad Reservation Stations:"); 
            for (i = 0; i < LD_RS_SIZE; i = i + 1) begin 
                if (ld_rs_busy[i]) begin 
                    $display("RS%0d: Busy=1, Addr=%d, Qj=%d, Dest=%d",  
                        i, ld_rs_addr[i], ld_rs_qj[i], ld_rs_dest[i]); 
                end else begin 
                    $display("RS%0d: Busy=0", i); 
                end 
            end 
             
            $display("\nStore Reservation Stations:"); 
            for (i = 0; i < ST_RS_SIZE; i = i + 1) begin 
                if (st_rs_busy[i]) begin 
                    $display("RS%0d: Busy=1, Addr=%d, Vk=%d, Qj=%d, Qk=%d",  
                        i, st_rs_addr[i], st_rs_vk[i], st_rs_qj[i], st_rs_qk[i]); 
                end else begin 
                    $display("RS%0d: Busy=0", i); 
                end 
            end 
             
            // ???????? 
            $display("\nExecution Units:"); 
            $display("ADD Unit: Busy=%d, Timer=%d", add_unit_busy, add_timer); 
            $display("MUL Unit: Busy=%d, Timer=%d", mul_unit_busy, mul_timer); 
            $display("DIV Unit: Busy=%d, Timer=%d", mul_unit_busy, div_timer); 
            $display("LD Unit: Busy=%d, Timer=%d", ld_unit_busy, ld_timer); 
            $display("ST Unit: Busy=%d, Timer=%d", st_unit_busy, st_timer); 
             
            // ??CDB?? 
            $display("\nCommon Data Bus:"); 
            $display("Valid=%d, Tag=%d, Value=%d", cdb_valid, cdb_tag, cdb_value); 
             
            // ?????? 
            $display("\nMemory [relevant locations]:"); 
            $display("Memory[100] = %d", memory[100]); 
            $display("Memory[104] = %d", memory[104]); 
            $display("Memory[108] = %d", memory[108]); 
             
            $display("==========================\n"); 
        end 
    endtask 
     
    // ??? 
    always @(posedge clk or posedge reset) begin 
        if (reset) begin 
            init_system(); 
            init_test_program(); 
        end else begin 
            // ??Tomasulo??????? 
            writeback_stage();  // ???? 
            execute_stage();    // ???? 
            issue_instruction();// ???? 
             
            // ???? 
            print_status(); 
             
            // ?????? 
            cycles = cycles + 1; 
        end 
    end 
     
endmodule 

