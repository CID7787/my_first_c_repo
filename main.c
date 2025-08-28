#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#define headerfile 1

#include <math.h>
#include "user_defined_datatypes.c"
#include "constants.c"
#include "logical_functions_of_decision.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"

int main(){ 
    error err = NO_ERROR;
    printf("%f", safe_double_addition((dbits){ .d = 4.76}, (dbits){ .d = -39.23}, &err));
    return 0;
}
