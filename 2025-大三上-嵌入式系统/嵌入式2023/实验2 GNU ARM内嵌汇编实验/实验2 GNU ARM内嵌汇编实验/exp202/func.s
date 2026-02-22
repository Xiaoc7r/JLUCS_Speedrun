	.text
	.align	2
	.global	blockCopy
	.type	blockCopy, %function
blockCopy:      @ R0==>srcDtat  R1===>dstDate  R2==>len(byte)
        	MOVS   	r3, r2, LSR #5   	@bytes/32      
        	BEQ     	copywords       
        	STMFD   	sp!, {r4-r11} 	
octcopy:
        	LDMIA   	r0!, {r4-r11}  
        	STMIA   	r1!, {r4-r11} 
        	SUBS    	r3, r3, #1   
        	BNE     	octcopy     
		LDMFD   	sp!, {r4-r11}
copywords:      ANDS R3,R2,#0x0000001F
                MOVS R3,R2,LSR #2
                BEQ copybytes
wordcopy:	LDR   r4, [r0], #4     
  		STR   r4, [r1], #4    
  		SUBS  r3, r3, #1 
  		BNE   wordcopy  
copybytes:      ANDS R3,R2,#0x3
                BEQ l1 
copybyte:       LDRB R4,[R0],#1
                STRB R4,[R1],#1
                SUBS R3,R3,#1
                BNE copybyte 
l1:             MOV PC,LR

 	        MOV PC,LR
