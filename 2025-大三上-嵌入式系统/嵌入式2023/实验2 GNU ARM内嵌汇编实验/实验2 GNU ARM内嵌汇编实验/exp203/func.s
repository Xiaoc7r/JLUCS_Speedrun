	.text
	.align	2
	.global	bubbleSort
	.type	bubbleSort, %function
bubbleSort:
	SUB R1,R1,#4   @R1<=LEN-1
        MOV R4,#0      @i
	MOV R5,#0      @j
l1:    LDR R7,[R0,R5]	
       ADD R6,R5,#4
       LDR R8,[R0,R6]  
       CMP R7,R8
       BLE l2
       STR R8,[R0,R5] 
       STR R7,[R0,R6]
l2:    ADD R5,R5,#4
       SUB R6,R1,R4
       CMP R5,R6
       BLE l1
       MOV R5,#0
       ADD R4,R4,#4
       CMP R4,R1
       BLE l1 
       MOV PC,LR
