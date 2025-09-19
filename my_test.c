#include <stdio.h>

int main(){
  long unsigned int uint_nan = 0b01111111100000000000000000000000;
  float f_nan = *(float*)&uint_nan;
  double d_fnan = f_nan;
  uint_nan = *(unsigned int*)&f_nan;
  printf("\nf_nan as uint: %032b\n", uint_nan);
  uint_nan = *(unsigned int*)&d_fnan;
  printf("\nd_fnan as uint: %064lb\n", uint_nan);
  printf("\nf_nan: %f\n", f_nan);
  printf("\nd_fnan: %f\n", d_fnan);
  return 0;
}

/*
  dbits safe_double_mantissa_multiplication_with_rounding_t(dbits a, dbits b){
  a.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa;
  b.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa;
  lluint result = long_mantissa_multiplication(a.luint, b.luint);
  // b.parts.exp = (result.high & (1ul << 41));// TODO: OPTIMIZE
  b.parts.exp = result.high >> 41;// TODO: OPTIMIZE
  a.luint = (result.high << (23 - b.parts.exp)) | result.low >> (41 + b.parts.exp);// TODO: OPTIMIZE
  a.luint >>= 12;// TODO: I GUESS IT SHOULD BE SHIFTED 11 TIMES
  a.parts.exp = b.parts.exp;
  return a;
}  

double safe_double_multiplication_with_rounding_test_v(dbits a, dbits b, error* err){
  if(!err){ return a.d; }// check whether or not is NULL pointer
  unsigned char a_overfl_cond = a.parts.exp > MAX_NORM_DOUBLE_EXP, b_overfl_cond = b.parts.exp > MAX_NORM_DOUBLE_EXP;
  *err = ternary(a_overfl_cond, ternary(a.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(b_overfl_cond, ternary(b.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(a_overfl_cond && a.parts.mantissa, QNAN, *err);// check for NaN
  *err = ternary(b_overfl_cond && b.parts.mantissa, QNAN, *err);// check for NaN
  if(!a.d | !b.d){ return 0; }// check whether or not one of argument is equal to 0
  dbits result = safe_double_mantissa_multiplication_with_rounding_t(a, b);
  int exponent = a.parts.exp + b.parts.exp + result.parts.exp - DOUBLE_EXP_BIAS;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  *err = ternary(exponent < 0, UNDERFLOW, *err);
  *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, ternary(result.parts.sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
  result.parts.exp = exponent;
  return result.d;
}
*/