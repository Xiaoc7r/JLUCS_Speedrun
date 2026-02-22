#include <stdio.h>

extern int addsum(int a,int b);
int main()
{
        int x=10,y=12,z;
	printf("x=%d   y=%d\n",x,y);
        z=addsum(x,y);
        printf("x + y =%d\n",z);	
	return 0;
}
