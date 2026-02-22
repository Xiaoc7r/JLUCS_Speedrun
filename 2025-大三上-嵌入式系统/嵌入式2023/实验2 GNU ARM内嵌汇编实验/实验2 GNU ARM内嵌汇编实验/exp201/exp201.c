#include <stdio.h>
unsigned int BytePositionSwap(unsigned int x)
{
unsigned int y=0; 
asm volatile (
        "eor r4, %1, %1, ror #16\n\t"
        "bic r4, r4, #0x00FF0000\n\t"
        "mov %0, %1, ror #8\n\t"
        "eor %0, %0, r4, lsr #8"
        : "=r" (y)
        : "0"(x)
        : "r4"
);
return y;
}



int main()
{
 unsigned int  x;
 unsigned int y=0x11223344;
 while(1)
{
 printf("Input Data:");
 scanf("%x",&y);
 printf("\n"); 
 printf("y=%x\n",y);
 if(y==0) break;
 x=BytePositionSwap(y);
 printf("x=%x\n",x);
}
 return 0;
}

