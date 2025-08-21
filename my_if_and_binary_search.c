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
void* conditional_operator(int cond, void* (*true_cond_func)(), void* (*false_cond_func)()){
  cond = !(!cond); // 1; 0
  void* t = true_cond_func; // convert function pointer to data pointer
  void* f = false_cond_func;
  int big = cond << (sizeof(int) << 1); // 0/0010000....
  int r = (~0) >> big; // 00000000... | 11111111......
  // void* result = (r & t) | (r & f); // leave only one data pointer according to IF/OR logic, based on cond value
  // void* (*resulting_function)() = (void* (*)())result; // convert back to the function pointer
  resulting_function(); // call a function
  // return result;
}
  
int absolute_value(int condition){ 
    int the_original_value = condition;
    condition = *((unsigned int*)&condition) >> ((sizeof(int) << 3) - 1);// condition if last bit of arg is 1(if number is negative) 
    return (1 - (condition << 1)) * the_original_value;
}
  
typedef enum function_state{
 NO_PROBLEM = 0,
 ROUNDING,
 F_OVERFLOW,
 F_UNDERFLOW,
}error;
    

unsigned int dist_uint(unsigned int a, unsigned int b){
  if(b > a){ swap_in_place(&a, &b); }
      return a - b;
}
  
  unsigned int dist_int(unsigned int a, unsigned int b){
    return dist_uint(a ^ ((unsigned int)MAX_INT + 1u), b ^ ((unsigned int)MAX_INT + 1u));
}
  

int integer_binary_search_v2(int num, int (*func)(int, error*), int left, int right, int precision, error* err) { 
  if(!err){ return num; }
  if(left > right) { swap_in_place(&left, &right); }
  int sum, middle, var;

  while(1) {
    *err = safe_int_addition(left, right, &sum);
    if(*err){ return middle; }
    middle = sum / 2;
  
    var = (*func)(middle, err);  
    if((*func)(middle, err) == num){ return middle; };
    if((right - left) <= precision) {
      (*err) = ROUNDING;
      int dis_left_2 = num - (*func)(left, err); // distance between 'num' and value of function called with argument 'left'
      int dis_right = num - (*func)(right, err); // distance between 'num' and value of function called with argument 'right'
      int case_dis_right_bigger = (dis_right >= dis_left_2) << (sizeof(int) << 3);////in case of distance from func(right) to num bigger than distance from func(left), this variable will be certain big number which than totaly shifts right to 0, else it will be 0 and will return right
      int case_dis_left_bigger = (dis_left_2 >= dis_right) << (sizeof(int) << 3);//in case of distance from func(left) to num bigger than distance from func(right), this variable will be certain big number which than totaly shifts left to 0, else it will be 0 and will return left
      return (right >> case_dis_right_bigger) | (left >> case_dis_left_bigger);
    }
  
    if(var < num) { left = middle; }
    if(var > num) { right = middle; }
  }     
  return middle;
}

int integer_binary_search(int num, int (*func)(int, error*), int start, unsigned int size, unsigned int precision, error* err){
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
