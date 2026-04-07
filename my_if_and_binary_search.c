#ifndef headerfile
  #include <stdio.h>
  #include <stdlib.h>
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "bitwise_functions.c"
  #include "safe_arithmetic_functions.c"
#endif

  /**
includes: ,
dependencies: ,
run: 11
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) == (*(long int*)&i) % 2) {printf("\nERROR! %u != %u (i: %u)", $0(i), (*(unsigned int*)&i) % 2, i); };
  }
}
description: This function checks w
hether the last bit is 0

*/
/* void* conditional_operator(int cond, void* (*true_cond_func)(), void* (*false_cond_func)()){
  cond = !(!cond); // 1; 0
  void* t = true_cond_func; // convert function pointer to data pointer
  void* f = false_cond_func;
  int big = cond << (sizeof(int) << 1); // 0/0010000....
  int r = (~0) >> big; // 00000000... | 11111111......
  void* result = (r & t) | (r & f); // leave only one data pointer according to IF/OR logic, based on cond value
  void* (*resulting_function)() = (void* (*)())result; // convert back to the function pointer
  resulting_function(); // call a function
  return result;
} */

void* my_if(int cond, void* (*true_cond_func)(), void* (*false_cond_func)()){
  void* t = true_cond_func; // convert function pointer to data pointer
  void* f = false_cond_func;
  int big1 = ~0 >> (!(!cond) << (sizeof(int) << 1)); // 0/0010000....
  int big2 = ~0 >> (!cond << (sizeof(int) << 1)); // 00000000... | 11111111......
  void* result = (void*)((big1 & (long long int)t) | (big2 & (long long int)f)); // leave only one data pointer according to IF/OR logic, based on cond value
  void* (*resulting_function)() = (void* (*)())result; // convert back to the function pointer
  resulting_function(); // call a function
}

unsigned int dist_uint(unsigned int a, unsigned int b){
  if(b > a){ swap_in_place(&a, &b); }
      return a - b;
}
  
  unsigned int dist_int(unsigned int a, unsigned int b){
    return dist_uint(a ^ ((unsigned int)MAX_INT32 + 1u), b ^ ((unsigned int)MAX_INT32 + 1u));
}
  

int integer_binary_search(int num, int (*func)(int, f_error*), int start, unsigned int size, unsigned int precision, f_error* err){
  if(!err){ return num; }
  int middle, func_of_middle, func_of_start, dis_num_to_func_of_start, func_of_end, dis_num_to_func_of_end;
  while(1){
    middle = safe_int_addition(start, size / 2, err); if(*err){ return middle; }
    func_of_middle = (*func)(middle, err); if(*err){ return middle; }

    if(num == func_of_middle){ return middle; }
    size = safe_int_addtiion(middle, -start, err); if(*err){ return middle; }
    if(func_of_middle < num){ start = middle; } 
    if(size <= precision){
      func_of_start = (*func)(start, err); if(*err){ return middle; }
      dis_num_to_func_of_start = safe_int_addition(num, -func_of_start, err); if(*err){ return middle; }

      func_of_end = (*func)(start + (int)size, err); if(*err){ return middle; }
      dis_num_to_func_of_end = safe_int_addition(num, -func_of_end, err); if(*err){ return middle; }
      return ternary(dis_num_to_func_of_start <= dis_num_to_func_of_end, start, size); 
    }
  }
}



// leave only one data pointer according to IF/OR logic, based on cond value
//
// JAVA code -> byte code () <-> machine code ()
//      Compilation       Interpretation
// C code ---------------------> machine code ()
// C code -> LLVM frontend <-> LLVM backend -> machine code()
// Microsoft -> Microsoft Visual Studio
// GNU project -> GNU C Compiler (GCC)
// GNU project -> GNU Image Manipulation Program (GIMP)
//                   140 * sizeof(int)
// ..................

// systax sugar
// (void* (*)()) typecasting to function pointer with return type void* and no arguments
