#include <stdio.h> // Library name: StandarD Input/Output
                   // Header file: stdio.h
                   // compiler directive: #include int main(){
#include <stdlib.h>
#include <math.h>
#include "printlib.h"
//hello

/**
includes: ,
dependencies: ,
run: 11
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) == (*(long int*)&i) % 2) {printf("\nERROR! %u != %u (i: %u)", $0(i), (*(unsigned int*)&i) % 2, i); };
  }
}
description: This function checks whether the last bit is 0

*/
unsigned int last_bit_is_0(unsigned int a){
  a = a << 31;
  a = a >> 31;
  0.1f; 0.1; 0xb; 0b1;
  return !a; 
}
/**
includes: ,
dependencies: ,
run: 11
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) != (*(long int*)&i) % 2) {printf("\nERROR! %u != %u (i: %u)", $0(i), (*(unsigned int*)&i) % 2, i); };
  }
}
description: This function checks whether the last bit is 1

*/
unsigned int last_bit_is_1(unsigned int a){
  unsigned int b = 1;
  return a & b; // should return 1 if last bit is 1, 0 if last bit is 0
}

unsigned int ith_bit_is_1(unsigned int a, unsigned int i){
  unsigned int b = 1;
  return a & (b << i);
}

unsigned int byte2bit(unsigned int n){
  return n << 3; // n * 8 = n * 2^3 = n << 3
}

unsigned int compare_1st_bit(unsigned int a, unsigned int b){
  unsigned int d = 1;
  return (~(a ^ b)) & d;
}

unsigned int how_many_0(int a){ 
  unsigned int u = 1;
  unsigned int o = 0;
  while(!(a & u)){
  // while (a ^ u){
   u =  u << 1;
   ++o;}
  return o;
}


unsigned int lshift(unsigned int a, unsigned int b){
    //  a << b         if (b == 0) { return a; }        if (b == 32) { return 0; }
    unsigned int n = how_many_0(sizeof(b) << 3); // 00100000
    unsigned int zero = 0;
    unsigned int mask = ~(((~zero) >> n) << n); // 00000000...00011111          a << 0 == a    a << 32 == 0
    unsigned int newshiftvalue = b & mask;  //     0000000...0 00100000       if (b == bitsize) { newshiftvalue = 0; } else { newshiftvalue = b % bitsize; }  
    // printf("\t(n = %u, b & mask = %u)", n, newshiftvalue);
    // printfint("mask:", &mask, 'd');
    // printfint("b & mask", &newshiftvalue, 'd');
    if ((~mask) & b) { return 0; }    // if (b >= bitsize) { return 0; }
    return a << newshiftvalue; // a << 0 ; => a
}

unsigned int compare_ith_bit(unsigned int a, unsigned int b, unsigned int i){//2
  // unsigned int c = ~0;
  // c = c >> i; c = c << i;
  unsigned int d = 1;
  d = lshift(d, i);  // 000...0001 << 32
  return (~(a ^ b)) & d; // 11111111 
}



unsigned int compare(unsigned int a, unsigned int b){ 
//   unsigned int h = 0;
//   while(compare_ith_bit(a, b, h)){ ++h; }
//   unsigned int c;
//   c = (compare_ith_bit(h, byte2bit(sizeof(a)), how_many_0(byte2bit(sizeof(h))) ));
  
  return !(a ^ b);
}

unsigned int logicalNOT(unsigned int a){
  return 1 >> a;
}

unsigned int condition(unsigned int a){ // what is the usage of this function?
  return !(!a);
}


unsigned int set_ith_bit_to_1(unsigned int a, unsigned int i){ 
  unsigned int b = 1;
  b = b << i;
  return a | i;
}
  
unsigned int set_last_bit_to_1(unsigned int x){ // this function sets my number's first(0) bit to 1
  unsigned int c = 1;
  return x | c;
}

