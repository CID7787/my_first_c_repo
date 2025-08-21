#ifndef headerfile
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "bitwise_functions.c"
#endif

// -- find inconsistencies in luint and uint multiplication and their use:
// some return OVERFLOWed value, some abort operation on OVERFLOW, different thing rely on different behaviors
//                      - safe_luint_multiplication relies on the latter safe_double_multiplication relies on the former




long unsigned int safe_luint_multiplication(long unsigned int a, long unsigned int b, error* err){
  long unsigned int product = 0;
  while(b-- > 0){
    if(*err){ return product; }
    product = safe_luint_addition(product, a, err); 
  }
  return product;
}
#ifndef headerfile

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

  dbits safe_double_mantissa_multiplication_with_rounding(dbits a, dbits b){
    a.luint = DOUBLE_MANTISSA_HIDDEN_ONE | a.parts.mantissa;
    b.luint = DOUBLE_MANTISSA_HIDDEN_ONE | b.parts.mantissa;
    lluint result = long_mantissa_multiplication(a.luint, b.luint);
    unsigned int exp = (result.high & (1ul << 41));
    a.luint = (result.high << (23 - exp)) | result.low >> (41 + exp);
    a.luint >>= 12;
    a.parts.exp = exp;
    return a;
  }
#endif

double safe_double_multiplication_with_rounding(dbits a, dbits b, error* err){
  if(!a.d | !b.d){ return 0; } // check whether or not one of argument is equal to 0
  if((a.parts.exp > MAX_NORM_DOUBLE_EXP) && a.parts.mantissa){ return a.d; }// check whether or not a is NaN
  if((b.parts.exp > MAX_NORM_DOUBLE_EXP) && b.parts.mantissa){ return b.d; }// check whether or not b is NaN
  if(!err){ return a.d; }
  dbits result = safe_double_mantissa_multiplication_with_rounding(a, b);
  int exponent = a.parts.exp + b.parts.exp + result.parts.exp - DOUBLE_EXP_BIAS;
  *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW, ternary(exponent < 0, UNDERFLOW, *err)); if(*err){ return result.d; }    // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  result.parts.exp = exponent;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  return result.d;
}