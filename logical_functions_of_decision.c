#ifndef headerfile
#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#include "user_defined_datatypes.c"
#include "constants.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
#include "new_vector_functions.c"
#endif
long unsigned int else0_array(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}

long unsigned int else0(long unsigned int condition, long unsigned int x){
    return x & (~0ul + !condition);
    // return -!!condition & x;
}


long unsigned int ternary(unsigned char condition, long unsigned int true_value, long unsigned int false_value){
    long unsigned int array[2] = {true_value, false_value};
    return array[!condition];
}// TODO: write new ternary that won't use array

int have_frac_part(dbits a){
    int norm_exp = a.parts.exp - DOUBLE_EXP_BIAS;
    int result = -(norm_exp <= 53) & norm_exp; // MAX_DOUBLE_MANTISSA '1' bits amount
    return !!(a.parts.mantissa & (MAX_DOUBLE_MANTISSA >> result));
}