#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#define headerfile 1

#include "user_defined_datatypes.c"
#include "constants.c"
#include "additional_functions.c"
#include "logical_functions_of_decision.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
#include "new_vector_functions.c"
#include "test.c"   


char main(){
  vector_exponentiation_test();
  return 0;    
}


/*
int v = 0;
while(v < a.n){
    printf("\na.elements.b4.f[%d] = %f\n", v, a.elements.b4.f[v]);
    v++;
}
v = 0;
while(v < a.n){
    printf("\nb.elements.b4.f[%d] = %f\n", v, b.elements.b4.f[v]);
    v++;
}
*/