#ifndef headerfile
  #include "user_defined_datatypes.c"
  #include "constants.c"
#endif
double safe_double_addition(dbits a, dbits b, error* err){
  //checking for errors
    if(!err){ return a.d; }
    if(*err){ return a.d; }
    if((a.parts.exp > MAX_NORM_DOUBLE_EXP) & a.parts.mantissa){ *err = SNAN; return a.d; }
    if((b.parts.exp > MAX_NORM_DOUBLE_EXP) & b.parts.mantissa){ *err = SNAN; return b.d; }
    if((a.parts.exp > MAX_NORM_DOUBLE_EXP) | (b.parts.exp > MAX_NORM_DOUBLE_EXP)){ *err = QNAN; return a.d; }
    long int v3, v4 = -(double_absolute_value(a.d) < double_absolute_value(b.d));
  //moving addend with the biggest absolute value to position of 'a' argument
    a.luint ^= b.luint & v4; 
    b.luint ^= a.luint & v4; 
    a.luint ^= b.luint & v4; 
  //added implicit one to mantissa representation
    v4 = DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa;
  //shifting smalest (b) so that number now represented with the same exponent 
    v4 >>= a.parts.exp - b.parts.exp; if ((a.parts.exp - b.parts.exp) > (1 + AMOUNT_OF_DOUBLE_MANTISSA_BITS)) { *err = UNDERFLOW; return a.d;}
    v3 = ((DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa) ^ -a.parts.sign) + a.parts.sign
       + ((v4) ^ -b.parts.sign) + b.parts.sign;
    v4 = *(long unsigned int*)&v3 >> ((sizeof(double) << 3) - 1);// sizeof(double) << 3 + 1 <==> AMOUNT_OF_DOUBLE_MANTISSA_BITS + AMOUNT_OF_DOUBLE_EXP_BITS 
    v3 = (v3 ^ -v4) + v4;
    v4 = v3 & DOUBLE_MANTISSA_IMPLICIT_ONE;
    b.parts.exp = ternary(v4, v3 >> (AMOUNT_OF_DOUBLE_MANTISSA_BITS + 1), AMOUNT_OF_DOUBLE_MANTISSA_BITS - how_many_bits_until_eldest_1(v3));
    v3 = ternary(v4, v3 << b.parts.exp, v3 >> b.parts.exp);
    // v4 = a.luint;
    a.parts.mantissa = v3;
  //v4 stores oveflow condition 
    v3 = (a.parts.exp + b.parts.exp) > MAX_NORM_DOUBLE_EXP; 
    *err = ternary(v3 & a.parts.sign & b.parts.sign, NEGATIVE_OVERFLOW, *err);
    *err = ternary(v3 & !(a.parts.sign & b.parts.sign), POSITIVE_OVERFLOW, *err);
    *err = ternary(a.parts.exp - b.parts.exp < 0, UNDERFLOW, *err); 
    a.parts.exp = ternary(v4, a.parts.exp + b.parts.exp, a.parts.exp - b.parts.exp);
    // b.luint = v4;
    // *err = ternary(a.d != b.d, UNDERFLOW, *err);
    return a.d;
}
