#include <stdio.h>
#include <stlib.h>

const int SMALLEST_INT_NUMBER = ( 1 << ((sizeof(int) << 3) - 1) );
const int BIGGEST_INT_NUMBER = ( ~(1 << ( (sizeof(int) << 3) - 1) ) ); 


int logical_not(int arg){
    // 0100
    // 0001
    // ???1
    int last_bit =  arg ^ 1; // => ???! // 0101 1 last bit = -(last bit)
    last_bit  = last_bit & 1;
    int previous_last_bit = (arg >> 1) ^ 1; // => 0??! // 0011 1 last bit = -(previous last bit)
    previous_last_bit = previous_last_bit & 1;
    int second_bit = (arg >> 2) ^ 1; // => 00?! // 0000 0 last bit = -(second bit)
    second_bit = second_bit & 1;
    int first_bit = (arg >> 3) ^ 1; // => 000! // 0001 1 last bit = -(first bit)
    first_bit = first_bit & 1;
    return last_bit & previous_last_bit & second_bit & first_bit; // == 0 if at least one 0, == 1 in every other case
  }
  
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
  void* conditional_operator(int cond, void* (*true_condition_func)(), void* (*false_condition_func)()){
    cond = !(!cond); // 1; 0
    void* t = (void*) true_condition_func; // convert function pointer to data pointer
    void* f = (void*) false_condition_func;

    int big = ( cond << sizeof(int) ) << sizeof(int); // 0/0010000....
    int r = (~0) >> big; // 00000000... | 11111111......

    void* result = (r & t) | (r & f); // leave only one data pointer according to IF/OR logic, based on cond value

    void* (*resulting_function)() = (void* (*)())result; // convert back to the function pointer
    resulting_function(); // call a function
    return result;
    
  }
  
int absolute_value(int condition){ 
    int the_original_value = condition;
    condition = *((unsigned int*)&condition) >> ((sizeof(int) << 3) - 1);// condition if last bit of arg is 1(if number is negative) 
    return (1 - (condition << 1)) * the_original_value;
}

int trueXfalse0(unsigned int cond, int x){
  unsigned int n_bits = sizeof(int) << 3;// sizeof(int) in bits, number of bits in registry. (x << 3) is the same as  (x * 8)
  int any = (!cond) << n_bits;
  return x >> any;// if(cond){return x;} else{ return 0;}
}

void ifnot(void* a, void* b){
  int* c = (int*)a; 
  int* d = (int*)b;
  //    if(!(*c)){ *c = *d; }    
  *c = trueXfalse0(*c, *d) | trueXfalse0(!(*c), *c);
}
  
  
typedef enum function_state{
 NO_PROBLEM = 0,
 ROUNDING,
 OVERFLOW,
 UNDERFLOW,
}error;
    

void swap_in_place(int* a, int* b){// through xor operator
  (*a) = (*a) ^ (*b);// xor of couple numbers like this 0101 and 0110 equal to 0011
  (*b) = (*a) ^ (*b);// then xor of result with any of these two numbers will result to another number, ex_1:0011 ^ 0101 = 0110
  (*a) = (*a) ^ (*b);// ex_2: 0011 ^ 0110 = 0101
}


int safe_int_sum(int a, int b, error* e){ 
  long int temp_sum = ( (long int)a ) + ( (long int)b );
  int cond = trueXfalse0(temp_sum > BIGGEST_INT_NUMBER, OVERFLOW) | trueXfalse0(temp_sum < SMALLEST_INT_NUMBER, UNDERFLOW);
  *e = trueXfalse0(cond, cond) | trueXfalse0(!cond, *e);
  return (int)temp_sum;
}


unsigned int dist_uint(unsigned int a, unsigned int b){
  if(b > a){ swap_in_place(&a, &b); }
      return a - b;
}
  
  unsigned int dist_int(unsigned int a, unsigned int b){
    return dist_uint(a ^ ((unsigned int)BIGGEST_INT_NUMBER + 1u), b ^ ((unsigned int)BIGGEST_INT_NUMBER + 1u));
}
  

int integer_binary_search_v2(int num, int (*func)(int, error*), int left, int right, int precision, error* err) { 
  if(left > right) { swap_in_place(&left, &right); }
  int sum, middle, var;

  while(1) {
    (*err) = safe_int_sum(left, right, &sum);
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
  int middle;
  while(1){
    middle = safe_int_sum(start, size / 2, err); if(*err){ return middle; }
    int func_of_middle = (*func)(middle, err); if(*err){ return middle; }

    if(num == func_of_middle){ return middle; }
    size = safe_int_sum(middle, -start, err); if(*err){ return middle; }
    if(func_of_middle < num){ start = middle; } 
    if(size <= precision){
      int func_of_start = (*func)(start, err); if(*err){ return middle; }
      int dis_num_to_func_of_start = safe_int_sum(num, -func_of_start, err); if(*err){ return middle; }

      int func_of_end = (*func)(start + (int)size, err); if(*err){ return middle; }
      int dis_num_to_func_of_end = safe_int_sum(num, -func_of_end, err); if(*err){ return middle; }
      return trueXfalse0(dis_num_to_func_of_start <= dis_num_to_func_of_end, start) | trueXfalse0(dis_num_to_func_of_end < dis_num_to_func_of_start, end); 
    }
  }
}



float newton_method_func(float (*func)(float), float (*derivative)(float), float x0, float precision){
    float x = x0;
    while( ((*func)(x) >= precision) || ((*func)(x) <= -precision) ){// WANING!!! precision works only in this case(need to consider what precision need to put for other cases) 
        x = x - ( (*func)(x) / (*derivative)(x) );// WARNING!!! division by 0 
    }
    return x;
}


int main(){
    printf("%f", newton_method(&square, &square_derivative, 6, 0.1));
    
    return 0;
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
