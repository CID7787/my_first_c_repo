#ifndef headerfile
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "bitwise_functions.c"
#endif

// -- find inconsistencies in luint and uint multiplication and their use:
//      some return OVERFLOWed value, some abort operation on OVERFLOW, different thing rely on different behaviors - safe_luint_multiplication relies on the latter safe_double_multiplication relies on the former



unsigned int safe_uint_multiplication(unsigned int a, unsigned int b, error* err){
  unsigned int result = 0;
  while(b-- > 0){
    result = safe_uint_addition(result, a, err);
  }
  return result;
}


long unsigned int safe_luint_multiplication(long unsigned int a, long unsigned int b, error* err){
  long unsigned int product = 0;
  while(b-- > 0){
    if(*err){ return product; }
    product = safe_luint_addition(product, a, err); 
  }
  return product;
}