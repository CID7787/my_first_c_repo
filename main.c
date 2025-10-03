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
// #include "my_test.c"

int main(){ 
    vecN a = vector_creation(FLOAT, 5, B4type_f_elements((float[]){3.4, 6.3, 9.4, 5.6, 54.2}));
    vecN b = vector_creation(FLOAT, 5, B4type_f_elements((float[]){34.1, 13.1, 35.7, 1.2, 6.2}));
    a = vector_negation(b);
    int v = 0;
    while(v < a.n){
        printf("\na.elements.b4.f[%d] = %f\n", v, a.elements.b4.f[v]);
        v++;
    }
    // test negation, addition, multiplication and exponentiation(check extremums and random values)
    return 0;
}


/*
v = 0;
while(v < a.n){
    printf("\nb.elements.b4.f[%d] = %f\n", v, b.elements.b4.f[v]);
    v++;
}
*/