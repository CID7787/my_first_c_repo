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
    v3   =    DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa;
    v3 += ( ((DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa) >> v4) ^ -(b.parts.sign & !a.parts.sign) ) + (b.parts.sign & !a.parts.sign);
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
    a.parts.exp = ternary(v4, a.parts.exp + b.parts.exp, a.parts.exp - b.parts.exp);
    a.parts.sign ^= b.parts.sign; 
    v4 = (a.parts.exp + b.parts.exp) > MAX_NORM_DOUBLE_EXP; 
    *err = ternary(v4 & a.parts.sign & b.parts.sign, NEGATIVE_OVERFLOW, *err);
    *err = ternary(v4 & !(a.parts.sign & b.parts.sign), POSITIVE_OVERFLOW, *err);
    *err = ternary(a.parts.exp - b.parts.exp < 0, UNDERFLOW, *err); 
    return a.d;
}

int main(){

  return 0;
}