#ifndef headerfile
    #include <time.h>
    #include <stdlib.h>
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
    unsigned int sizeof_half_arg_type = sizeof(a) << 2; //(sizeof(a) << 3) >> 1 == sizeof_type_in_bits(sizeof(a) * 8(2^3)) / 2 == sizeof(a) * 4 
    error err = NO_ERROR;
    long unsigned int a_l = a >> sizeof_half_arg_type,
                      b_l = b >> sizeof_half_arg_type,
                      a_r = a & mask_r,
                      b_r = b & mask_r;
    long unsigned int middle = (a_l * b_r) + (a_r * b_l);
    lluint result = {.high = (a_l * b_l) + (middle >> sizeof_half_arg_type),
                     .low  = safe_luint_addition(a_r * b_r, middle << sizeof_half_arg_type, &err) };
    result.high += else0(err, 1);
    return result;
}

dbits safe_double_mantissa_multiplication_with_rounding(dbits a, dbits b, error* err){
    a.luint = DOUBLE_MANTISSA_HIDDEN_ONE | a.parts.mantissa;
    b.luint = DOUBLE_MANTISSA_HIDDEN_ONE | b.parts.mantissa;
    lluint result = long_mantissa_multiplication(a.luint, b.luint);
    unsigned int exp = (result.high & (1ul << 41));
    a.luint = (result.high << (23 - exp)) | result.low >> (41 + exp);
    a.luint >>= 12;
    a.parts.exp = exp;
    return a;
}

double safe_double_multiplication_with_rounding(dbits a, dbits b, error* err){
    if(!a.d | !b.d){ return 0; } // check whether or not one of argument is equal to 0
    if((a.parts.exp > MAX_NORM_DOUBLE_EXP) && a.parts.mantissa){ return a.d; }// check whether or not a is NaN
    if((b.parts.exp > MAX_NORM_DOUBLE_EXP) && b.parts.mantissa){ return b.d; }// check whether or not b is NaN
    dbits result = safe_double_mantissa_multiplication_with_rounding(a, b, err);
    int exponent = a.parts.exp + b.parts.exp + result.parts.exp - DOUBLE_EXP_BIAS;
    *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW, ternary(exponent < 0, UNDERFLOW, *err)); if(*err){ return result.d; }    // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
    result.parts.exp = exponent;
    result.parts.sign = a.parts.sign ^ b.parts.sign;
    return result.d;
}

unsigned int random_uint(unsigned int v, unsigned int u){
    v = 36969*(v & 65535) + (v >> 16);
    u = 18000*(u & 65535) + (u >> 16);
    return (v << 16) + (u & 65535);
}

void d_mul_test(void){
    dbits a, b, result;
    error err = NO_ERROR;
    srand(899);
    unsigned int sizeof_int_in_bits = sizeof(int) << 3;
    for(int i = 0; i < 1000; ++i){
        a.luint  = ((long unsigned int)rand() << sizeof_int_in_bits) | rand();
        b.luint  = ((long unsigned int)rand() << sizeof_int_in_bits) | rand();
        result.d = safe_double_multiplication_with_rounding(a, b, &err);
        printf("%d::: %g   * \t %g = \t %g\n %064lb * %064lb = ", i, a.d, b.d , result.d, a.d, b.d);
        printf("%064lb \nerror: %u\n", result.luint, err);
        
        err = NO_ERROR;  
    }
    a.luint = 0b00000000111111111111000101010110010010001011000011010001110111111001100;
    printf("nan? %d\n", isnormal(a.d));

    a.d = fmod(4.7, 2.2);
    printf("%f\n", a.d);
}


/*
 nan   *         3.32825e-294 =          9.22337e+18
 0 00000001111 11111111000101010110010010001011000011010001110111111001100 * 10101011110111001110110101100100101000010001000 = 11111110011011100100111001000001001000000111100
*/