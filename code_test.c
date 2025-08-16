#ifndef abracadabra
    #include <stdio.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "safe_arithmetic_functions.c"
    #include "bitwise_functions.c"
#endif


dbits safe_double_mantissa_multiplication_without_rounding(dbits multiplicand, dbits multiplier, error* err){
    multiplicand.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplicand.parts.mantissa;
    multiplier.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplier.parts.mantissa;
    // remove useless zeros
    while (!(multiplicand.luint & 1ul)) { multiplicand.luint >>= 1; }
    while (!(  multiplier.luint & 1ul)) { multiplier.luint >>= 1; }
    // variable declaration
    unsigned int bits_after_binary_point = safe_uint_addition(how_many_bits_until_eldest_1(multiplicand.luint), how_many_bits_until_eldest_1(multiplier.luint), err); if(*err){ return multiplicand; }

    multiplier.luint = safe_luint_multiplication(multiplicand.luint, multiplier.luint, err); if(*err){ return multiplier; } //@TODO: make it so any result fits.(in the version with rounding)
    // variable declararion 
    unsigned int mant_overflow_cond = multiplier.luint > MAX_DOUBLE_MANTISSA; // I suspect this can be simpler with binary operators. Something like this: (multiplier.luint & ~MAX_DOUBLE_MANTISSA) 
    *err = else0(mant_overflow_cond, POSITIVE_OVERFLOW) | else0(!mant_overflow_cond, *err); if(*err){ return multiplicand; }
    bits_after_binary_point = safe_int_addition(how_many_bits_until_eldest_1(multiplier.luint), -bits_after_binary_point, err); if(*err){ return multiplier; }
    // moving product's first one to hidden one position
    multiplier.luint <<= safe_int_addition(AMOUNT_OF_DOUBLE_MANTISSA_BITS, -how_many_bits_until_eldest_1(multiplier.luint), err); if(*err){ return multiplier; }
    multiplier.parts.exp = bits_after_binary_point;
    return multiplier;
}

double safe_double_multiplication_without_rounding(dbits multiplicand, dbits multiplier, error* err){
    // check whether or not one of arguments equal to 0
    if(!multiplicand.d | !multiplier.d){ return 0; }
    dbits result = multiplier;
    result = safe_double_mantissa_multiplication_without_rounding(multiplicand, multiplier, err); if(*err){ return result.d; }
    unsigned int exponent = safe_uint_addition(multiplicand.parts.exp, multiplier.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; }
    // check whether of not exponent value bigger than MAX_DOUBLE_EXPONENT
    *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW, *err); if(*err){ return result.d; }
    result.parts.exp = exponent;
    result.parts.sign = multiplicand.parts.sign ^ multiplier.parts.sign;
    return result.d;
}




int main(){

}
