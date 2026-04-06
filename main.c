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


int main(){
    FILE* fptr = fopen("pic.ppm", "w");
    char str[100000] = {"P6\n100 100\n255\n"};
    uint32_t i, d; 
    for(i = 0; str[i]; i++){}
    for(d = i + 30000; i < d; i+=3){
        str[i]  = 255;
        str[i + 1] = 1; 
        str[i + 2] = 1;
    }
    str[i] = 0;
    fprintf(fptr, "%s", str);
    fclose(fptr);
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