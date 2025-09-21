#ifndef headerfile
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "bitwise_functions.c"
#endif


// FUNCTION: f_mod(double and dbits)

double my_fmod(dbits x, dbits y, error* err){
    if(!err){ return x.d; }
    unsigned char x_overfl_cond = x.parts.exp > MAX_NORM_DOUBLE_EXP, y_overfl_cond = y.parts.exp > MAX_NORM_DOUBLE_EXP;
    *err = ternary(x_overfl_cond, ternary(x.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
    *err = ternary(y_overfl_cond, ternary(y.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
    *err = ternary(x_overfl_cond && x.parts.mantissa, QNAN, *err);// check for NaN
    *err = ternary(y_overfl_cond && y.parts.mantissa, QNAN, *err);// check for NaN
    *err = ternary(!y.d, DIVISION_BY_ZERO, *err);
    y.bits.sign = 0; // we set the sign bit of y to 0
    dbits ux = x, uy = y; // we copy x and y to new variables, which will become unsigned
    ux.bits.sign = 0; // we set ux sign to 0, and uy already has its sign set to 0
    while((uy.d <= ux.d) && !(*err)){ uy.d += y.d; } // we are exiting as soon as uy > ux.  (uy - ux) shows us how much we overflow 
    y.d -= uy.d - ux.d; // we subtract (uy - ux) from y to find a remainder
    x.bits.positive = y.bits.positive; // here we set only positive x, so that we preserve the sign of x
    return x.d;
}

// FUNCTION: char_addition(char, error*)

char safe_char_addition(char a, char b, error* err){
  *err = ternary((a < 0) & (a < (int)MIN_CHAR - (int)b), NEGATIVE_OVERFLOW, *err);
  *err = ternary((a > 0) & (a > (int)MAX_CHAR - (int)b), POSITIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: char_multiplication(char, error*)

char safe_char_multiplication(char a, char b, error* err){
  if(!err){ return a; }
  char sign = (a ^ b) & MIN_CHAR;
  a &= MAX_CHAR;
  b &= MAX_CHAR; 
  char result = 0;
  while(b-- && !(*err)){
    result = safe_char_addition(result, a, err);
  }
  *err = ternary((*err == POSITIVE_OVERFLOW) && sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW);
  result |= sign; 
  return result;
}


// FUNCTION: unsigned_char_addition(unsigned char, error*)

unsigned char safe_uchar_addition(unsigned char a, unsigned char b, error* err){
  *err = ternary(a > MIN_CHAR - b, POSITIVE_OVERFLOW, *err);
  return a + b; 
}


// FUNCTION: unsigned_char_multiplication(unsigned char, error*)

unsigned char safe_uchar_multiplication(unsigned char a, unsigned char b, error* err){
  if(!err){ return a; }
  char result = 0;
  while(b-- && !(*err)){
      result = safe_uchar_addition(result, a, err);
  }
  return result;
}


// FUNCTION: integer_addition(int, error*)

int safe_int_addition(int a, int b, error* err){
  if(!err){ return a; }
  *err = ternary((a > 0) && (b > ((long int)MAX_INT - (long int)a)), POSITIVE_OVERFLOW, *err);
  *err = ternary((a < 0) && (b < ((long int)MIN_INT - (long int)a)), NEGATIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: integer_multiplication(int, error*)

int safe_int_multiplication(int a, int b, error* err){
  if(!err){ return a; }
  int result = 0, sign = (a ^ b) & MIN_INT;
  a &= MAX_INT;
  b &= MAX_INT; 
  while(b-- && !(*err)){
    result = safe_int_addition(result, a, err);
  }
  *err = ternary((*err == POSITIVE_OVERFLOW) && sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW);
  result |= sign; 
  return result;
}


// FUNCTION: unsigned_int_addition(unsigned int, error*)

unsigned int safe_uint_addition(unsigned int arg1, unsigned int arg2, error* err){
  if(!err){ return arg1; }
  *err = ternary(arg2 > (MAX_UINT - arg1), POSITIVE_OVERFLOW, *err);
  return arg1 + arg2;
}


// FUNCTION: unsigned_int_multiplication(unsigned int, error*)

unsigned int safe_uint_multiplication(unsigned int arg1, unsigned int arg2, error* err){
  if(!err){ return arg1; }
  unsigned int result = 0;
  while(arg2-- && !(*err)){
    result = safe_uint_addition(result, arg1, err);
  }
  return result;
}


// FUNCTION: long_int_addition(long int, long int, eror*)

long int safe_lint_addition(long int a, long int b, error* err){
  if(!err){ return a; }
  *err = ternary((a > 0) & (a > (MAX_LINT - b)), POSITIVE_OVERFLOW, *err);
  *err = ternary((a < 0) & (a < (MIN_LINT - b)), NEGATIVE_OVERFLOW, *err);
  return a + b;
}


// FUNCTION: long_int_multiplication(long int, error*)

long int safe_lint_multiplication(long int a, long int b, error* err){
  if(!err){ return a; }
  long int sign = (a ^ b) & MIN_LINT;
  a &= MAX_LINT;
  a &= MAX_LINT;
  long int result = 0;
  while(b--){
      if(*err){ *err = ternary((*err == POSITIVE_OVERFLOW) && sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW); return a; }
      result = safe_lint_addition(result, a, err);
  }
  result |= sign;
  return result;
}


// FUNCTION: long_unsigned_int_addition(long unsigned int, error*)

long unsigned int safe_luint_addition(long unsigned int addend1, long unsigned int addend2, error* err){
  if(!err){ return addend1; }
  char overflow_cond = -( -(addend2 > (MAX_LUINT - addend1) ) );
  *err = (overflow_cond & POSITIVE_OVERFLOW) | (overflow_cond & *err);
  return addend1 + addend2;
}

long unsigned int safe_luint_addition_optim_v_prot(long unsigned int addend1, long unsigned int addend2, error* err){
  if(!err){ return addend1; }
  unsigned int r_leftmost8b;


  return addend1 + addend2;
}

// FUNCTION: long_unsigned_int_multiplication(long unsigned int, error*)

long unsigned int safe_luint_multiplication(long unsigned int a, long unsigned int b, error* err){
  if(!err){ return a; }
  long unsigned int result = 0;
  while(b--){
      if(*err){ return a; }
      result = safe_luint_addition(result, a, err);
  }
  return result;
}


long unsigned int safe_luint_multiplication_optim_v_prot(long unsigned int a, long unsigned int b, error* err){
  if(!err){ return a; }
  long unsigned int result;
  

  return result;
}


// FUNCTION: double_absolute_value(double)

double double_absolute_value(dbits value){
  value.bits.sign = 0;
  return value.d;
}
  

// FUNCTION: float_addition(fbits, error*)
  
float safe_float_addition(fbits a, fbits b, error* err){
  //checking for errors
    if(!err){ return a.f; }
    if(*err){ return a.f; }
    if((a.parts.exp > MAX_NORM_FLOAT_EXP) & a.parts.mantissa){ *err = SNAN; return a.f; }
    if((b.parts.exp > MAX_NORM_FLOAT_EXP) & b.parts.mantissa){ *err = SNAN; return b.f; }
    if((a.parts.exp > MAX_NORM_FLOAT_EXP) | (b.parts.exp > MAX_NORM_FLOAT_EXP)){ *err = QNAN; return a.f; }
    unsigned int v3, v4 = -(double_absolute_value((dbits){ .d = a.f}) < double_absolute_value((dbits){ .d = a.f}));
  //moving addend with the biggest absolute value to position of 'a' argument
    a.uint ^= b.uint & v4; 
    b.uint ^= a.uint & v4; 
    a.uint ^= b.uint & v4; 
  //shifting smalest (b) so that number now represented with the same exponent 
    b.parts.exp = a.parts.exp - b.parts.exp; if (b.parts.exp > (1 + AMOUNT_OF_FLOAT_MANTISSA_BITS)) { *err = UNDERFLOW; return a.f; }
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
  long unsigned int result = safe_luint_multiplication(a.uint, b.uint, &err);
  b.parts.exp = result >> 47;
  a.uint = result >> (23 + b.parts.exp);
  a.parts.exp = b.parts.exp;
  return a; 
}  

float safe_float_multiplication_with_rounding(fbits a, fbits b, error* err){
  if(!err){ return a.f; }//check for NULL pointer
  unsigned char a_overfl_cond = a.parts.exp > MAX_NORM_FLOAT_EXP, b_overfl_cond = b.parts.exp > MAX_NORM_FLOAT_EXP;
  *err = ternary(a_overfl_cond, ternary(a.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(b_overfl_cond, ternary(b.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(a_overfl_cond && a.parts.mantissa, QNAN, *err);// check for NaN
  *err = ternary(b_overfl_cond && b.parts.mantissa, QNAN, *err);// check for NaN
  if(!a.f | !b.f){ return 0; } // check whether or not one of argument is equal to 0
  fbits result = safe_float_mantissa_multiplication_with_rounding(a, b);
  int exponent = a.parts.exp + b.parts.exp + result.parts.exp - FLOAT_EXP_BIAS;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  *err = ternary(exponent < 0, UNDERFLOW, *err);
  *err = ternary(exponent > MAX_NORM_FLOAT_EXP, ternary(result.parts.sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err); if(*err){ return result.f; }
  result.parts.exp = exponent;
  return result.f;
}  


// FUNCTION: double_addition(dbits, error*)

double safe_double_addition(dbits a, dbits b, error* err){
  //checking for errors
    if(!err){ return a.d; }
    if(*err){ return a.d; }
    if((a.parts.exp > MAX_NORM_DOUBLE_EXP) & a.parts.mantissa){ *err = SNAN; return a.d; }
    if((b.parts.exp > MAX_NORM_DOUBLE_EXP) & b.parts.mantissa){ *err = SNAN; return b.d; }
    if((a.parts.exp > MAX_NORM_DOUBLE_EXP) | (b.parts.exp > MAX_NORM_DOUBLE_EXP)){ *err = QNAN; return a.d; }
    long unsigned int v3, v4 = -(double_absolute_value(a) < double_absolute_value(b));
  //moving addend with the biggest absolute value to position of 'a' argument  
    a.luint ^= b.luint & v4; 
    b.luint ^= a.luint & v4; 
    a.luint ^= b.luint & v4; 
  //shifting smalest (b) so that number now represented with the same exponent   
    b.parts.exp = a.parts.exp - b.parts.exp; if (b.parts.exp > (1 + AMOUNT_OF_DOUBLE_MANTISSA_BITS)) { *err = UNDERFLOW; return a.d;}
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
  int bin_point_shift = safe_int_addition(how_many_bits_until_eldest_1(a.luint), how_many_bits_until_eldest_1(b.luint), err); if(*err){ return a; }
  b.luint = safe_luint_multiplication(a.luint, b.luint, err); if(*err){ *err = ternary((a_exp + b_exp) - DOUBLE_EXP_BIAS, POSITIVE_OVERFLOW, UNDERFLOW); return a; }
  *err = ternary(b.luint > MAX_DOUBLE_MANTISSA, POSITIVE_OVERFLOW, *err); if(*err){ return a; }
  bin_point_shift = safe_int_addition(how_many_bits_until_eldest_1(b.luint), -bin_point_shift, err); if(*err){ return b; }
  // moving product's first one to hidden one position
  b.luint <<= safe_int_addition(AMOUNT_OF_DOUBLE_MANTISSA_BITS, -how_many_bits_until_eldest_1(b.luint), err); if(*err){ return b; }
  b.parts.exp = bin_point_shift;
  return b;
}  

double safe_double_multiplication_without_rounding(dbits a, dbits b, error* err){
  if(!err){ return a.d; }
  unsigned char a_overfl_cond = a.parts.exp > MAX_NORM_DOUBLE_EXP, b_overfl_cond = b.parts.exp > MAX_NORM_DOUBLE_EXP;
  *err = ternary(a_overfl_cond, ternary(a.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(b_overfl_cond, ternary(b.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(a_overfl_cond && a.parts.mantissa, QNAN, *err);// check for NaN
  *err = ternary(b_overfl_cond && b.parts.mantissa, QNAN, *err);// check for NaN
  if(!a.d | !b.d){ return 0; } // check whether or not one of arguments equal to 0
  dbits result = b;
  result = safe_double_mantissa_multiplication_without_rouding(a, b, err); if(*err){ return result.d; }
  unsigned int exponent = safe_uint_addition(a.parts.exp, b.parts.exp, err); if(*err){ return result.d; }
  exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
  exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; }
  // check whether of not exponent value bigger than
  *err = else0(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(!(exponent > MAX_NORM_DOUBLE_EXP), *err); if(*err){ return result.d; }
  result.parts.exp = exponent;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  return result.d;
}  


// FUNCTION: double_multiplication_with_rounding(dbits,dbits,error*)

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
  a.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | a.parts.mantissa;
  b.luint = DOUBLE_MANTISSA_IMPLICIT_ONE | b.parts.mantissa;
  lluint result = long_mantissa_multiplication(a.luint, b.luint);
  b.parts.exp = (result.high & (1ul << 41));// TODO: OPTIMIZE
//b.parts.exp = result.high >> 41;// TODO: MAYBE THIS WILL BETTER   
  a.luint = (result.high << (23 - b.parts.exp)) | result.low >> (41 + b.parts.exp);// TODO: OPTIMIZE
  a.luint >>= 12;// TODO: I GUESS IT MUST BE SHIFTED 11 TIMES
  a.parts.exp = b.parts.exp;
  return a;
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

double safe_double_multiplication_with_rounding(dbits a, dbits b, error* err){
  if(!err){ return a.d; }// check whether or not is NULL pointer
  unsigned char a_overfl_cond = a.parts.exp > MAX_NORM_DOUBLE_EXP, b_overfl_cond = b.parts.exp > MAX_NORM_DOUBLE_EXP;
  *err = ternary(a_overfl_cond, ternary(a.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(b_overfl_cond, ternary(b.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
  *err = ternary(a_overfl_cond && a.parts.mantissa, QNAN, *err);// check for NaN
  *err = ternary(b_overfl_cond && b.parts.mantissa, QNAN, *err);// check for NaN
  if(!a.d | !b.d){ return 0; }// check whether or not one of argument is equal to 0
  dbits result = safe_double_mantissa_multiplication_with_rounding(a, b);
  int exponent = a.parts.exp + b.parts.exp + result.parts.exp - DOUBLE_EXP_BIAS;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  *err = ternary(exponent < 0, UNDERFLOW, *err);
  *err = ternary(exponent > MAX_NORM_DOUBLE_EXP, ternary(result.parts.sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
  result.parts.exp = exponent;
  return result.d;
}  


// FUNCTION: factorial(unsigned int, error*)

#define factorial_lookup_table_size 11
const unsigned int factorial_lookup_table[factorial_lookup_table_size] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800};  // DESCRIPTION: factorial values list of numbers from 0 to 10

unsigned int factorial(unsigned int a, error* err){
  if(!err){ return a; }
  if(a < factorial_lookup_table_size){ return factorial_lookup_table[a]; }
  unsigned int result = a;
  while(a-- > 1){
    result = safe_uint_multiplication(result, a, err);
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
    center = start + (start + size) / 2;
    if ( ((center*center) < (value + precision)) 
    || ((center*center) > (value - precision)) ) { return center; }
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
  *err = ternary(!(a.d) && !b, ZERO_TO_ZERO, *err);
  dbits r = (dbits){ .d = 1.0 };
  while(b-- && !(*err)){
      r.d = safe_double_multiplication_with_rounding(r, a, err);
  }
  return r.d;
}


// exponentiation of char_to_char(char, error*)

char exp_char2char(char a, char b, error* err){
  if(!err){ return a; }
  char result = 1;
  *err = else0(b < 0, UNDERFLOW);
  while(b-- && !(*err)){
      result = safe_char_multiplication(result, a, err);
  }
  return result;
}


// exponentiation of int_to_int(int, error*)

int exp_int2int(int a, int b, error* err){
  if(!err){ return a; }
  int result = 1;
  *err = else0(b < 0, UNDERFLOW);
  while(b-- && !(*err)){ 
      result = safe_int_multiplication(result, a, err);
  }
  return result;
}

// exponentiation of unsigned_int_to_unsigned_int(unsigned int, error*)

unsigned int exp_uint2uint(unsigned int a, unsigned int b, error* err){
  if(!err){ return a; }
  unsigned int result = 1;
  while(b-- && !(*err)){
      result = safe_uint_multiplication(result, a, err);
  }
  return result;
}


// exponentiation of unsigned_char_to_unsigned_char(unsigned char, error*)

unsigned char exp_uchar2uchar(unsigned char a, unsigned char b, error* err){
  if(!err){ return a; }
  unsigned char result = 1;
  while(b-- && !(*err)){
      result = safe_uchar_multiplication(result, a, err);
  }
  return result;
}


// exponentiation of long_int_to_long_int(long int, error*)

long int exp_lint2lint(long int a, long int b, error* err){
  if(!err){ return a; }
  long int result = 1;
  *err = else0(b < 0, UNDERFLOW);
  while(b-- && !(*err)){ 
      result = safe_lint_multiplication(result, a, err);
  }
  return result;
}

// exponentiation of long_unsigned_int_to_long_unsigned_int(long unsigned int, error*)

long unsigned int exp_luint2luint(long unsigned int a, long unsigned int b, error* err){
  if(!err){ return a; }
  long unsigned int result = 1;
  while(b-- && !(*err)){
      result = safe_luint_multiplication(result, a, err);
  }
  return result;
}


// exponentiation of float_to_float(float, error*)

float exp_float2float(fbits a, fbits b, error* err){
  if(!err){ return a.f; }
  fbits result = (fbits){ .f = 1}; 
  *err = else0(b.f - (int)b.f, ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER); 
  if(b.f < 0){
    while(b.f++ && !(*err)){
    //  result.f = safe_float_division_with_rounding(result, a, err);
    }
  }
  else{
      while(b.f-- && !(*err)){
      result.f = safe_float_multiplication_with_rounding(result, a, err);
      }
  }
  return result.f;
}


// exponentiation of double_to_double(double, error*)

double exp_double2double(dbits a, dbits b, error* err){
  if(!err){ return a.d; }
  dbits result = (dbits){ .d = 1}; 
  *err = else0(b.d - (long int)b.d, ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER); 
  if(b.d < 0){ 
    while(b.d++ && !(*err)){  
      // result.d = safe_double_division_with_rounding(result, a, err);
    }
  }
  else{
    while(b.d-- && !(*err)){  
      result.d = safe_double_multiplication_with_rounding(result, a, err);
    }
  }
  return result.d;
}


// FUNCTION: double_division

double safe_double_division(double dividend, double divisor){// quotient, remainder
    return dividend / divisor;
}
