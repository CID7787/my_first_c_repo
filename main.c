#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#define headerfile 1

#include "user_defined_datatypes.c"
#include "constants.c"
#include "logical_functions_of_decision.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
#include "additional_functions.c"
// #include "my_test.c"

int main(){ 
    unsigned int uint_fmax = 0b01111111011111111111111111111111;
    float fmax = *(float*)&uint_fmax;
    // uint_fmax = (unsigned int)fmax;
    printf("\nfmax: %f\n", fmax);
    printf("to uint casted fmax: %u\n", (unsigned int)fmax);
    return 0;
}

