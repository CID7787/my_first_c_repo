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
#include "type_cast_functions.c"
#include "safe_arithmetic_functions.c"
#include "image_functions.c"
#include "new_vector_functions.c"
#include "matrix_functions.c"
#include "print_binary.c"



int main(){
  matrix_t a = matrix_create(UINT8, 3, 2), b = matrix_create(FLOAT32, 2, 2);
  a.elements.ui8 = (uint8_t[]){1, 2,
                             2, 1,
                             3, 1};
  b.elements.f32 = (float[]){2, 2,
                             3, 1};
  matrix_t r = matrix_mult_first_arg_t(a, b);
  print_matrix(r);
  return 0;
}

/*TODO LIST  : FMPG
 graphic:
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
*/
