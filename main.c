#include <stdio.h>
#include <time.h>

#define headerfile 1

#include "user_defined_datatypes.c"
#include "constants.c"
#include "logical_functions_of_decision.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
#include "double_multiplication_with_rounding.c"

int main(){
    long unsigned int sum = 0, current_clock = 0;
clock_t start_time = clock();
    dbits d1 = { .d = 2.2}, d2 = { .d = 2.2};
    error err = NO_ERROR;
for(int i = 0; i < 11; ++i){
    safe_double_multiplication_with_rounding(d1, d2, &err);
    sum += clock() - current_clock;
    current_clock = clock();
    printf("\nEND of loop: %lu, sum: %lu", current_clock, sum);
    d1.d += current_clock;
    d2.d += current_clock;
}
printf("\nouter:%lu\n", clock() - start_time);
    return 0;
}