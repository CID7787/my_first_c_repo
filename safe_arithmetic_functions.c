#ifndef headerfile
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "bitwise_functions.c"
#endif



// FUNCTION: f_mod(double and dbits)

double my_fmod(dbits x, dbits y){ // @TODO: add error handling, maybe in v6 or another version
    y.bits.sign = 0; // we set the sign bit of y to 0
    dbits ux = x, uy = y; // we copy x and y to new variables, which will become unsigned
    ux.bits.sign = 0; // we set ux sign to 0, and uy already has its sign set to 0
    while(uy.d <= ux.d){ uy.d += y.d; } // we are exiting as soon as uy > ux.  (uy - ux) shows us how much we overflow 
    y.d -= uy.d - ux.d; // we subtract (uy - ux) from y to find a remainder
    x.bits.positive = y.bits.positive; // here we set only positive x, so that we preserve the sign of x
    return x.d;
}

double my_fmod_v1(double x, double y){
    long unsigned int n = 1;
    long unsigned int mask = (~n) >> 1;// 011111.....111111
    long unsigned int temp = (*(long unsigned int*)&y) & mask;
    y = *(double*)&temp;
    temp = ~mask & (*(long unsigned int*)&x);
    long unsigned int positive_luint_x = (*(long unsigned int*)&x) & mask;
    x = *(double*)&positive_luint_x;
    while((y * (double)n) <= x){ ++n; }
    x -= ((double)(--n))*y;
    n = (*(long unsigned int*)&x) | temp;
    return *(double*)&n;
}


double my_fmod_v2(double x, double y){
    long unsigned int n = 1;
    long unsigned int mask = (~n) >> 1;
    v1.d = y;
    v1.luint &= mask;
    y = v1.d;
    v1.d = x;

    n = v1.luint | ~mask;
    v1.luint &= mask;
    mask = n;
    n = 1;
    while((y * (double)n) <= v1.d){ ++n; }
    v1.d -= (double)(--n) * y;
    v1.luint |= mask;
    return v1.d;
}

double my_fmod_v3(double x, double y){
    long unsigned int n;
    union_var.d = y;
    union_var.luint.luint2 = 0;
    y = union_var.d;
    union_var.d = x;
    n = union_var.luint.luint2;
    union_var.luint.luint2 = 0;
    x = union_var.d;
    union_var.luint.luint2 = n;
    union_var.luint.luint1 = 0;
    n = 1;
    while((y * (double)n) < x){ ++n; }
    x -= y * (double)(--n);
    n = union_var.luint.luint2;
    union_var.d = x;
    union_var.luint.luint2 = n;
    return union_var.d;
}

double my_fmod_v4(double x, double y){
  long unsigned int n = 1;
  dbits temp;
  temp.d = y;
  temp.bits.sign = 0;
  y = temp.d;
  temp.d = x;
  x = temp.bits.sign;
  temp.parts.sign = 0;
  while((y * (double)n) < temp.d){ ++n; }
  temp.d = temp.d - y *(double)(--n);
  temp.bits.sign = (long unsigned int)x;
  return temp.d;
}
  
  
// FUNCTION: integer_addition(int, error*)

int safe_int_addition(int addend1, int addend2, error* err){
  *err = ternary((addend1 > 0) && (addend2 > ((long int)MAX_INT - (long int)addend1)), POSITIVE_OVERFLOW, *err);
  *err = ternary((addend1 < 0) && (addend2 < ((long int)MIN_INT - (long int)addend1)), NEGATIVE_OVERFLOW, *err);
  return addend1 + addend2;
  }


// FUNCTION: unsigned_int_addition(unsigned int, error*)

unsigned int safe_uint_addition(unsigned int arg1, unsigned int arg2, error* err){
  *err = ternary(arg2 > (MAX_UINT - arg1), POSITIVE_OVERFLOW, *err);
  return arg1 + arg2;
}


// FUNCTION: unsigned_int_multiplication(unsigned int, error*)

unsigned int safe_uint_multiplication(unsigned int arg1, unsigned int arg2, error* err){
  unsigned int result = 0;
  while((arg2-- > 0) && !(*err)){
    result = safe_uint_addition(result, arg1, err);
  }
  return result;
}


// FUNCTION: long_unsigned_int_addition(long unsigned int, error*)

