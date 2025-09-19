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
#include "my_test.c"

int main(){ 

    error err = NO_ERROR;
    dbits a = (dbits){ .d = 1.5}, b = (dbits){ .d = 1.5};
    printf("\ntest function: product of multiplication a: %f\t and b:%f\t equal to %f\n", a.d, b.d, safe_double_multiplication_with_rounding_test_v(a, b, &err));
    printf("\ntested function: product of multiplication a: %f\t and b:%f\t equal to %f\n", a.d, b.d, safe_double_multiplication_with_rounding(a, b, &err));
    return 0;
}


/*

1.1 * 2^0
1.1 * 2^0
 11
11
10.01
*/