// int c = 251;
// int error = increment(&c);
/* switch (error) {
    case 1: printf("OVERFLOW! ATTEMPTED TO INCREMENT MAX UNSIGNED INT (11111111111)"); break;
    case 2: ...; break;
    
    }

  int error = main();
  switch (error) {
  
  }
  
  a / b;
  int error = 1;
  division(a, b, &error);
  
*/
unsigned int increment_error(unsigned int * x){
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
  while(a & c){  // 
     a = a ^ c; // 0100 ^ 0100 = 0000
     c = lshift(c, 1);
  }
  (*x) = a | c; // i = 1;
  
  return (a | c); // return 1;
}

unsigned int decrement(unsigned int* x){ // warning: works wrong with 1
  if(!(*x)){return (~0);}
  unsigned int b = 1;
  unsigned int a = (*x);// 0000
  while (!(a & b)){ //  0001 & 0001
    a = a | b;
    b = lshift(b, 1);
  }

  (*x) = a ^ b;
  //(*x) = a | (b ^ (~0));// 0001 | 1000
 return 0;
}

unsigned int simple_addition(unsigned int a, unsigned int b){
  while(b){ increment_error(&a); decrement(&b);}
  return a;
}

unsigned int addition(unsigned int a, unsigned int b){
  // implement addition using increment and loops -- реализовать сложение используя инкремент и циклы
  unsigned int x = 0;
  while ( x < b ) { increment(&x); increment(&a); }
  // printf ("The sum of these numbers equal to:%u\n",a);
  return a;
}
unsigned int multiplication(unsigned int a, unsigned int b){
  // implement multiplication using addition and loops -- реализовать умножение используя сложение и циклы
  int c = 0;
  int d = 0;
  while ( c < b ) { increment(&c); d = addition(d, a); }
  // printf ("\n c:%d a:%d d:%d b:%d", c, a, d, b);
  return d;
}
unsigned int exponentiation(unsigned int a, unsigned int b){ // 2 , 0
  // implement exponentiation using multiplication and loops -- реализовать возведение в степень используя умножение и циклы
  int c = 0;
  int d = 1;
  while ( c < b) { increment(&c); d = multiplication(d, a); }
  // printf ("\n c:%d a:%d d:%d b:%d", c, a, d, b);
  return d;
}

unsigned int integer_division_by_2(unsigned int a){ // 4, 5, 8, 10
  return a >> 1;
}

unsigned int integer_division(unsigned int a, unsigned int b, unsigned int* isNull){
  //  2     0
  if (!b)  { (*isNull) = 2; return -1; }
  if (!a)  { return 0; }
  
  unsigned int c = 0;
  unsigned int d = 0;
  
  while (d <= (a - b)) {
    a = a - b;
    ++c;
  }
  (*isNull) = 1;
  return c;
  
  // a * b = c
  // multiplication(a, b) => c
  // division(c, a) => b == multiplication^-1(a, b)
  // f'(x) = df(x)/dx = lim( (f(x) - f(x + dx)) /dx)_(dx->0) = lim( (f(x) - f(x + 0.000000....000000001)) /0.0000....000001)
  // f(x) { return x*x; }
  // f'(x) = lim ( (f(2) - f(2 + dx))/dx )_(dx->0.125) = lim ( (f(2) - f(2 + 0.125))/0.125 )_(dx->0.125) = lim ( (4 - 4.515625)/0.125  ) = lim (0.51/0.125) = 4.08
  // f'(x) = 2*x -> 4
  // a * b => c
  // 0 * b => 1
  // 1 / 0 => null
  // c = a * b = a + a + ... b times
  // c = a - b = a--  ... b times
  // a-- ...
  // c = a / b
  // f^-1(x) * f(x) = 1
  // f(_x) -> x     [1] [1, 2, 3, 4, ....]
  
  // unsigned int quotient = a / b; // целая часть
  // unsigned int remainder = a % b; // остаток
  // return a / b;// return c
}
    // 0 1 2 3 4 5 6 7 8   
  //  {1,2,3,4,5,6,7,8,9}
 