long unsigned int safe_luint_addition(long unsigned int addend1, long unsigned int addend2, error* err){
  *err = ternary(addend2 > (MAX_LUINT - addend1), POSITIVE_OVERFLOW, *err);
  return addend1 + addend2;
}


// FUNCTION: long_unsigned_int_multipication(long unsigned int, error*)
long unsigned int safe_luint_multiplication(long unsigned int multiplier, long unsigned int multiplicand, error* err){
  long unsigned int product = 0;
  while(multiplicand-- > 0){
    if(*err){ return product; }
    product = safe_luint_addition(product, multiplier, err); 
  }
  return product;
}
  
  
// FUNCTION: factorial(unsigned int, error*)

#define factorial_lookup_table_size 11
const unsigned int factorial_lookup_table[factorial_lookup_table_size] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880, 3628800};  // DESCRIPTION: factorial values list of numbers from 0 to 10

unsigned int factorial(unsigned int a, error* err){
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
  int a = (*x);
  if (compare(a, ~0)) { return 1; } // special case 111111...111111
  unsigned int c = 1;
  while(a & c){ 
    a = a ^ c;
    c = lshift(c, 1);
  }
  (*x) = a | c;
  return 0;
}

unsigned int increment(unsigned int* x){
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
  //  2     0
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
  if (eldest_bit_is_0(number_without_exp)){
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
  if (!result) {return -3.0f;}
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


// FUNCTION: double_multiplication_without_rounding(dbits, dbits, error*)

dbits safe_double_mantissa_multiplication_without_rouding(dbits a, dbits b, error* err){
  unsigned int a_exp = b.parts.exp, b_exp = b.parts.exp;
  a.luint = DOUBLE_MANTISSA_HIDDEN_ONE | a.parts.mantissa;
  b.luint = DOUBLE_MANTISSA_HIDDEN_ONE | b.parts.mantissa;
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
// check whether or not one of arguments equal to 0
if(!a.d | !b.d){ return 0; }
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

long unsigned int luint_multiplication_v1(long unsigned int multiplicand, long unsigned int multiplier, unsigned int exponent){
  long unsigned int part1 = 0, part2 = 0;
  int n1, n2;
  unsigned int cond;
  error err = NO_ERROR;
  while(multiplicand--){
    part1 = safe_luint_addition(part1, multiplier, &err); 
    if(err) { ++part2; part1 = multiplier - (MAX_LUINT - part1) - 1; }
  }
  n1 = how_many_bits_until_eldest_1(part1);
  n2 = how_many_bits_until_eldest_1(part2);
  multiplicand = else0(part2, n2 + 1) | else0(!part2, n1 + 1);
  cond = multiplicand > (sizeof(double) << 3);
  multiplier = (else0(cond, part2) | else0(!cond, part1)) >> (exponent - else0(cond, (sizeof(double) << 3)));
  exponent = multiplier & 0b10;
  part1 >>= n2 + else0(part2, 1);
  part2 <<= 63 - n2;
  part2 |= part1;
  n2 = how_many_bits_until_eldest_1(part2);
  part2 >>= else0(part2 > MAX_DOUBLE_MANTISSA, safe_int_addition(n2, -52, &err));
  part2 <<= else0(part2 <= MAX_DOUBLE_MANTISSA, safe_int_addition(52, -n2, &err));
  part2 ^= DOUBLE_MANTISSA_HIDDEN_ONE;
  return ((long unsigned int)exponent << 52) | part2;
}

long unsigned int lluint_multiplication_v2(long unsigned int multiplicand, long unsigned int multiplier, unsigned int bin_point_shift){
  long unsigned int result_l, result_r, product1, product2, multiplicand_l, multiplicand_r, multiplier_l, multiplier_r, mask_r = 0x7ffffff;
  error err = NO_ERROR;
  // getting left and right parts of multiplicand and multiplier 
  multiplicand_l = multiplicand >> 27;
  multiplier_l = multiplier >> 27;
  multiplicand_r = multiplicand & mask_r;
  multiplier_r = multiplier & mask_r;
  // computing products
  product1 = safe_luint_multiplication(multiplicand_l, multiplier_l, &err);// << (27 + 27)
  product2 = safe_luint_multiplication(multiplicand_r, multiplier_l, &err);// << 27
  result_r = safe_luint_multiplication(multiplicand_l, multiplier_r, &err);// << 27
  product2 = safe_luint_addition(product2, result_r, &err);// << 27
  result_r = safe_luint_multiplication(multiplicand_r, multiplier_r, &err);
  // summing up products
  product2 = safe_luint_addition(product2, result_r & (~0ul << 27), &err);
  product1 = safe_luint_addition(product1, product2 & (~0ul << 27), &err);
  result_r = product1 << 54 | product2 << 27 | result_r;
  result_l = product1 >> 10;
  // getting exponent
  product1 = how_many_bits_until_eldest_1(result_r);
  product2 = how_many_bits_until_eldest_1(result_l);
  product1 = ternary(result_l, product1 + 1, product1);
  product2 = ternary(result_l, product2 + 1, product2);
  bin_point_shift = ternary(product2, product2 >> safe_int_addition(bin_point_shift, -64, &err), product1 >> bin_point_shift);
  bin_point_shift = !!(bin_point_shift & 0b10);
  // normalizing number and putting bin_point_shift into exponent
  result_l <<= safe_int_addition(64, -product2, &err); // 0000 0100
  result_r >>= product2;
  result_l |= result_r;
  product1 = how_many_bits_until_eldest_1(result_l);
  result_l = ternary(result_l < DOUBLE_MANTISSA_HIDDEN_ONE, result_l << safe_int_addition(52, -product1, &err), result_l >> safe_int_addition(product1, -52, &err));
  result_l = (DOUBLE_MANTISSA_HIDDEN_ONE - 1) & result_l;
  result_l = ternary(bin_point_shift, result_l | DOUBLE_MANTISSA_HIDDEN_ONE, result_l);
  return result_l;
}    

dbits safe_double_mantissa_multiplication_with_rounding_v1(dbits multiplicand, dbits multiplier, error* err){
  multiplicand.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplicand.parts.mantissa;
  //TODO: CHANGE mantissa PARTS TO MANT
  multiplier.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplier.parts.mantissa;
  // remove useless zeros
  while(!((multiplicand.luint | multiplicand.luint) & 1ul)){ multiplicand.luint >>= 1; multiplier.luint >>=1; }
  //    if((multiplicand.luint & multiplier.luint) == 1) { return (dbits){ .luint = DOUBLE_MANTISSA_HIDDEN_ONE}; }
  // variable declaration
  multiplier.luint = lluint_multiplication_v2(multiplicand.luint, multiplier.luint, how_many_bits_until_eldest_1(multiplicand.luint) << 1);// TODO: separte lluint_multiplication to multiplication and rounding parts
  return multiplier;
}

double safe_double_multiplication_with_rounding_v1(dbits a, dbits b, error* err){
  if(!a.d | !b.d){ return 0; }
  char a_nan_cond = (a.parts.exp > MAX_NORM_DOUBLE_EXP) & a.parts.mantissa;
  char b_nan_cond = (b.parts.exp > MAX_NORM_DOUBLE_EXP) & b.parts.mantissa;
  if(a_nan_cond | b_nan_cond){ return ternary(a_nan_cond, a.d, b.d); }
  dbits result = safe_double_mantissa_multiplication_with_rounding_v1(a, b, err); if(*err){ return result.d; }
  unsigned int exponent = safe_uint_addition(a.parts.exp, b.parts.exp, err); if(*err){ return result.d; }
  exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
  exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; }
  // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
  *err = else0(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(exponent <= MAX_NORM_DOUBLE_EXP, *err); if(*err){ return result.d; }
  result.parts.exp = exponent;
  result.parts.sign = a.parts.sign ^ b.parts.sign;
  return result.d;
}

// FUNCTION: double_base_to_unsigned_int_power(double, unsigned int, error*)

double exp_double2uint(double base, unsigned int power, error* err){// DESCRIPTION: base of type 'double' to power of type 'unsigned int'
  if (*err != NO_ERROR) { return 1.0; }
  if(!power){ 
    if(base == 0.0){ *err = ZERO_TO_ZERO; }// OPTIMIZE
    return 1.0;
  }
  double result = 1.0;
  dbits result_dbits = (dbits){ .d = result};
  dbits base_dbits = (dbits){ .d = base};
  while(power-- > 0){
    if(*err){ return result; }
    result = safe_double_multiplication_with_rounding_v1(result_dbits, base_dbits, err);
  }
  return result;
}


// FUNCTION: double_division

double safe_double_division(double dividend, double divisor){// quotient, remainder
    return dividend / divisor;
}
