#include <stdio.h>
#include "func.h"
int main (void) {
   char srcData[] = {3,5,1,7,4,9,6,8,
	             0,4,1,2,3,4,5,6,
		     8,9,0,1,2,3,4,5,
		     3,4,5,6,1,2,3,4,
                     1,2,3,4,4,4};
    char dstData[50]; 
    int len =38;
    int i;
    
    for (i=0; i<len; i++)
       printf ("%d ", srcData[i]);
       printf ("\n");
    blockCopy(srcData,dstData,len);
    for(i=0;i<len;i++)
	    printf("%d ",dstData[i]);
       printf("\n");
    return 0;
}


