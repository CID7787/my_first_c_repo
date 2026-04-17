#include <stdio.h> 
#include <time.h>  
#include <stdlib.h>
#include <math.h>  
#include <stdint.h> 
#define headerfile 1

#include "constants.c"
#include "user_defined_datatypes.c"
#include "logical_functions_of_decision.c"
#include "additional_functions.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
#include "image_functions.c"
// #include "new_vector_functions.c"
// #include "matrix_functions.c"
#include "print_binary.c"

uint64_t m = 0x1ffffffffffff;


void float_n_to_float_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
  if(!(from_ptr && to_ptr && sec_arg && err)){
      if(err){ *err = NULL_POINTER; }
      return;
  }
  int8_t i, zero_cond = 0;
  if(from_s == to_s){ for(i = 0; i < from_s; i++){ to_ptr[i] = from_ptr[i]; } return; }
  fbits f;
  dbits d;
  if(to_s > from_s){
    for(i = 4; i--; ){ 
      f.uint <<= 8; 
      f.uint |= from_ptr[i]; 
      zero_cond |= sec_arg[i] | sec_arg[i + 4];
    }
    *err = ternary(f.parts.exp > MAX_NORM_FLOAT_EXP, SNAN, *err);
    *err = ternary(!(f.parts.exp) && f.parts.mantissa && zero_cond, UNDERFLOW, *err);
    d.parts.mantissa = f.parts.mantissa;
    d.parts.mantissa <<= 29; // 29 == (AMOUNT_OF_DOUBLE_MANTISSA_BITS - AMOUNT_OF_FLOAT_MANTISSA_BITS)
    d.parts.exp = (f.parts.exp - FLOAT_EXP_BIAS + DOUBLE_EXP_BIAS) & -(!!f.parts.exp);
    d.parts.sign = f.parts.sign;
    for(i = 0; i < 8; i++){ to_ptr[i] = (d.luint >> (i * 8)) & 0xff; }
  }
  else{
    for(i = 8; i--; ){ 
      d.luint <<= 8; 
      d.luint |= from_ptr[i];
      zero_cond |= sec_arg[i % 4];
    }
    *err = ternary(d.parts.exp > MAX_NORM_DOUBLE_EXP, SNAN, *err);
    *err = ternary(!(d.parts.exp) && d.parts.mantissa && zero_cond, UNDERFLOW, *err);
    i = zero_cond && (how_many_0_until_youngest_1(d.parts.mantissa) < 29);// 29 == amount_of_double_mantissa_bits - amount_of_float_mantissa_bits
    *err = ternary(i, UNDERFLOW, *err);
    f.parts.mantissa = d.parts.mantissa >> 29;// 29 == amount_of_double_mantissa_bits - amount_of_float_mantissa_bits
    d.parts.exp -= (DOUBLE_EXP_BIAS + FLOAT_EXP_BIAS) & -(!!(d.parts.exp));
    *err = ternary((d.parts.exp > MAX_NORM_FLOAT_EXP) && zero_cond, ternary(d.parts.sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
    *err = ternary((d.parts.exp < 0) && zero_cond, UNDERFLOW, *err);
    f.parts.exp = (uint32_t)d.parts.exp % (MAX_NORM_FLOAT_EXP + 1);
    f.parts.sign = d.parts.sign;
    for(i = 0; i < 4; i++){ to_ptr[i] = (f.uint >> (i * 8)) & 0xff; }
  } 
}

void f(){
  double d = 1;
  float *f = malloc(4), sec = 3;
  error err = 0;
  float_n_to_float_k_type_cast((uint8_t*)&d, 8, (uint8_t*)f, 4, (uint8_t*)&sec, &err);
  printf("\n%f\t%d", *f, err);

}

int main(){
  f();
}

/*TODO LIST  : FMPG
 graphic:
  left side = color_1, right side = color_2
  top = color_1, bottom = color_2
  checkerboard pattern:
    even pixel = color_1, odd_pixel = color_2
    N pixels = color_1, N next pixels = color_2
  vertical line
    1px at any position
    of width N
    of width N with offset X
  horizontal line
    1px at any position
    of width N
    of width N with offset Y
  coordinate axis:
    1 vertical line + 1 horizontal line, intersecting at the center of the image
  horizontal gradient:
    left side: red = 0; green = 0; blue = 0; right side: red = 255, green = 0, blue = 0
    BLACK                      RED
    ______________________________
    |                            |
    |                            |
    |                            |
  vertical gradient:
    top: red =0; green = 0; blue = 0; bottom: red = 0; green = 255; blue = 0;
  diagonal gradient:
    top left corner: red = 0; green = 0; blue = 0; bottom right corner: red = 255;
/*



/*"
RGB CMBK ....

char* internal_memory_buffer = "P6\n10 10\n255\n000000";

pointer = malloc(4 + 4 + elements*type)
N TYPE ERROR elements
pointer2 = pointer1


void*
N TYPE ERROR element_pointer actual_elements
vecN a = *((vecN*)malloc(1 + 1 + 1 + elements_size));



tagged_union negatiation(b){
    alldatapointer = ...; // you have put long int* into the data pointer
    tagged_union.tag = LINT;
    tagged_union.data = alldatapointer;
    return tagged_union;
}
    

1. They show me how
2. I copy
3. they give me a different example that is nothing like what they show me
    or where the method doesn't work
4. I am confused, but I need to figure out how to solve it

1. ?
2. X

1. Solve the simplest case possible
2. Solve several variations
3. Find a pattern
4. Generalize the pattern  << THE first time, it will be very inefficient
5. See the problems.
6. Solve the problems.


Prisoner's dilemma


memory allocation          create_matrix(type, rows, cols, pointer)   allocate memory
memory initialization      populate_matrix()    filling the allocated memory with values

data processing            transform_matrix()

Option #1: group allocation and initalization
                            create_matrix(type, rows, cols, pointer, extra_parameter_related_to_initialization)

Option #2: group initalization and processing

                            matrix_t M = create_matrix(type, rows, cols, pointer) // default [ 1 0 ]
                                                                                             [ 0 1 ]
                            multiply(M, 2)


*/