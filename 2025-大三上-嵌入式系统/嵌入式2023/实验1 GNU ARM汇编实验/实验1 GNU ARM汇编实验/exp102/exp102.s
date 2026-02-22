.data
        .align  2
.LC0:
        .ascii  "%d\012\000"
.text
	.extern sum
	.global	main
	.type	main, %function
main:
        mov r0,#9
        bl  sum
        mov r1,r0
        ldr     r3, .L1
        mov     r0, r3
        bl      printf
        mov pc,lr
.L1:
        .word   .LC0

