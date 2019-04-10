#include <stdio.h>
#include <math.h>

int length;


 int numLength(long long number){
     length = log10(number) + 1;
     return length;
 }