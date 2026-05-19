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

/*
int to int, to uint, to float
uint to uint, to int, to float
float to float
*/



int main(){
  matrix_t m = matrix_create(UINT32, 100, 100);
  vertical_gradient(m, (uint32_bytes){ .parts.b1 = 220, .parts.b2 = 0, .parts.b3 = 230}, (uint32_bytes){ .parts.b1 = 255, .parts.b2 = 165, .parts.b3 = 0});
  file_filler("pic.ppm", m);
  // // print_matrix((matrix_t){ .elements = m.elements, .type = (datatype[]){UINT8}, .col = (uint32_t[]){40}, .row = (uint32_t[]){1}, .err = m.err});
  // free(m.elements.ui32);
  return 0;
}

/*Ask Amirako why compiler throws("free(): invalid size
                                   zsh: IOT instruction (core dumped)  ./main.exe ")
int main(){
  matrix_t m = matrix_create(UINT32, 1, 10);
  free(m.elements.ui32);
  return 0;
}
*/

/*TODO LIST  : FMPG
graphic:
  horizontal gradient:
  vertical gradient:
  diagonal gradient:
    top left corner: red = 0; green = 0; blue = 0; bottom right corner: red = 255;
  circle:
*/
