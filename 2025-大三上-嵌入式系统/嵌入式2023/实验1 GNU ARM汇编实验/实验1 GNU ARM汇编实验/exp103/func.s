	.text                          @代码段
@*********** Function BitValue **********************************
@   入口参数  R0:密钥中的字符二进制值                           *
@****************************************************************
	.align	2                      @按照2x2对齐
	.global	BitValue               @声明为全局函数
	.type	BitValue, %function    @BitValue类型:函数
BitValue:                              @函数名
	mov	ip, sp                 @ip1=sp1
	stmfd	sp!, {fp, ip, lr, pc}  @sp2=sp1+4x4 fp(r11) ip(r12) 
	sub	fp, ip, #4             @fp1=ip1-4
	sub	sp, sp, #4             @sp3=sp2+4
	mov	r3, r0                 @入口参数=>r0=0-7
	strb	r3, [fp, #-13]         @保存入口参数,一字节
	mov	r3, #1                 @x=1         
	strb	r3, [fp, #-15]
	mov	r3, #0                 @i=0 乡党循环变量
	strb	r3, [fp, #-14]
.L2:
	ldrb	r2, [fp, #-14]	       @i =>r2
	ldrb	r3, [fp, #-13]	       @r0=>r3    ASCII char Binary Value
	cmp	r2, r3  
	bcs	.L3                    @无符号大于等于 r2>r3 (i>r0)
	ldrb	r3, [fp, #-15]         @r3<=x
	mov	r3, r3, asl #1         @x=r3<<1
	strb	r3, [fp, #-15]         
	ldrb	r3, [fp, #-14]
	add	r3, r3, #1             @i=i++
	strb	r3, [fp, #-14]
	b	.L2
.L3:
	ldrb	r3, [fp, #-15]	       
        mov r0,r3                      @ return r0=x
                                       @ R0=0 return 1;  R0=1 return 2;  R0=2 return 4;   
                                       @ R0=3 return 8;  R0=4 return 16; R0=5 return 32;
                                       @ R0=6 return 64; R0=7 return 128;
	ldmfd	sp, {r3, fp, sp, pc}
	.size	BitValue, .-BitValue

	.align	2
	.global	Encryption             @声明为全局函数        
	.type	Encryption, %function  @Encryption为函数
Encryption:                            @函数名
	mov	ip, sp
	stmfd	sp!, {fp, ip, lr, pc}
	sub	fp, ip, #4
	sub	sp, sp, #12
	mov	r3, r0                 @入口参数:R0=>要加密的字节数据
	str	r1, [fp, #-20]         @入口参数:R1=>密钥字符串=>[fp-20]
	strb	r3, [fp, #-13]         @R0[7:0]          =>[fp-13]
	mov	r3, #0                 @data=0           =>[fp-21]
	strb	r3, [fp, #-21]
	mvn	r3, #127               @Temp=0x80, 0x80-1=>[fp-22]
	strb	r3, [fp, #-22]
	mov	r3, #0                 @i=0循环变量      =>[fp-23]
	strb	r3, [fp, #-23]
.L6:
	ldrb	r3, [fp, #-23]	@ zero_extendqisi2
	cmp	r3, #7
	bhi	.L7
	ldrb	r2, [fp, #-23]	@ zero_extendqisi2
	ldr	r3, [fp, #-20]
	add	r3, r2, r3
	ldrb	r3, [r3, #0]
	sub	r3, r3, #48
	and	r3, r3, #255
	mov	r0, r3
	bl	BitValue
	mov	r2, r0
	ldrb	r3, [fp, #-13]
	and	r3, r2, r3
	and	r3, r3, #255
	cmp	r3, #0
	beq	.L9
	ldrb	r2, [fp, #-21]
	ldrb	r3, [fp, #-22]
	add	r3, r2, r3
	strb	r3, [fp, #-21]
.L9:
	ldrb	r3, [fp, #-22]	@ zero_extendqisi2
	mov	r3, r3, lsr #1
	strb	r3, [fp, #-22]
	ldrb	r3, [fp, #-23]
	add	r3, r3, #1
	strb	r3, [fp, #-23]
	b	.L6
.L7:
	ldrb	r3, [fp, #-21]	@ zero_extendqisi2
	mov	r0, r3
	sub	sp, fp, #12
	ldmfd	sp, {fp, sp, pc}


