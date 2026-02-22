#include <stdio.h>
int sum(int n);
int sum(int n)
{
	int i,nResult=0;
	if(n<=0) return 0;
	for(i=1;i<=n;i++) nResult+=i;
	return nResult;
}

