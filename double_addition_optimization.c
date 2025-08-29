#include <stdio.h>
#include "user_defined_datatypes.c"
#include "constants.c"
#include "logical_functions_of_decision.c"

double double_absolute_value(dbits value){
  value.bits.sign = 0;
  return value.d;
}

double safe_double_addition(dbits a, dbits b, error* err){
  //checking for errors
    if(!err){ return a.d; }
    if(*err){ return a.d; }
    if((a.parts.exp > MAX_NORM_DOUBLE_EXP) & a.parts.mantissa){ *err = SNAN; return a.d; }
    if((b.parts.exp > MAX_NORM_DOUBLE_EXP) & b.parts.mantissa){ *err = SNAN; return b.d; }
    if((a.parts.exp > MAX_NORM_DOUBLE_EXP) | (b.parts.exp > MAX_NORM_DOUBLE_EXP)){ *err = QNAN; return a.d; }
    long int v3, v4 = -(double_absolute_value(a) < double_absolute_value(b));
  //moving addend with the biggest absolute value to position of 'a' argument
    a.luint ^= b.luint & v4; 
    b.luint ^= a.luint & v4; 
    a.luint ^= b.luint & v4; 
  //shifting smalest (b) so that number now represented with the same exponent 
    v4 = a.parts.exp - b.parts.exp; if (v4 > (1 + AMOUNT_OF_DOUBLE_MANTISSA_BITS)) { *err = UNDERFLOW; return a.d;}
    b.parts.sign ^= a.parts.sign; 

    v3 = (DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa) >> v4;
    printf("\ns:%lb sum:%lu diff: %lu", b.parts.sign, ((DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa) + v3), ((DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa) - v3));


    v3 = ( ((DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa) + v3) & ~(-b.parts.sign) )
       | ( ((DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa) - v3) &   -b.parts.sign );
    b.parts.exp = 0;
    v4 = !(v3 & ~(DOUBLE_MANTISSA_IMPLICIT_ONE - 1)); // v3 < DOUBLE_MANTISSA_IMPLICIT_ONE
    while(!(DOUBLE_MANTISSA_IMPLICIT_ONE & v3) & v4){ 
        v3 <<= 1; b.parts.exp += 1; 
    }
    v4 = !v4;
    b.parts.exp = (v3 >> (AMOUNT_OF_DOUBLE_MANTISSA_BITS + 1)) & -v4; 
    v3 >>= b.parts.exp & -v4; 
    // b.parts.exp = ternary(v4, v3 >> (AMOUNT_OF_DOUBLE_MANTISSA_BITS + 1), AMOUNT_OF_DOUBLE_MANTISSA_BITS - how_many_bits_until_eldest_1(v3));
    // v3 = ternary(v4, v3 << b.parts.exp, v3 >> b.parts.exp);
    a.parts.mantissa = v3;
    a.parts.exp -= b.parts.exp;
    v4 = (a.parts.exp + b.parts.exp) > MAX_NORM_DOUBLE_EXP; 
    *err = ternary(v4 & a.parts.sign & b.parts.sign, NEGATIVE_OVERFLOW, *err);
    *err = ternary(v4 & !(a.parts.sign & b.parts.sign), POSITIVE_OVERFLOW, *err);
    *err = ternary(a.parts.exp - b.parts.exp < 0, UNDERFLOW, *err); 
    return a.d;
}
/*
        s eeee mmmmmmmm
            11 10101010
        b = -1
            00 00000011
            << 52
        int b = [-1; 0] + [0; 52]  = [-1; 52]
        a -= b;
        1 -  (-1) = 2
        0000 0001 - 1111 1111
        0000 0000 - 1111 1110
        1111 1111 - 1111 1101
        0000 0010

        0000 1111 - 1111 1111
        0000 0000 - 1111 0000
        1111 1111 - 1110 1111
        0001 0000

        
        1110 1010 - 1111 1111
        0000 0000 - 0000 0101
        1111 1111 - 0000 0100 
        1111 1011 
        1000 0000 000 - 1111 1111 111
        0000 0000 000 - 0111 1111 111
        1111 1111 111 - 0111 1111 110
        1000 0000 001f 
        
    */