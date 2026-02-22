#include <stdio.h>
#include "func.h"
int main (void) {
    int arr[10] = {3,5,1,-7,4,9,-6,8,10,4};
    int len = 10;
    int i;
    
    bubbleSort (arr, 4*len);
    for (i=0; i<len; i++)
        printf ("%d\t", arr[i]);
    putchar ('\n');
    
    return 0;
}


