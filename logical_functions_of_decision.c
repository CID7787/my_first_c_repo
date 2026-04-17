#ifndef headerfile
    #include "user_defined_datatypes.c"
    #include "constants.c"
#endif

long unsigned int else0_array(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}

static inline uint64_t else0(uint64_t condition, uint64_t x){
    return x & (~0ul + !condition);
    // return -!!condition & x;
}

static inline uint64_t ternary(uint64_t condition, uint64_t true_value, uint64_t false_value){
    return (-(!!condition) & true_value) | (-(!condition) & false_value);
}

long unsigned int ternary_array(unsigned char condition, long unsigned int true_value, long unsigned int false_value){
    long unsigned int array[2] = {true_value, false_value};
    return array[!condition];
}

int have_frac_part(dbits a){
    int norm_exp = a.parts.exp - DOUBLE_EXP_BIAS;
    int result = -(norm_exp <= 53) & norm_exp; // MAX_DOUBLE_MANTISSA '1' bits amount
    return !!(a.parts.mantissa & (MAX_DOUBLE_MANTISSA >> result));
}