#include <stdio.h>
#include <time.h>

#define headerfile 1

#include "user_defined_datatypes.c"
#include "constants.c"
#include "logical_functions_of_decision.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
// #include "double_multiplication_with_rounding.c"
#include "code_test.c"

int main(){    error err = NO_ERROR;
    printf("result: %f", safe_double_multiplication_with_rounding((dbits){ .d = -5.240}, (dbits){ .d = -885.2}, &err));
    return 0;
}
