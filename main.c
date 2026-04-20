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
  matrix_t m = matrix_create(UINT32, 101, 175);
  pix_from_k_to_m_color(m, (uint32_bytes){ .parts.b1 = 255, .parts.b2 = 255, .parts.b3 = 255 }, 0, 17675);
  vertical_line_width_n_offset_k(m, (uint32_bytes){ .parts.b1 = 0, .parts.b2 = 0, .parts.b3 = 255 }, 10, 50);
  horizontal_line_width_n_offset_k(m, (uint32_bytes){ .parts.b1 = 0, .parts.b2 = 0, .parts.b3 = 255 }, 10, 45);
  file_filler("pic.ppm", m);
}

/*TODO LIST  : FMPG
 graphic:
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