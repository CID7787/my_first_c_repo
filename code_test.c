#ifndef headerfile
    #include <stdio.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "safe_arithmetic_functions.c"
    #include "bitwise_functions.c"
#endif

lluint long_mantissa_multiplication(long unsigned int a, long unsigned int b){ 	
    // getting left and right parts of multiplicand and multiplier 
    long unsigned int mask_r = 0xffffffff;
    unsigned int sizeof_half_arg_type = sizeof(a) << 2; //(sizeof(a) << 3) >> 1 == sizeof_type_in_bits(sizeof(a) * 8(2^3)) / 2 == sizeof(a) * 4 
    error err = NO_ERROR;
    long unsigned int a_l = a >> sizeof_half_arg_type,
                      b_l = b >> sizeof_half_arg_type,
                      a_r = a & mask_r,
                      b_r = b & mask_r;
    lluint result = {.high = a_l * b_l,
                     .low  = a_r * b_r };
    long unsigned int middle = (a_l * b_r) + (a_r * b_l);
    result.low = safe_luint_addition(result.low, middle << sizeof(sizeof_half_arg_type), &err);
    result.high += (middle >> sizeof_half_arg_type) + else0(err, 1);
    return result;
}

dbits safe_double_mantissa_multiplication_with_rounding(dbits a, dbits b, error* err){
    a.luint = DOUBLE_MANTISSA_HIDDEN_ONE | a.parts.mantissa;
    b.luint = DOUBLE_MANTISSA_HIDDEN_ONE | b.parts.mantissa;
    lluint result = long_mantissa_multiplication(a.luint, b.luint);

    unsigned int bin_point_shift = how_many_bits_until_eldest_1(a.luint) << 1;
    // getting exponent
    a.luint = how_many_bits_until_eldest_1(result.high);
    b.luint = how_many_bits_until_eldest_1(result.low);
    a.luint += else0(result.high, 1);
    b.luint += else0(result.high, 1);
    // 1.0 * 2^1
    // 1.0 * 2^1
    // 1.00 >> 2 = 1
    bin_point_shift = ternary(a.luint, result.high >> safe_int_addition(bin_point_shift, -64, err), result.low >> bin_point_shift);
    bin_point_shift = !!(bin_point_shift & 0b10);
    // normalizing number and putting bin_point_shift into exponent
    result.high <<= safe_int_addition(64, -a.luint, err); // 0000 0100
    result.low >>= a.luint;
    result.high |= result.low;
    a.luint = how_many_bits_until_eldest_1(result.high);
    result.high = ternary(result.high < DOUBLE_MANTISSA_HIDDEN_ONE, result.high << safe_int_addition(52, -a.luint, err), result.high >> safe_int_addition(a.luint, -52, err));
    result.high = (DOUBLE_MANTISSA_HIDDEN_ONE - 1) & result.high;
    result.high = ternary(bin_point_shift, result.high | DOUBLE_MANTISSA_HIDDEN_ONE, result.high);
    b.luint = result.high;
    return b;
}

double safe_double_multiplication_with_rounding(dbits a, dbits b, error* err){
    if(!a.d | !b.d){ return 0; }
    char a_nan_cond = (a.parts.exp > MAX_NORM_DOUBLE_EXP) && a.parts.mantissa;
    char b_nan_cond = (b.parts.exp > MAX_NORM_DOUBLE_EXP) && b.parts.mantissa;
    if(a_nan_cond | b_nan_cond){ return ternary(a_nan_cond, a.d, b.d); }
    dbits result = safe_double_mantissa_multiplication_with_rounding(a, b, err); if(*err){ return result.d; }
    int exponent = a.parts.exp + b.parts.exp + result.parts.exp - DOUBLE_EXP_BIAS;
    // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
    *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW, ternary(exponent < 0, UNDERFLOW, *err)); if(*err){ return result.d; }
    result.parts.exp = exponent;
    result.parts.sign = a.parts.sign ^ b.parts.sign;
    return result.d;
}


