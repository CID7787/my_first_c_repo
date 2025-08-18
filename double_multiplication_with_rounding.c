#ifndef headerfile
    #include <time.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
#endif

/* COMMENTS TO PROCESS OF BUILDING long_mantissa_multiplication FUNCTION
We are shifting the number by 27 to the left so that it becomes smaller
There COULD be any number, but there SHOULD only be 1 number.
Why 32?
Because any number larger than 32 will not fit into long unsigned int when we multiply 2 of them together.
N_sign_bits + N_exp_bits - 1
seeeeeee eeeemmmm mmmmmmmm mmmmmmmm  mmmmmmmm mmmmmmmm mmmmmmmm mmmmmmmm
00000000 00011111 11111111 11111111  11111111 11111111 11111111 11111111 (a)
=
00000000 00000000 00000000 00000000  11111111 11111111 11111111 11111111 (right)
+
00000000 00000001 11111111 11111111  00000000 00000000 00000000 00000000 (left)

00000000 00000001 11111111 11111111
right * right = x < 64
right_result = right * right == (a & mask) * (b & mask)
left = 00000000 00000001 11111111 11111111 << 32
left * left = 00000000 00000001 11111111 11111111 * 00000000 00000001 11111111 11111111 << 64
result_left = (left >> 32) * (left >> 32) == (a >> 32) * (b >> 32)
a * b = (a_left + a_right) * (b_left + b_right) = a_left * (b_left + b_right) + a_right * (b_left + b_right) == (a_left * b_left) + (a_left * b_right + a_right * b_left) + (a_right * b_right)
a * b = left_result right_result + (a_left * b_right + a_right * b_left)
a_left * b_right = (00000000 0000000a aaaaaaaa aaaaaaaa * bbbbbbbb bbbbbbbb bbbbbbbb bbbbbbbb) << 32
a_right * b_left = (aaaaaaaa aaaaaaaa aaaaaaaa aaaaaaaa * 00000000 0000000b bbbbbbbb bbbbbbbb) << 32
*/

lluint long_mantissa_multiplication(long unsigned int a, long unsigned int b){ 	
    // getting left and right parts of multiplicand and multiplier 
    long unsigned int mask_r = 0xffffffff;
    error err = NO_ERROR;
    long unsigned int a_l = a >> (sizeof(a) << 2),
                      b_l = b >> (sizeof(a) << 2),
                      a_r = a & mask_r,
                      b_r = b & mask_r;
    lluint result = {.high = a_l * b_l,
                     .low  = a_r * b_r };
    long unsigned int middle = (a_l * b_r) + (a_r * b_l);
    result.low = safe_luint_addition(result.low, middle << 32, &err);
    result.high += (middle >> 32) + else0(err, 1);
    return result;
}    

dbits safe_double_mantissa_multiplication_with_rounding(dbits a, dbits b, error* err){
    a.luint = DOUBLE_MANTISSA_HIDDEN_ONE | a.parts.mantissa;
    b.luint = DOUBLE_MANTISSA_HIDDEN_ONE | b.parts.mantissa;
    // remove useless zeros: shift right until one of the numbers runs out of zeroes in the least significant 
    // char counter_of_lost_zeroes = 1;
    while(!((a.luint | b.luint) & 1ul)) {
         a.luint >>= 1; b.luint >>= 1;
        //  counter_of_lost_zeroes++; 
    }

    // if((multiplicand.luint & multiplier.luint) == 1) { multiplicand.luint = DOUBLE_MANTISSA_HIDDEN_ONE; return multiplicand; }

    lluint result = long_mantissa_multiplication(a.luint, b.luint);

    unsigned int bin_point_shift = how_many_bits_until_eldest_1(a.luint) << 1;
    // getting exponen
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
    char a_nan_cond = (a.parts.exp > MAX_NORM_DOUBLE_EXP) & a.parts.mantissa;
    char b_nan_cond = (b.parts.exp > MAX_NORM_DOUBLE_EXP) & b.parts.mantissa;
    if(a_nan_cond | b_nan_cond){ return ternary(a_nan_cond, a.d, b.d); }
    dbits result = safe_double_mantissa_multiplication_with_rounding(a, b, err); if(*err){ return result.d; }
    unsigned int exponent = safe_uint_addition(a.parts.exp, b.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; }
    // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
    *err = else0(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(exponent <= MAX_NORM_DOUBLE_EXP, *err); if(*err){ return result.d; }
    result.parts.exp = exponent;
    result.parts.sign = a.parts.sign ^ b.parts.sign;
    return result.d;
}


void my_lcg(void){
    dbits a, b;
    error err = NO_ERROR;

    printf("the product of multiplication %f and \t %f equal to %f\n", a.d, b.d , safe_double_multiplication_with_rounding(a, b, &err));
    printf("error%u", err);
}
