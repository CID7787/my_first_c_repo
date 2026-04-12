#ifndef headerfile
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "additional_functions.c"
  #include "bitwise_functions.c"
#endif


// FUNCTION: f_mod(double and dbits)

double my_fmod(dbits x, dbits y, error* err){
    if(*err){ return x.d; }
    unsigned char x_overfl_cond = x.parts.exp > MAX_NORM_DOUBLE_EXP, y_overfl_cond = y.parts.exp > MAX_NORM_DOUBLE_EXP;
    *err = ternary(x_overfl_cond, ternary(x.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
    *err = ternary(y_overfl_cond, ternary(y.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
    *err = ternary(x_overfl_cond && x.parts.mantissa, QNAN, *err);// check for NaN
    *err = ternary(y_overfl_cond && y.parts.mantissa, QNAN, *err);// check for NaN
    *err = ternary(!y.d, DIVISION_BY_ZERO, *err);
    y.bits.sign = 0; // we set the sign bit of y to 0
    dbits ux = x, uy = y; // we copy x and y to new variables, which will become unsigned
    ux.bits.sign = 0; // we set ux sign to 0, and uy already has its sign set to 0
    while(uy.d <= ux.d){ uy.d += y.d; } // we are exiting as soon as uy > ux.  (uy - ux) shows us how much we overflow 
    y.d -= uy.d - ux.d; // we subtract (uy - ux) from y to find a remainder
    x.bits.positive = y.bits.positive; // here we set only positive x, so that we preserve the sign of x
    return x.d;
}


// FUNCTION: char_addition(char, error*)

int8_t int8_add(int8_t a, int8_t b, error* err){
  if(!err){ return a; }
  *err = ternary((a < 0) & (a < MIN_INT8 - b), NEGATIVE_OVERFLOW, *err);
  *err = ternary((a > 0) & (a > MAX_INT8 - b), POSITIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: char_multiplication(char, error*)

int8_t int8_mult(int8_t a, int8_t b, error* err){
  if(!err){ return a; }  
  int8_t result = 0, sign = b < 0;
  *err = ternary(b == MIN_INT8, ternary(a < 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), *err);
  b = ternary(sign, int64_neg(b), b); 
  while(b-- > 0){ result = int8_add(result, a, err); }
  return ternary(sign, int64_neg(result), result);
}


// FUNCTION: integer_addition(int, error*)

int32_t int32_add(int32_t a, int32_t b, error* err){
  if(!err){ return a; }  
  *err = ternary((a > 0) && (b > (MAX_INT32 - a)), POSITIVE_OVERFLOW, *err);
  *err = ternary((a < 0) && (b < (MIN_INT32 - a)), NEGATIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: integer_multiplication(int, error*)

int32_t int32_mult(int32_t a, int32_t b, error* err){
  if(!err){ return a; }  
  int32_t result = 0, sign = b < 0;
  *err = ternary(b == MIN_INT32, ternary(a < 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), *err);
  b = ternary(sign, int64_neg(b), b);
  while(b-- > 0){ result = int32_add(result, a, err); }
  return ternary(sign, int64_neg(result), result);
}


// FUNCTION: long_int_addition(long int, long int, eror*)

int64_t int64_add(int64_t a, int64_t b, error* err){
  if(!err){ return a; }  
  *err = ternary((a > 0) & (a > (MAX_INT64 - b)), POSITIVE_OVERFLOW, *err);
  *err = ternary((a < 0) & (a < (MIN_INT64 - b)), NEGATIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: long_int_multiplication(long int, error*)

int64_t int64_mult(int64_t a, int64_t b, error* err){
  if(!err){ return a; }  
  int64_t result = 0, sign = b < 0;
  *err = ternary(b == MIN_INT64, ternary(a < 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), *err);
  b = ternary(sign, int64_neg(b), b);
  while(b-- > 0){ result = int64_add(result, a, err); }
  return ternary(sign, int64_neg(result), result);
}


// FUNCTION: long int and long unsigned int addition(int64_t, uint64_t, error*)

int64_t int64_uint64_add(int64_t a, uint64_t b, error* err){
  if(!err){ return a; }
  *err = ternary((MAX_INT64 - a) < b, POSITIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: long unsigned int and long int addition(uint64_t, int64_t, error*)

uint64_t uint64_int64_add(uint64_t a, int64_t b, error* err){
  if(!err){ return a; }
  *err = ternary((MAX_UINT64 - a) < b, POSITIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: long int and double addition(int64_t, double, error*)

int64_t int64_double_add(int64_t a, dbits b, error* err){
  if(!err){ return a; }
  *err = ternary(b.parts.exp > MAX_NORM_DOUBLE_EXP, QNAN, *err);    
  *err = ternary((*err == QNAN) && b.parts.mantissa, SNAN, *err);
  *err = ternary(!b.parts.exp && b.parts.mantissa, UNDERFLOW, *err);
  if((*err == QNAN) | (*err == SNAN) | (*err == UNDERFLOW)){ return a; }
  *err = ternary((MAX_INT64 - (a * (a > 0))) < b.d, POSITIVE_OVERFLOW, *err);
  *err = ternary((MIN_INT64 - (a * (a < 0))) > b.d, NEGATIVE_OVERFLOW, *err);
  return (a + b.d) * !(*err);
}


// FUNCTION: long unsigned int and double addition(uint64_t, double, error*)

uint64_t uint64_double_add(uint64_t a, dbits b, error* err){
  if(!err){ return a; }
  *err = ternary(b.parts.exp > MAX_NORM_DOUBLE_EXP, QNAN, *err);    
  *err = ternary((*err == QNAN) && b.parts.mantissa, SNAN, *err);
  *err = ternary(!b.parts.exp && b.parts.mantissa, UNDERFLOW, *err);
  if((*err == QNAN) | (*err == SNAN) | (*err == UNDERFLOW)){ return a; }
  *err = ternary(b.d > MAX_UINT64, POSITIVE_OVERFLOW, *err);
  *err = ternary((double_abs((dbits){ .d = b.d }) > a) & (b.d < 0), NEGATIVE_OVERFLOW, *err);
  return (a + b.d) * !(*err);    
}


// FUNCTION: unsigned_char_addition(unsigned char, error*)

uint8_t uint8_add(uint8_t a, uint8_t b, error* err){
  if(!err){ return a; }  
  *err = ternary(a > MIN_INT8 - b, POSITIVE_OVERFLOW, *err);
  return a + b; 
}


// FUNCTION: unsigned_char_multiplication(unsigned char, error*)

uint8_t uint8_mult(uint8_t a, uint8_t b, error* err){
  uint8_t result = 0;
  while(b-- > 0){ result = uint8_add(result, a, err); }
  return result;
}


// FUNCTION: unsigned_int_addition(unsigned int, error*)

uint32_t uint32_add(uint32_t a, uint32_t b, error* err){
  if(!err){ return a; }  
  *err = ternary(b > (MAX_UINT32 - a), POSITIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: unsigned_int_multiplication(unsigned int, error*)

uint32_t uint32_mult(uint32_t arg1, uint32_t arg2, error* err){
  uint32_t result = 0;
  while(arg2-- > 0){ result = uint32_add(result, arg1, err); }
  return result;
}


// FUNCTION: long_unsigned_int_addition(long unsigned int, error*)

uint64_t uint64_add(uint64_t a, uint64_t b, error* err){
  if(!err){ return a; }  
  *err = ternary(a > (MAX_UINT64 - b), POSITIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: long_unsigned_int_multiplication(long unsigned int, error*)

uint64_t uint64_mult(uint64_t a, uint64_t b, error* err){
  uint64_t result = 0;
  while(b-- > 0){ result = uint64_add(result, a, err); }
  return result;
}
  

// FUNCTION: float_addition(fbits, error*)
  
float float_add(fbits a, fbits b, error* err){
  //checking for errors
    if(*err){ return a.f; }  
    *err = ternary((a.parts.exp > MAX_NORM_FLOAT_EXP) | (b.parts.exp > MAX_NORM_FLOAT_EXP), QNAN, *err);
    *err = ternary( (*err == QNAN) & (a.parts.mantissa | b.parts.mantissa), SNAN, *err);
    *err = ternary((!a.parts.exp && a.parts.mantissa) | (!b.parts.exp && b.parts.mantissa), UNDERFLOW, *err);
    unsigned int v3, v4 = -(double_abs((dbits){ .d = a.f}) < double_abs((dbits){ .d = b.f}));
  //moving addend with the biggest absolute value to position of 'a' argument
    a.uint ^= b.uint & v4; 
    b.uint ^= a.uint & v4; 
    a.uint ^= b.uint & v4; 
  //shifting smalest (b) so that number now represented with the same exponent 
    b.parts.exp = a.parts.exp - b.parts.exp; 
    if (b.parts.exp > (1 + AMOUNT_OF_FLOAT_MANTISSA_BITS)) { *err = UNDERFLOW; return a.f; }
    b.parts.sign ^= a.parts.sign; // if a.sign != b.sign
    v3 =    (FLOAT_MANTISSA_IMPLICIT_ONE | a.parts.mantissa)
       + ( ((FLOAT_MANTISSA_IMPLICIT_ONE | b.parts.mantissa) >> b.parts.exp) ^ -b.parts.sign) + b.parts.sign;
    b.parts.exp = -!!(v3 & (FLOAT_MANTISSA_IMPLICIT_ONE << 1)); // 1 if the result of the sum overflows the normalized mantissa value, else 0
    v3 >>= -b.parts.exp; // renormalization from bigger than normal number
    while(!(v3 & FLOAT_SIGN_AND_EXP_MASK)){ // v3 < FLOAT_MANTISSA_IMPLICIT_ONE
        v3 <<= 1; ++b.parts.exp;  // renormalization from smaller than normal number
    }
    a.parts.mantissa = v3;
    a.parts.exp -= b.parts.exp;
    v4 = (a.parts.exp + b.parts.exp) > MAX_NORM_FLOAT_EXP; 
    *err = ternary(v4 & a.parts.sign & b.parts.sign, NEGATIVE_OVERFLOW, *err);
    *err = ternary(v4 & !(a.parts.sign & b.parts.sign), POSITIVE_OVERFLOW, *err);
    *err = ternary(a.parts.exp - b.parts.exp < 0, UNDERFLOW, *err); 
    return a.f;
}


// FUNCTION: float_multiplication_with_rounding(fbits, error*)

fbits safe_float_mantissa_multiplication_with_rounding(fbits a, fbits b){
  a.uint = FLOAT_MANTISSA_IMPLICIT_ONE | a.parts.mantissa;
  b.uint = FLOAT_MANTISSA_IMPLICIT_ONE | b.parts.mantissa;
  error err = NO_ERROR;
  long unsigned int result = uint64_mult(a.uint, b.uint, &err);
  b.parts.exp = result >> 47;
  a.uint = result >> (23 + b.parts.exp);
  a.parts.exp = b.parts.exp;
  return a; 
}  

float float_mult_round(fbits a, fbits b, error* err){
  if(!err){ return a.f; }  
  *err = ternary((a.parts.exp > MAX_NORM_FLOAT_EXP) | (b.parts.exp > MAX_NORM_FLOAT_EXP), QNAN, *err);
  *err = ternary( (*err == QNAN) & (a.parts.mantissa | b.parts.mantissa), SNAN, *err);
  *err = ternary((!a.parts.exp && a.parts.mantissa) | (!b.parts.exp && b.parts.mantissa), UNDERFLOW, *err);
  fbits result = safe_float_mantissa_multiplication_with_rounding(a, b);
  int exponent = a.parts.exp + b.parts.exp + result.parts.exp - FLOAT_EXP_BIAS;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  *err = ternary(exponent < 0, UNDERFLOW, *err);
  *err = ternary(exponent > MAX_NORM_FLOAT_EXP, ternary(result.parts.sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
  result.parts.exp = exponent;
  return result.f;
}  


// FUNCTION: double_division(dbits, dbits, error*)
  
float float_div_round(fbits a, fbits b, error* err){// it's just termenant substitution
  if(!err){ return a.f; }  
  *err = ternary((a.parts.exp > MAX_NORM_FLOAT_EXP) & a.parts.mantissa, SNAN, *err);
  *err = ternary((b.parts.exp > MAX_NORM_FLOAT_EXP) & b.parts.mantissa, SNAN, *err);
  *err = ternary(!a.parts.exp && a.parts.mantissa, UNDERFLOW, *err);
  *err = ternary(!b.parts.exp && b.parts.mantissa, UNDERFLOW, *err);
  *err = ternary((a.parts.exp > MAX_NORM_FLOAT_EXP) | (b.parts.exp > MAX_NORM_FLOAT_EXP), QNAN, *err);
  *err = ternary(b.f, *err, DIVISION_BY_ZERO);
  b.f += else0(!b.f, 1);
  return a.f / b.f;
}


// FUNCTION: double_addition(dbits, error*)

double double_add(dbits a, dbits b, error* err){
  //checking for errors
    if(*err){ return a.d; }
    *err = ternary((a.parts.exp > MAX_NORM_DOUBLE_EXP) | (b.parts.exp > MAX_NORM_DOUBLE_EXP), QNAN, *err);    
    *err = ternary((*err == QNAN) & (a.parts.mantissa | b.parts.mantissa), SNAN, *err);
    *err = ternary((!a.parts.exp && a.parts.mantissa) | (!b.parts.exp && b.parts.mantissa), UNDERFLOW, *err);
    long unsigned int v3, v4 = -(double_abs(a) < double_abs(b));
  //moving addend with the biggest absolute value to position of 'a' argument  
    a.luint ^= b.luint & v4; 
    b.luint ^= a.luint & v4; 
    a.luint ^= b.luint & v4; 
  //shifting smalest (b) so that number now represented with the same exponent   
    b.parts.sign ^= a.parts.sign; // if a.sign != b.sign
    v3 =    (DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa)
       + ( ((DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa) >> b.parts.exp) ^ -b.parts.sign) + b.parts.sign;
    b.parts.exp = -!!(v3 & (DOUBLE_MANTISSA_IMPLICIT_ONE << 1)); // 1 if the result of the sum overflows the normalized mantissa value, else 0   
    v3 >>= -b.parts.exp; // renormalization from bigger than normal number
    while(!(v3 & DOUBLE_SIGN_AND_EXP_MASK)){ // v3 < DOUBLE_MANTISSA_IMPLICIT_ONE
        v3 <<= 1; ++b.parts.exp;  // renormalization from smaller than normal number
    }    
    a.parts.mantissa = v3;
    a.parts.exp -= b.parts.exp;
    v4 = (a.parts.exp + b.parts.exp) > MAX_NORM_DOUBLE_EXP; 
    *err = ternary(v4 & a.parts.sign & b.parts.sign, NEGATIVE_OVERFLOW, *err);
    *err = ternary(v4 & !(a.parts.sign & b.parts.sign), POSITIVE_OVERFLOW, *err);
    *err = ternary(a.parts.exp - b.parts.exp < 0, UNDERFLOW, *err); 
    return a.d;
}    


// FUNCTION: double_multiplication_without_rounding(dbits, dbits, error*)

dbits safe_double_mantissa_multiplication_without_rouding(dbits a, dbits b, error* err){
  if(!err){ return a; }  
  unsigned int a_exp = b.parts.exp, b_exp = b.parts.exp;
  a.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa;
  b.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa;
  // remove useless zeros
  while (!(a.luint & 1ul)) { a.luint >>= 1; }
  while (!(b.luint & 1ul)) { b.luint >>= 1; }
  // variable declaration
  int bin_point_shift = int32_add(how_many_bits_until_eldest_1(a.luint), how_many_bits_until_eldest_1(b.luint), err);
  b.luint = uint64_mult(a.luint, b.luint, err); 
  *err = ternary(b.luint > MAX_DOUBLE_MANTISSA, POSITIVE_OVERFLOW, *err);
  bin_point_shift = int32_add(how_many_bits_until_eldest_1(b.luint), -bin_point_shift, err); 
  // moving product's first one to hidden one position
  b.luint <<= int32_add(AMOUNT_OF_DOUBLE_MANTISSA_BITS, -how_many_bits_until_eldest_1(b.luint), err); 
  b.parts.exp = bin_point_shift;
  return b;
}  

double double_mult_no_round(dbits a, dbits b, error* err){
  if(!err){ return a.d; }  
  *err = ternary((a.parts.exp > MAX_NORM_DOUBLE_EXP) | (b.parts.exp > MAX_NORM_DOUBLE_EXP), QNAN, *err);    
  *err = ternary((*err == QNAN) & (a.parts.mantissa | b.parts.mantissa), SNAN, *err);
  *err = ternary((!a.parts.exp && a.parts.mantissa) | (!b.parts.exp && b.parts.mantissa), UNDERFLOW, *err);
  dbits result = safe_double_mantissa_multiplication_without_rouding(a, b, err);
  unsigned int exponent = uint32_add(a.parts.exp, b.parts.exp, err); 
  exponent = uint32_add(exponent, result.parts.exp, err); 
  exponent = int32_add(exponent, -DOUBLE_EXP_BIAS, err); 
  // check whether of not exponent value bigger than
  *err = else0(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(!(exponent > MAX_NORM_DOUBLE_EXP), *err);
  result.parts.exp = exponent;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  return result.d;
}  


// FUNCTION: double_multiplication_with_rounding(dbits,dbits,error*)

lluint long_mantissa_multiplication(long unsigned int a, long unsigned int b){ 	
  // getting left and right parts of multiplicand and multiplier 
  unsigned int sizeof_half_arg_type_bits = sizeof(a) << 2; //(sizeof(a) << 3) >> 1 == sizeof_type_in_bits(sizeof(a) * 8(2^3)) / 2 == sizeof(a) * 4 
  error err = NO_ERROR;
  long unsigned int a_l = a >> sizeof_half_arg_type_bits,
                    b_l = b >> sizeof_half_arg_type_bits,
                    a_r = a & MAX_UINT32,
                    b_r = b & MAX_UINT32;
  long unsigned int middle = (a_l * b_r) + (a_r * b_l);
  lluint result = {.high = (a_l * b_l) + (middle >> sizeof_half_arg_type_bits),
                   .low  = uint64_add(a_r * b_r, middle << sizeof_half_arg_type_bits, &err) };
  result.high += else0(err, 1);                 
  return result;
}

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

dbits safe_double_mantissa_multiplication_with_rounding(dbits a, dbits b){
  a.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa;
  b.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa;
  lluint result = long_mantissa_multiplication(a.luint, b.luint);
  b.parts.exp = result.high >> 41;// TODO: OPTIMIZE
  a.luint = (result.high << (23 - b.parts.exp)) | result.low >> (41 + b.parts.exp);// TODO: OPTIMIZE
  a.luint >>= 11;
  a.parts.exp = b.parts.exp;
  return a;
}  

double double_mult_round(dbits a, dbits b, error* err){
  if(!err){ return a.d; }  
  *err = ternary((a.parts.exp > MAX_NORM_DOUBLE_EXP) | (b.parts.exp > MAX_NORM_DOUBLE_EXP), QNAN, *err);    
  *err = ternary((*err == QNAN) & (a.parts.mantissa | b.parts.mantissa), SNAN, *err);
  *err = ternary((!a.parts.exp && a.parts.mantissa) | (!b.parts.exp && b.parts.mantissa), UNDERFLOW, *err);
  dbits result = safe_double_mantissa_multiplication_with_rounding(a, b);
  int exponent = a.parts.exp + b.parts.exp + result.parts.exp - DOUBLE_EXP_BIAS;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  *err = ternary(exponent < 0, UNDERFLOW, *err);
  *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, ternary(result.parts.sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
  result.parts.exp = exponent;
  return result.d;
}  


// FUNCTION: double_division(dbits, dbits, error*)
  
double double_div_round(dbits a, dbits b, error* err){// quotient, remainder
  if(!err){ return a.d; }  
  *err = ternary( ((a.parts.exp > MAX_NORM_DOUBLE_EXP) & a.parts.mantissa) | ((b.parts.exp > MAX_NORM_DOUBLE_EXP) & b.parts.mantissa), SNAN, *err);
  *err = ternary((!a.parts.exp && a.parts.mantissa) | (!b.parts.exp && b.parts.mantissa), UNDERFLOW, *err);
  *err = ternary((a.parts.exp > MAX_NORM_DOUBLE_EXP) | (b.parts.exp > MAX_NORM_DOUBLE_EXP), QNAN, *err);    
  *err = ternary(b.d, *err, DIVISION_BY_ZERO);
  return a.d / b.d;
}


// FUNCTION: factorial(unsigned int, error*)

#define factorial_lookup_table_size 13
const unsigned int factorial_lookup_table[factorial_lookup_table_size] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800, 39916800, 43545600};  // DESCRIPTION: factorial values list of numbers from 0 to 10

long unsigned int factorial(long unsigned int a, error* err){
  if(!err){ return a; }
  if(a < factorial_lookup_table_size){ return factorial_lookup_table[a]; }
  long unsigned int result = a;
  while(a-- > 1){
    result = uint64_mult(result, a, err);
    if(*err){ return result; }
  }    
  return result;
}    

// FUNCTION: increment(unsigned int*)

unsigned int increment_error(unsigned int * x){// TODO: how to distinguish case when argument is 0 and when it is ~0
  if(!x){ return 0; }
  unsigned int a = (*x);
  if (compare(a, ~0u)) { return 1; } // special case 111111...111111
  unsigned int c = 1;
  while(a & c){ 
    a = a ^ c;
    c = lshift(c, 1);
  }    
  (*x) = a | c;
  return 0;
}    

unsigned int increment(unsigned int* x){
  if(!x){ return 0; }
  unsigned int a = (*x); // a = 0
  
  unsigned int c = 1; // c = 1
  while(a & c){  // a & c = 0
     a = a ^ c;
     c = lshift(c, 1);
  }      
  (*x) = a | c; // i = 1;
    
  return (a | c); // return 1;
}    


// FUNCTION: addition(unsigned int, unsigned int)

unsigned int int_addition_v1(unsigned int a, unsigned int b){
  // implement addition using increment and loops -- реализовать сложение используя инкремент и циклы
  unsigned int x = 0;
  while ( x < b ) { increment(&x); increment(&a); }
  // printf ("The sum of these numbers equal to:%u\n",a);
  return a;
}    


// FUNCTION: multiplication(unsigned int, unsigned int)

unsigned int uint_multiplication(unsigned int a, unsigned int b){
  // implement multiplication using addition and loops -- реализовать умножение используя сложение и циклы
  int c = 0;
  int d = 0;
  while ( c < b ) { increment(&c); d = int_addition_v1(d, a); }
  // printf ("\n c:%d a:%d d:%d b:%d", c, a, d, b);
  return d;
}    


// FUNCTION: exponentiation(unsigned int, unsigned int)

unsigned int uint_to_uint_exponentiation(unsigned int a, unsigned int b){ // 2 , 0
  // implement exponentiation using multiplication and loops -- реализовать возведение в степень используя умножение и циклы
  int c = 0;
  int d = 1;
  while ( c < b) { increment(&c); d = uint_multiplication(d, a); }
  // printf ("\n c:%d a:%d d:%d b:%d", c, a, d, b);
  return d;
}    


// FUNCTION: unsigned_int_division_by_2

unsigned int unsigned_integer_division_by_2(unsigned int a){ // 4, 5, 8, 10
  return a >> 1;
}    


// FUNCTION: decrement(unsigned int*)

unsigned int decrement(unsigned int* x){ // TODO: function works incorrect, fix it
  if(!x){ return ~0; }
  unsigned int a = (*x);
  if(compare(a, 0)){ return ~0u; }
  unsigned int b = 1;
  while (!(a & b)){
    a = a | b;
    b = lshift(b, 1);
  }    
  (*x) = a | (b ^ (~0));
  return 0;
}    


// FUNCTION: integer_division(unsigned int, unsigned int, unsigned int* isNull)

unsigned int integer_division(unsigned int a, unsigned int b, unsigned int* isNull){
  if(!isNull){ return a; }
  if (!b)  { (*isNull) = 2; return -1; }// TODO: how to distinguish division of any number by 0 and division of ~0 by 1
  if (!a)  { return 0; }
  
  unsigned int c = 0;
  unsigned int d = 0;
  
  while (d <= (a - b)) {// TODO: what if: a - b = 1 - 3 ---> ? fix it
    a -= b;
    ++c;
  }    
  (*isNull) = 1;
  return c;    
}    


// FUNCTION: square_root(float, float)

float square_root(float value, float precision) {// TODO: negative argument handling
  unsigned int i = ~0; //  100,  0.001    50, 25, [6.25, 12.5], 6.25 + 3.125 
  float start = 0;
  float size = value;
  float center = size / 2;
  while (--i) {
    if ((center*center) > (value + precision)) { size = center; }
    if ((center*center) < (value - precision)) { start = center; }
    center = start + (size - start) / 2;
    if ( ((center*center) < (value + precision)) 
    && ((center*center) > (value - precision)) ) { return center; }
  }    
  return center;
}    


// FUNCTION: floating_division_by_2

float floating_division_by_2(float b) { // TODO: function works incorrect
  int mask1 = ((((~0) << 1) >> 1) >> 23) << 23;
  int max_int = (~0) >> 1;
  int a = *((int*)&b);
  int exp = a & mask1;
  int number_without_exp = a & (~mask1);
  if (!eldest_bit_is_1(number_without_exp)){
    if (last_bit_is_1(number_without_exp)){
      number_without_exp = uint_multiplication(number_without_exp, 5);
      exp = ((exp >> 23) - 1) << 23;
    } else {
      int just_1 = 1;
      number_without_exp = (number_without_exp >> 1); // 0 00000000 mmmmmmmm...mmmm0 >> 1
    }    
  } else {
    int mantissa = convert_to_sign_and_magnitude(number_without_exp);
    if (last_bit_is_1(mantissa)) {
      mantissa = uint_multiplication(mantissa, 5);
      exp = ((exp >> 23) - 1) << 23;
    } else {
      mantissa = mantissa >> 1;
    }    
    number_without_exp = convert_from_sign_and_magnitude(mantissa);
  }    
  number_without_exp = number_without_exp | exp;
  float result = *((float*)&number_without_exp);
  return result; //
}    


// FUNCTION: float_division(float)

float float_division(float result, float m1, float precision){ // a = 9, b = 3 c = 1
  if (!result) {return 0.0f;}
  if (!m1) {return -1.0f;}
  int m0 = (~(0b0) >> 1) & (~(0b1 << 23));
  float max = *((float*)&m0); // ~(0b100000001 << 23)
  unsigned int iterations = 0;
  float min = 0;
  int sign1 = ((*((int*)&result)) ^ (*((int*)&m1))) >> 31;// (result * m1) < 0
  result *= !(result < 0) - (result < 0);
  int array[2] = {1, -1};
  int sign2 = array[sign1];
  m1 *= ((!(m1 < 0)) - (m1 < 0));
  float m2 = max / 2;
  while (1){
    if (m1 >= 1) { // [0 ; result]
      max = result;
    }    
    if ((m1 < 1) && (m1 > 0)) { // [result; +inf]
      min = result;
    }    
    if ((m1 * m2) > (result + precision)) {max = m2;}
    if ((m1 * m2) < (result - precision)) {min = m2;}
    
    m2 = min + ((max - min)/2);
    ++iterations; 
    
    if (((m1 * m2) > (result - precision)) 
    || ((m1 * m2) < (result + precision))) {printf ("How many iterations:%d", iterations); return m2 * (float)sign2;}
  }    
  return -2.0f;
}    


// FUNCTION: double_base_to_long_unsigned_int_power(double, unsigned int, error*)

double exp_double2luint(dbits a, long unsigned int b, error* err){
  if(!err){ return a.d; }
  unsigned int cond = -(!(a.d || b));
  *err = (cond & ZERO_TO_ZERO) | (~cond & *err);
  dbits r = (dbits){ .d = 1.0 };
  while(b--){ r.d = double_mult_round(r, a, err); }
  return r.d;
}


// exponentiation of char_to_char(char, error*)

char exp_char2char(char a, char b, error* err){
  if(!err){ return a; }
  char result = 1;
  *err = else0(b < 0, UNDERFLOW);
  while(b-- && !(*err)){ result = int8_mult(result, a, err); }
  return result;
}


// exponentiation of int_to_int(int, error*)

int exp_int2int(int a, int b, error* err){
  if(!err){ return a; }
  int result = 1;
  *err = else0(b < 0, UNDERFLOW);
  while(b-- && !(*err)){ 
      result = int32_mult(result, a, err);
  }
  return result;
}

// exponentiation of unsigned_int_to_unsigned_int(unsigned int, error*)

unsigned int exp_uint2uint(unsigned int a, unsigned int b, error* err){
  if(!err){ return a; }
  unsigned int result = 1;
  while(b-- && !(*err)){
      result = uint32_mult(result, a, err);
  }
  return result;
}


// exponentiation of unsigned_char_to_unsigned_char(unsigned char, error*)

unsigned char exp_uchar2uchar(unsigned char a, unsigned char b, error* err){
  if(!err){ return a; }
  unsigned char result = 1;
  while(b-- && !(*err)){
      result = uint8_mult(result, a, err);
  }
  return result;
}


// exponentiation of long_int_to_long_int(long int, error*)

long int exp_lint2lint(long int a, long int b, error* err){
  if(!err){ return a; }
  long int result = 1;
  *err = else0(b < 0, UNDERFLOW);
  while(b-- && !(*err)){ 
      result = int64_mult(result, a, err);
  }
  return result;
}

// exponentiation of long_unsigned_int_to_long_unsigned_int(long unsigned int, error*)

long unsigned int exp_luint2luint(long unsigned int a, long unsigned int b, error* err){
  if(!err){ return a; }
  long unsigned int result = 1;
  while(b-- && !(*err)){
      result = uint64_mult(result, a, err);
  }
  return result;
}


// exponentiation of float_to_float(float, error*)

float exp_float2float(fbits a, fbits b, error* err){
  if(!err){ return a.f; }
  fbits result = (fbits){ .f = 1};
  unsigned int cond = -( have_frac_part((dbits){ .d = b.f }) ); 
  *err = (cond & ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER) | (~cond & *err);
  cond = -(!(a.f || b.f)); 
  *err = (cond & ZERO_TO_ZERO) | (-(!cond) & *err);
  if(b.f < 0){
    while(b.f++ && !(*err)){
      result.f = float_div_round(result, a, err);// TODO: WRITE FLOAT DIVISION WITH ROUNDING FUNCTION
    }
  }
  else{
      while(b.f-- && !(*err)){
      result.f = float_mult_round(result, a, err);
      }
  }
  return result.f;
}


// exponentiation of double_to_double(double, error*)

double exp_double2double(dbits a, dbits b, error* err){
  if(!err){ return a.d; }
  dbits result = (dbits){ .d = 1};
  unsigned int cond = -have_frac_part(b);
  *err = (cond & ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER) | (~cond & *err);
  cond = -(!(a.d || b.d));
  *err = (cond & ZERO_TO_ZERO) | (-(!cond) & *err);
  if(b.d < 0){ 
    while(b.d++ && !(*err)){  
      result.d = double_div_round(result, a, err);// TODO: WRITE DOUBLE DIVISION WITH ROUNDING FUNCTION
    }
  }
  else{
    while(b.d-- && !(*err)){  
      result.d = double_mult_round(result, a, err);
    }
  }
  return result.d;
}


// exponenetiation of double to long int(dbits, long int) 

double exp_double2lint(dbits a, long int b, error* err){
  if(!err){ return a.d; }
  dbits r = (dbits){ .d = 1.0 };
  unsigned int cond = -(!(a.d || b));
  *err = (cond & ZERO_TO_ZERO) | (~cond & *err);
  if(b < 0){ 
      while(b++ && !(*err)){
          r.d = double_div_round(r, a, err);// TODO: WRITE DOUBLE DIVISION WITH ROUNDING FUNCTION
      }    
  }
  else{ 
      while(b-- && !(*err)){
          r.d = double_mult_round(r, a, err);
      }
  }
  return r.d;
}


// exponentiation float to long int(fbits, long int)

float exp_float2lint(fbits a, long int b, error* err){
  if(!err){ return a.f; }
  unsigned int cond = -(!(a.f || b));
  *err = (cond & ZERO_TO_ZERO) | (~cond & *err);
  fbits result = (fbits){ .f = 1.0 };
  if(b < 0){ 
      while(b++ && !(*err)){
          a.f = float_div_round(result, a, err); // TODO: WRITE FLOAT DIVISION WITH ROUNDING FUNCTION
      }
  }
  else{
      while(b-- && !(*err)){
          a.f = float_mult_round(result, a, err);
      }
  }
  return result.f; 
}


// exponentiation float to long unsigned int(fbits, long unsigned int)

float exp_float2luint(fbits a, long unsigned int b, error* err){
  if(!err){ return a.f; }
  unsigned int cond = -(!(a.f || b));
  *err = (cond & ZERO_TO_ZERO) | (~cond & *err);
  fbits result = (fbits){ .f = 1.0 };
  while(b-- && !(*err)){
      a.f = float_mult_round(result, a, err);
  }
  return result.f; 
}


// epxonentiation long int to double(long int, dbits)

long int exp_lint2double(long int a, dbits b, error* err){
  if(!err){ return a; }
  long int r = 1;
  unsigned int cond = -have_frac_part(b);
  *err = (cond & ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER) | (~cond & *err);
  cond = -(!a & !b.d);
  *err = (cond & ZERO_TO_ZERO) | (cond & *err);
  cond = -(b.d < 0);
  *err = (cond & UNDERFLOW) | (cond & *err);
  while(b.d-- && !(*err)){
      r = int64_mult(r, a, err);
  } 
  return r;
}


// epxonentiation long unsigned int to double(long unsigned int, dbits)

long unsigned int exp_luint2double(long unsigned int a, dbits b, error* err){
  if(!err){ return a; }
  long unsigned int r = 1;
  unsigned int cond = -have_frac_part(b);
  *err = (cond & ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER) | (~cond & *err);
  cond = -(!(a || b.d));
  *err = (cond & ZERO_TO_ZERO) | (-(!cond) & *err);
  cond = -(b.d < 0);
  *err = (cond & UNDERFLOW) | (-(!cond) & *err);
  while(b.d-- && !(*err)){
      r = uint64_mult(r, a, err);
  } 
  return r;
}


// exponentiation long int to luint(long int, long unsigned int)

long int exp_lint2luint(long int a, long unsigned int b, error* err){ 
  if(!err){ return a; }
  long int result = 1;
  while(b-- && !(*err)){
    result = int64_mult(result, a, err);
  }    
  return result;
}


