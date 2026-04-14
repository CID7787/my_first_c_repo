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

void float_n_to_uint_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
  if(!(from_ptr && to_ptr && sec_arg && err)){
      if(err){ *err = NULL_POINTER; }
      return;
  } 
  int32_t i, exp, cond = (from_s == 8) * 3;// from_s == 8 condition to check if type variavle is type double 
  uint64_t mant = 0;
  exp = (from_ptr[from_s - 1] & 0x7f) << 1;
  exp <<= cond;
  exp |= from_ptr[from_s - 2] >> (7 - cond);
  for(i = from_s - 1; i--; ){ mant <<= 8; mant |= from_ptr[i]; }
  mant &= ternary(cond, MAX_DOUBLE_MANTISSA, MAX_FLOAT_MANTISSA);
  mant |= ternary(cond, DOUBLE_MANTISSA_IMPLICIT_ONE, FLOAT_MANTISSA_IMPLICIT_ONE);
// error check
  *err = ternary(exp > ternary(cond, MAX_NORM_DOUBLE_EXP, MAX_NORM_FLOAT_EXP), SNAN, *err);
  mant = else0(exp, mant);// if exp == 0, it amplies that mant must be 0 too, not mantissa implicit one number 
  exp -= ternary(cond, DOUBLE_EXP_BIAS, FLOAT_EXP_BIAS);
  *err = ternary((exp < 0) && mant, UNDERFLOW, *err);
  cond = from_s << 3;
  // error check
  *err = ternary(from_ptr[from_s - 1] >> 7, NEGATIVE_OVERFLOW, *err);
  *err = ternary(exp > cond, POSITIVE_OVERFLOW, *err);
  exp = ((int64_t)ternary(cond, AMOUNT_OF_DOUBLE_MANTISSA_BITS, AMOUNT_OF_FLOAT_MANTISSA_BITS) - exp) % cond;
  for(i = cond = 0; i < exp; i++){ cond <<= 1; cond |= 1; }
  cond = (mant & cond);
  *err = ternary(cond && (exp > 0), UNDERFLOW, *err);
  mant = ternary(exp < 0, mant << -exp, mant >> exp);
  for(i = cond = exp = 0; i < to_s; i++){
      to_ptr[i] = (mant >> (8 * i)) & 0xff; 
      cond |= sec_arg[i];
  }
  for(; i < 8; i++){ 
      exp |= (mant >> (8 * i)) & 0xff;
      cond |= sec_arg[0]; 
  }
  *err = ternary(exp && cond, POSITIVE_OVERFLOW, *err);
}

void f(int a){}

int main(){
  double  d = -0;
  uint32_t* to_p = malloc(4);
  uint64_t sec = 8;
  error err = 0;
  float_n_to_uint_k_type_cast((uint8_t*)&d, 8, (uint8_t*)to_p, 4, (uint8_t*)&sec, &err);
  printf("%u\t%d", *to_p, err);
  return 0;
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