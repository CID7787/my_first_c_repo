#ifndef headerfile
    #include "user_defined_datatypes.c"
    #include "constants.c"
#endif

long unsigned int else0_array(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}

static inline uint64_t else0(uint64_t condition, uint64_t x){
    return -(!!condition) & x;
    // return x & (~0ul + !condition);
}

static inline uint64_t ternary(uint64_t condition, uint64_t true_value, uint64_t false_value){
    return (-(!!condition) & true_value) | (-(!condition) & false_value);
}

long unsigned int ternary_array(unsigned char condition, long unsigned int true_value, long unsigned int false_value){
    long unsigned int array[2] = {true_value, false_value};
    return array[!condition];
}

int8_t have_frac_part(dbits a){// TODO: fix function doesn't working correct
    int32_t norm_exp = a.parts.exp - DOUBLE_EXP_BIAS;
    return ((norm_exp < 0) | !!( a.parts.mantissa & (MAX_DOUBLE_MANTISSA >> (-(norm_exp > 0) & norm_exp)) )) && a.luint;
}