unsigned int binary_search(int *array, unsigned int size, int value){ //10
  if (value > array[size - 1]){return -1;}
  unsigned int start = 0;
  if (value < array[start]){return -2;}
  unsigned int f = 0;
  unsigned int center = integer_division_by_2(size - start); //  center = 0 + (2 - 0) / 2 = 0 + 2/2 = 1
  while (1) {
    if(array[center] > value) {
       size = center; // size = 1
       center = start + integer_division_by_2(size - start); // 0 + (1 - 0) / 2 == 0
    }
    if (array[center] < value) {
      start = center; //  start = 0
      center = start + integer_division_by_2(size - start); // 0
    }
    if (array[center] == value) { break; }
    if ((start + 1) >= size) {
      if (array[start] == value) { return start; }
      if (array[size] == value) { return size; } // TODO: possible error
      return -3;
    }
    ++f;
    if (f > size) { return -4; }
  }
 return center;
}

float square_root(float value, float precision) {
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

char eldest_bit_is_0(int a){
  // int b = byte2bit(sizeof(a)) - 1; // size of variable a in bits - 1
  // int mask = 1 << b;number_without_exp
  // return !((a & mask) >> b);
  int mask = ~((~0) >> 1);
  return !(mask & a);
}

int division_of_negative_numbers_by_2(int a){// -6(1010)  
  int max_positive_integer = (~0) >> 1;
  int mask = ~max_positive_integer;
  return mask | ((max_positive_integer - (max_positive_integer - (mask ^ a) + 1) >> 1) + 1);
}

unsigned int convert_to_sign_and_magnitude(int a){
  int max_positive_integer = (~0) >> 1;
  int mask = ~max_positive_integer;
  return (max_positive_integer - (mask ^ a) + 1);
}
int convert_from_sign_and_magnitude(unsigned int a){
  int max_positive_integer = (~0) >> 1; // 0111111....    10000000000...
  int mask = ~max_positive_integer;
  a = a & max_positive_integer;
  return (mask | (max_positive_integer - a) + 1);
}

float floating_division_by_2(float b) { // 0|00001010|0000000 00000000 00000001
  int mask1 = ((((~0) << 1) >> 1) >> 23) << 23;
  int max_int = (~0) >> 1;
  int a = *((int*)&b);
  int exp = a & mask1;
  int number_without_exp = a & (~mask1);
  if (eldest_bit_is_0(number_without_exp)){
    if (last_bit_is_1(number_without_exp)){
      number_without_exp = multiplication(number_without_exp, 5);
      exp = ((exp >> 23) - 1) << 23;
    } else {
       int just_1 = 1;
       number_without_exp = (number_without_exp >> 1); // 0 00000000 mmmmmmmm...mmmm0 >> 1
      }
  } else {
     int magnitude = convert_to_sign_and_magnitude(number_without_exp);
      if (last_bit_is_1(magnitude)) {
        magnitude = multiplication(magnitude, 5);
        exp = ((exp >> 23) - 1) << 23;
      } else {
        magnitude = magnitude >> 1;
      }
     number_without_exp = convert_from_sign_and_magnitude(magnitude);
    }
  number_without_exp = number_without_exp | exp;
  float result = *((float*)&number_without_exp);
  return result; //
}

float division(float result, float m1, float precision){ // a = 9, b = 3 c = 1
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


/*
if (type == 'f') { float }
if (type == 'i') { int }
if (type == 'u') { unsigned int }
if (type == 'c') { char }
if (type == 'd') { double }

if (type == 'f') {
  float val = 0.0f;
  int i = 10;
  while (i--) { val++; }
}
if (type == 'i') {
  int val = 0;
  int i = 10;
  while (i--) { val++; }
}
if (type == 'u') {
  unsigned int val = 0;
  int i = 10;
  while (i--) { val++; }
}
if (type == 'c') {
  char val = '0';
  int i = 10;
  while (i--) { val++; }
}
if (type == 'd') {
  double val = 0.0;
  int i = 10;
  while (i--) { val++; }
}

---

  int i = 10;
  while (type == 'f' && i--) { float val = 0.0f; val++; }
  while (type == 'i' && i--) { int val = 0; val++; }
  while (type == 'u' && i--) { unsigned int val = 0; val++; }
  while (type == 'c' && i--) { char val = '0'; val++; }
  while (type == 'd' && i--) { double val = 0.0; val++; }

---
  int i = 10;
  while (i--){
    if (type == 'f') {
      float val = 0.0f;
      val++;
    }
    if (type == 'i') {
      int val = 0;
      val++;
    }
    if (type == 'u') {
      unsigned int val = 0;
      val++;
    }
    if (type == 'c') {
      char val = '0';
      val++;
    }
    if (type == 'd') {
      double val = 0.0;
      val++;
    }
  }
*/


char greater_than(void* a, int offsetA, void* b, int offsetB, char type){
  if ('i' == type){ return ( ((int*)a)[offsetA] > ((int*)b)[offsetB] ); }
  if ('f' == type){ return ( ((float*)a)[offsetA] > ((float*)b)[offsetB] ); }
  if ('u' == type){ return ( ((unsigned int*)a)[offsetA] > ((unsigned int*)b)[offsetB] );}
  if ('d' == type){ return ( ((double*)a)[offsetA] > ((double*)b)[offsetB]);}
  if ('c' == type){ return ( ((char*)a)[offsetA] > ((char*)b)[offsetB] );}
  return 2;
}
char less_than(void* a, int offsetA, void* b, int offsetB, char type){
  if ('i' == type){ return ( ((int*)a)[offsetA] < ((int*)b)[offsetB] ); }
  if ('f' == type){ return ( ((float*)a)[offsetA] < ((float*)b)[offsetB] ); }
  if ('u' == type){ return ( ((unsigned int*)a)[offsetA] < ((unsigned int*)b)[offsetB] ); }
  if ('d' == type){ return ( ((double*)a)[offsetA] < ((double*)b)[offsetB]); }
  if ('c' == type){ return ( ((char*)a)[offsetA] < ((char*)b)[offsetB] ); }
  return 2;
}
char equals(void* a, int offsetA, void* b, int offsetB, char type){
  if ('i' == type){ return ( ((int*)a)[offsetA] == ((int*)b)[offsetB] ); }
  if ('f' == type){ return ( ((float*)a)[offsetA] == ((float*)b)[offsetB] ); }
  if ('u' == type){ return ( ((unsigned int*)a)[offsetA] == ((unsigned int*)b)[offsetB] );}
  if ('d' == type){ return ( ((double*)a)[offsetA] == ((double*)b)[offsetB] );}
  if ('c' == type){ return ( ((char*)a)[offsetA] == ((char*)b)[offsetB] );}
  return 2;
}

int array_binary_search(void* array, void* x, unsigned int size, char type){
    int min = 0;
    int middle = (size - 1) / 2;
    while(1){
      if (greater_than(x, 0, array, middle, type)) {min = middle;}
      if (less_than(x, 0, array, middle, type)) {size = middle;}
      middle = min + ((size + min) / 2);
      if (equals(x, 0, array, middle, type))  {return middle;}
    }
  return -1;
}

int square(int a){
  return a * a;
}
int cube(int a){
  return a * a * a;
}

/*
    if (eldest_bit_is_0(a)) { // число положительное
      return a >> b;
    } else { // число отрицательное
    }
 00000101
?
 00000010|101

 00000010|101
 +25 * 10^-1
 
  32 - 1 - 8 = 23
   +   -1           25
 | |s|eeeeeeee|mmmmmmm mmmmmmmm mmmmmmmm| |
  & 1|00000000|0000000 00000000 00000000|   <- mask1
  = s|00000000|0000000 00000000 00000000
  >> 8
  = 0|0000000s|0000000 00000000 00000000   -> x
  
  x = (a & mask1) >> 8;
  
  
  
    s|eeeeeeee|mmmmmmm mmmmmmmm mmmmmmmm|
  & 0|00000000|1111111 11111111 11111111  <- mask2
  = 0|00000000|mmmmmmm mmmmmmmm mmmmmmmm  -> y
  
  y = (a & mask2)
  
  r = x | y ==
  = 00000000 smmmmmmm mmmmmmmm mmmmmmmm
  
  1 00111100 000000000000000000000001     -1
  00000000 1 000000000000000000000001    2^23 + 1
  1 00000000 000000000000000000000001    -1
  
  sqrt(1) = +-1
  
  Какие 2 операции умножения мне нужно выполнить, чтобы оказаться в 1?
  
  Умножение на 1:  Масштабировать себя на 1, и еще раз масштабировать себя на 1.
  Умножение на -1: Развернуться на 180, и еще раз развернутся на 180.
  
                 ...
                 3i    *
                 2i                   '*' just mean dots in complex plane(probably like this)
                 i
  ...-4 -3 -2 -1 0 1 2 3 4 *....
                -i
               -2i
               -3i
               ...

  a + b = c
  
  a [r] + b [i] =  
  
// Online C compiler to run C program online
#include <stdio.h>

struct STRING {
  char* pointer; // 32
  float f; // 32
  double d; // 64
  unsigned long int size; //64
};

union FLOATINT{
  float f;  // 32
  int i; // 32
  unsigned int u; // 32
} typdef FLOATINT;

FLOATINT a;
a.f = -1.0f;
int b = a.i;
unsigned int c = a.u;
a.i = 10;

|1011111 10000000 00000000 00000001|

EDIT_FILE        1
EDIT_MESSAGE     0
DELETE_MESSAGE   1
... 

enum PERMISSIONS {
   EDIT_FILE = 0,   // 0
   EDIT_MESSAGE,    // 1
   DELETE_MESSAGE,  // 2
}
long int permissions = 0b001;

char set_permissions(unsigned int p) {
  if (p < 3) {
    return p | (0b1 << PERMISSIONS.DELETE_MESSAGE);
    return 0;
  }
  return 1;
}

struct USER {
  unsigned int id; // 32
  long int permissions; // 64
  unsigned int* profile_picture; // 32
  char* username; // 32
} typedef USER; // 160 bit

USER* database = malloc(1024 * sizeof(USER)); // 20480

|20 bytes|20 bytes|... 1024 times

char register(char* username){
  ...
  struct USER u;
  u.username = username;
  u.id = generate_new_ID();
  u.permissions = 0b100;
  
  database[0] = u;
}

char delete_message(unsigned int user_id, unsigned int message_id) {
  if (!( get_user(used_id).permissions & (0b1 << PERMISSIONS.DELETE_MESSAGE) )) { return 1; }
  ...
}

// after 10 years I will be good programmer like Amirako


int main() {
    STR s;
    s.f = 10.0f;
    printf("%f", s.f);

    return 0;
}
  
  |char* pointer|float f|double d|...
  
  int a = x & mask1;
  int b = x & mask2;
  int c = (b << byte2bit(sizeof(b))) | a;
  
  i = 0;
  STR s;
  s.size = 10;
  s.pointer = malloc(sizeof(char) * (s.size));
  
  POINT2D p1;
  unsigned int length = (p1.x * p1.x) + (p1.y * p1.y);
  while (i++ < (STRING->size)) {
  }
  
  
                ID  MEMORY  OPERATIONS 
  int           1    32      ...
  float         2    32      ...
  ...              
  POINT2DTYPE   97   64      ...   struct POINT2D 
  
  
  struct POINT2D
  p1
  444491919191919
  int              int
  radius           angle
  444491919191919  444491919191920
  |000000...4bytes|0000...4bytes|
  
  struct POINT3D {
     int x;
     int y;
     int z;
  };
  
   int*
   x                       y                       z
   444491919191919         444491919191920         444491919191921         
   |00000000.....00000000||00000000......00000000|0000000000000000000000000000|00000000......00000000|
  
  int x;
  int y;
  int z;
  
  [1, 1];
  2 * [1, 1] = [2*1, 2*1] = [2, 2]
  
  x *= 2;
  y *= 2;
  z *= 2;
  ...
  
  
  int arraysize = 2;
  int* point = malloc(arraysize * sizeof(int)); // -> 444491919191919
  
  
  array[i]

  unsigned int i = arraysize;
  while (i-- > 0) {   // 0 > 0  => false ; i-- => i 
    (&x)[i] *= 2;   //  444491919191919
  }
  
  
  ... arraysize
  ... arraysize
  
  
  POINT2DTYPE p1;
  struct POINT2D p2;
  
  struct POINT3D P1;
  struct POINT3D P2;
  
  int F(int a) { return a*a; }
  
  F(1);
  
  
  
  float -> sign + mantissa -> ...  -> float
        -> exponent        -> ...  /
  
  1|01111110|0000000 00000000 00000001
  1 00000000 0000000 00000000 00000001  -(2^(N-1) - 1)
  1 ???????? ??????? ???????? ????????   2^(N-1) - 1  
  1 11111111 1111111 11111111 11111111
  
  1000000
     1111

    
    0000 >> 2 = 0000
    0001 >> 2 = 0000
    0010 >> 2 = 0000
    0011 >> 2 = 0000
    0100 >> 2 = 0001
    0101 >> 2 = 0001
    0110 >> 2 = 0001
    0111 >> 2 = 0001
    
    
    
    
     -8 + 0 = -8
     -8 + 7 = -1
     (~0) 
    (-1) 1111 / 2 = 0000
    (-2) 1110 / 2 = 0001
    (-3) 1101 / 2 = 0001
    (-4) 1100 / 2 = 0010
    (-5) 1011 / 2 = 0010
    (-6) 1010 / 2 = 0011
    (-7) 1001 / 2 = 0011
    (-8) 1000 / 2 = 0100
    
    
    1|000
    -|000
    -|(8-0)
    -|(2^N - xxx)
    
  
    
    (2^N-1) - (1000 ^ 1010) + (1) = 1
    (2^N-1) - 6 + (I) = 2
    7 - 6 = 1
    6 - 4 = 2
    
    -6  6 >> 1
    
    return 0;

    
    
    
    
    
    
    
      
        ----------------------------------------------
       |                                             \/
  1) Basics -> algorithms & data structures <- numerical methods
                                            -> domain-specific
                                            -> architecture
  2) high-level abstractions -> algorithms & data structures -> return to basics
  
>> 1
 00000001|10|101


  magnitude (прямой код)
  1111  =  15
  0111  =  7
  0000  = 0 
  
  sign and magnitude (прямой код)
  1111  = -7
  0111  = 7
  0000 = +0
  1000 = -0
  
  increment(a)
  
  One's complement (обратный код)
  
  Two's complement (дополнительный код)
  1111 = -1
  1110 = -2
  1101 = -3
  1100 = -4
  1011 = -5
  1010 = -6
  1001 = -7
  1000 = -8
  0111 = 7
  ...
  0011 = 3
  0010 = 2
  0001 = 1
  0000 = 0
  
  offset binary (двоичный сдвиг)
  
  
  
  S&M - 2^(bitsize-1)  -> rotate_counterclockwise(180) -> 2sC

int array[] = {1, 4, 7, 8, 10, 12, 14, 20, 21};
int a = 10;
int* pointer = &a; // 4593939393933
char* pointer2 = (char *)&a; // 4593939393933

pointer[1]
pointer2[1]
  
int* arr = malloc(size); // Memory ALLOCation



x^2 = 1

x_0 = sqrt(1) = 1  x_1 = -sqrt(1) = -1
x = sqrt(1) = 1

x^2 = 2

x_0 = sqrt(2)  x_1 = -sqrt(2)
x = sqrt(2) = 1.4132...

x^2 = 576

x = sqrt(5) = ????




      1,4,7,8,10,12,14,20,21
      1
      1,3     (2)
  
  */
  
  //     int
  //     a
  //     4593939393933  4593939393934  4593939393935  4593939393936  4593939393937 
  //     00000000,      00000000,      00000000,      00000000,      00000000, ...
  


  
  
  /*
  0 1 2 3 4 5 .... 4 129 985 727
  | | | 
  | | |
  | | |
  0 1 2 3 4 5 ...
  
  int a;
  sizeof(int) + 1
  00001111|00001111|00001111|00001111|00000000
  
  
  2^8 - 1
  2^7 - 1
  
d unsigned int 00001111
  
  int? a = null;
  int* a = 
  int a = 0;
  
  1) Зарезервировать 1 значение как специальное значение
  2) Выделить память после числа, в которой будет метаинформация про число
  3) Выделить 1 бит внутри памяти числа
  4) Компилятор хранит информацию в типе данных
  5) Функции возвращают несколько значений
  
  struct VAR {
    int a;
    char type;
    char isNull;
    ...
  }
  
  
  var a;                              
                                      char
  da                                   type
  00001111|00001111|00001111|00001111|00000000|
  
  6 / 3 = 2 (0)
  
  2.0f / 3.0f = 0.6666666....
  */


int main(){

  double d = 0.25;
  printfint("d", &d , 'd');// 0 01111111110 (1)0.00000 1 * 2^-2
  d *= 10;
  printfint("d", &d , 'd');// 0 10000000001 (1)0.10000
  
  double d1 = 0.75;
  printfint("d1", &d1, 'd');// 0  01111111011(-5) (1) 0.000000000000000000000...[0.0001](0.0625)
  d1 *= 10;
  printfint("d1", &d1, 'd');// 0  01111111110(-2) (1) 0.100000000000000000000...[0.101](0.625)
  d1 *= 10;
  printfint("d1", &d1, 'd');// 0  10000000001(1) (1) 1.001000000000000000000...[110.01](6.25)
  d1 *= 10;
  printfint("d1", &d1, 'd');// 0  10000000100(4) (1) 1.111010000000000000000...[111110.1](62.5)
  d1 *= 10; 
  printfint("d1", &d1, 'd');// 0  10000001000(8) (1) 0.011100010000000000000...[1001110001](625)
  

        
   // AMIR:
  // 0 10000000010 (1)000.0000000000000000000000000000000000000000000000000 (8.00000 = 8, 2)
  // 0 10000000001 (1)11.00000000000000000000000000000000000000000000000000 (7.00000 = 7, 1)
  // 0 10000000001 (1)10.00000000000000000000000000000000000000000000000000 (6.00000 = 6, 1)
  // 0 01111111110 .(1)0000000000000000000000000000000000000000000000000000 (0.50000 = 0.5, -2)
  
  // 0 10000000000 (1)0.100000000000000000000000000000000000000000000000000 ()
  // 0 10000000000 (1)0.010000000000000000000000000000000000000000000000000 ()
  
  //  1 * 10^-2
  // ....0000000.01000000000...
  //  1 * 2^0
  //  (1) 0.000000
  // точка справа от (e)-того бита
  
  // FARRUKH:
  // 0 10000000010 (1)000.0000000000000000000000000000000000000000000000000 (8.00000 = 2 * 2^2)
  // 0 10000000001 (1)11.00000000000000000000000000000000000000000000000000 (7.00000 = 3.5 * 2^1)
  // 0 10000000001 (1)10.00000000000000000000000000000000000000000000000000 (6.00000 = 3 * 2^1)
  
  //    e
  // 0 10000000001 (1)0.100000000000000000000000000000000000000000000000000 (5.00000 = 2.5 * 2^1)
  // 0 10000000001 (1)0.000000000000000000000000000000000000000000000000000 (4.00000 = 2 * 2^1)
  // 0 10000000000 (1)1.000000000000000000000000000000000000000000000000000 (3.00000 = 3 * 2^0)
  // 0 10000000000 (1)0.000000000000000000000000000000000000000000000000000 (2.00000 = 2 * 2^0)
  // 0 01111111111 (1)0.000000000000000000000000000000000000000000000000000 (1.00000 = 2 * 2^-1)
  
  /*
  {
    int ab; // ab = 140725892993180; 4byte; interpret memory as integer;
  } // stop associating ab with 140725892993180;
  
  int ba = 1; // ba = 140725892993180; 4 byte; interpret memory as integer
  char c = 'h';
  float f = -11.0;
  printfint("ba", &ba, 'd');  // printf("%ld")
  printfint("f", &f, 'f');
  printfint("c", &c, 'c');

int a [2];  // int[]  2

(int[]){1,2};

int a;

1;


a 
int 
2833jjds   1133isnc 
00000001  00000010

//
#include <stdio.h>
int f1(int a[]) {
  while (i != 0){
      i++;
  }
  return i;
}
int main() {
    int c[7] = {1,2,3,4,5,6,7};
    int* v = &c;
    printf("%p\n",f1(v));

    return 0;
}
//
  int* pointer_to_int = &ba; // 140725892993180; te * 8 = 32bit (unsigned) integer
  long int pointer_int = &ba; //    ampersand -&
  long int pointer_int2 = &c;
  char* pointer_to_char = &c;
  int** pointer_to_pointer_to_int = &pointer_to_int;
  
  printfint("pointer_to_int", &pointer_to_int, 'a');
  
  char* s = "10101";
  int a = 'v';
  printfint ("a", &a, 'd');
  */
  
  
        
  // 0, 1, 2, 3
  // 4  5  6  7
  unsigned int size = 10;

  int* iarray = malloc(sizeof(int) * size);
  for (int i = 0; i < size; ++i) { iarray[i] = i*10; }
  int ix = 80;
  // array_binary_search((void*)iarray, (void*)&ix, size, 'i');

  float* farray = malloc(sizeof(float) * size);
  for (int i = 0; i < size; ++i) { farray[i] = i * 20.0f; }
  float fx = 75;
  // array_binary_search((void*)farray, (void*)&fx, size, 'f');
    
  // f(x) => y
  // function_binary_search(&square, 100, 0, 100);
  // function_binary_search(&cube, 1000, 0, 1000);
  
  // char*
  // s
  // 140735518521764
  // |00110001|00110000|00110001|0110000|00110001|00000000|0101010|101010101|....
  //           140735518521765                    ^  null
  
  /*int a = 1;
int* array = &a;
char* s = (char *)&a;

int access_ith_element_of_array(void* arr, int i){ // 4134455989191 , 1
  int size = sizeof(arr); // 1
  int new_address = arr + i * size; // 4134455989191 + 1*1 = 4134455989192
  int value = *new_addresss;
  return value;
}
access_ith_element_of_array(array, 1); // array[1];
*/  
// TESTS:
char flag = 1;
for (unsigned int i = 0; i < 10; ++i) {
  // unsigned int i_temp = i;
  // unsigned int Iresult = increment(&i_temp);
  // if (Iresult != (i+1)) { flag = 0; printf("\nERROR! %u++ != %u", i, Iresult); }
  for (unsigned int j = 0; j < 10; ++j){
    unsigned int Aresult = addition(i, j);
    if (Aresult != (i + j)) { flag = 0; printf("\nERROR! %u + %u != %u", i, j, Aresult); }
    unsigned int Mresult = multiplication(i, j);
    if (Mresult != (i * j)) { flag = 0; printf("\nERROR! %u * %u != %u", i, j, Mresult); }
    if (j < 5) {
      unsigned int Eresult = exponentiation(i, j);
      if (Eresult != (unsigned int)pow((double)i, (double)j)) { flag = 0; printf("\nERROR! %u ** %u != %u", i, j, Eresult); }
    }
    // float Dresult = division((float)i, (float)j, 0.001);
    // if (Dresult != (i / j)) { flag = 0; printf("\nERROR! %f / %f != %f", (float)i, (float)j, Dresult); }
  }
}
if (flag) { printf("\nSUCCESS!"); }
  
  return 0;
} 

// https://en.wikipedia.org/wiki/Instruction_set_architecture
// https://en.wikipedia.org/wiki/Tunnel_vision
// https://en.wikipedia.org/wiki/Von_Neumann_architecture
// https://en.wikipedia.org/wiki/Harvard_architecture
// https://killedbygoogle.com/
// https://www.programmingfonts.org/
// https://betterexplained.com
// https://www.mathsisfun.com/algebra/logarithms.html
// https://www.youtube.com/watch?v=IxXaizglscw
// https://www.open-std.org/jtc1/sc22/WG14/www/docs/n1256.pdf
