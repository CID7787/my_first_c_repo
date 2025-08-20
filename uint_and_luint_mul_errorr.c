#ifndef headerfile
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "bitwise_functions.c"
#endif

-- find inconsistencies in luint and uint multiplication and their use:
some return OVERFLOWed value, some abort operation on OVERFLOW, different thing rely on different behaviors
                     - safe_luint_multiplication relies on the latter safe_double_multiplication relies on the former




long unsigned int safe_luint_multiplication(long unsigned int a, long unsigned int b, error* err){
  long unsigned int product = 0;
  while(b-- > 0){
    if(*err){ return product; }
    product = safe_luint_addition(product, a, err); 
  }
  return product;
}


double safe_double_multiplication_with_rounding(dbits a, dbits b, error* err){
  if(!a.d | !b.d){ return 0; }
  char a_nan_cond = (a.parts.exp > MAX_NORM_DOUBLE_EXP) && a.parts.mantissa;
  char b_nan_cond = (b.parts.exp > MAX_NORM_DOUBLE_EXP) && b.parts.mantissa;
  if(a_nan_cond | b_nan_cond){ return ternary(a_nan_cond, a.d, b.d); }
  dbits result = safe_double_mantissa_multiplication_with_rounding(a, b, err); 
  int exponent = a.parts.exp + b.parts.exp + result.parts.exp - DOUBLE_EXP_BIAS;
  // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW, ternary(exponent < 0, UNDERFLOW, *err)); if(*err){ return result.d; }
  result.parts.exp = exponent;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  return result.d;
}