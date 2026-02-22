#include <stdio.h>

int main()
{
 unsigned char strKey[]="37165204";
 unsigned char sn=0xE6;
 unsigned char data=0;
 data=Encryption(sn,strKey);
 printf("\n%x    %x",0x000000FF&sn,0x000000FF&data);
 printf("\n");
 return 0;
}


