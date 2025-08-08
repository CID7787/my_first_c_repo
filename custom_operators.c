#include <stdio.h>
#include <math.h>

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

void* test(void* args){
  printf("%c", (char*)args);
  return args;
}


/**
includes: ,
dependencies: ,
run: 19, 19
test: {
}
description: This function should perform a cast? Probably use it with our own data type
*/
int castToInt(void* a){
  return (int)a;
}

-
/**
includes: ,
dependencies: ,
run: 32
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) == !i) {printf("\nERROR! %u++ != %u", i, $0(i)); };
  }
}
description: This function should return 1 if passed 0 as an argument, and should return 0 if passed anything else as an argument.
It should do so without using if operator or a ternary operator.
*/
unsigned int logicalNOT(unsigned int a){
  return !a;
  return 1 >> a;
  /*
  Explanation:
  
  1 is represented as 0000...0001 in computer memory.
  The exact number of zeroes depends on computer architecture.
  In 32bit systems, there will be 32 bits inside an integer, so we get: 00000000000000000000000000000001
  In 64bit systems, there will be 64 bits inside an integer, so we get 63 zeroes and a single 1.
  
  Let's review 32bit as an example:
  00000000000000000000000000000001
  
  When a == 0, shifting this number by 0 will give you the same number.
  (00000000000000000000000000000001 >> 0) == 00000000000000000000000000000001
  However, when shifting this number by any other number, we will obtain 0:
  (00000000000000000000000000000001 >> 1) == 00000000000000000000000000000000
  (00000000000000000000000000000001 >> 2) == 00000000000000000000000000000000
  (00000000000000000000000000000001 >> X) == 00000000000000000000000000000000
  
  Therefore (1 >> a) is the same as (!a)
  */
}

/**
includes: ,
dependencies: ,
run: 19, 19
test: {
}
description: This function should return 1 if a == b, and 0 if a != b.
It should do so without using if operator or a ternary operator.
*/
unsigned int comparison(unsigned int a, unsigned int b){
  return a == b;
  return !(a ^ b);
}

/**
includes: ,
dependencies: ,
run: 1, &test, &test, (void*){49}
test: {
    void* testtruefunc(void*) { return 1; }
    void* testfalsefunc(void*) { return 0; }
    if($0(0, &testtruefunc, &testfalsefunc, (void*){48})) { printf("\nERROR! Used true function instead of false"); }
    if(!$0(1, &testtruefunc, &testfalsefunc, (void*){48})) { printf("\nERROR! Used false function instead of true"); };
}
description: This function should execute truefunction if the condition is true, and falsefunction if the condition is false
*/
void* ternary_operator(int condition, void* (*truefunction)(void*), void* (*falsefunction)(void*), void* args){
  return condition ? truefunction(args) : falsefunction(args);
}

/**
includes: ,
dependencies: ,
run: 1
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) == i-1) {printf("\nERROR! %u-- != %u", i, $0(i)); };
  }
}
description: This function should perform decrement (previous number) without using subtraction, addition and other, more complex operators
*/
unsigned int decrement(unsigned int a){
  return a--;
}

/**
includes: ,
dependencies: ,
run: 1
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) == i+1) {printf("\nERROR! %u++ != %u", i, $0(i)); };
  }
}
description: This function should perform increment (next number) without using addition and other, more complex operators
*/
unsigned int increment(unsigned int a){
  return a++;
}

/**
includes: ,
dependencies: ,
run: 9, 8
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    for (@1 j = 0; j < sizeof(@1)*8; ++i){
      if($0(i, j) == i+j) {printf("\nERROR! %u + %u != %u", i, j, $0(i, j)); };
    }
  }
}
description: This function should perform addition with simpler operators
*/
unsigned int addition(unsigned int a, unsigned int b){
  return a + b;
}

/**
includes: ,
dependencies: ,
run: 7, 8
test: {
  for (@1 i = 0; i < sizeof(@1)*2; ++i) {
    for (@1 j = 0; j < sizeof(@1)*2; ++i){
      if($0(i, j) == i*j) {printf("\nERROR! %u + %u != %u", i, j, $0(i, j)); };
    }
  }
}
description: This function should perform addition with simpler operators
*/
unsigned int multiplication(unsigned int a, unsigned int b){
  return a * b;
}

/**
includes: ,
dependencies: ,
run: 2, 5
test: {
  for (@1 i = 0; i < sizeof(@1); ++i) {
    for (@1 j = 0; j < sizeof(@1); ++i){
      if($0(i, j) == pow(base, exponent)) {printf("\nERROR! %u + %u != %u", i, j, $0(i, j)); };
    }
  }
}
description: This function should perform exponentiation with simpler operators
*/
unsigned int exponentiation(unsigned int base, unsigned int exponent){
  return pow(base, exponent);
}

/**
includes: ,
dependencies: ,
run: 3, 10
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    for (@1 j = 0; j < sizeof(@1)*8; ++i){
      if($0(i, j) == i+j) {printf("\nERROR! %u + %u != %u", i, j, $0(i, j)); };
    }
  }
}
description: This function should perform subtraction with simpler operators
*/
int subtraction(int a, int b){
  return a - b;
}