#include <stdio.h> 
#include <time.h>  
#include <stdlib.h>
#include <math.h>  
#include <stdint.h> 
#define headerfile 1

#include "user_defined_datatypes.c"
#include "constants.c"
#include "additional_functions.c"
#include "logical_functions_of_decision.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
#include "matrix_functions.c"
#include "image_functions.c"
#include "new_vector_functions.c"
#include "print_binary.c"



int main(){
    int* arr = malloc(20 * sizeof(int));
    for(int i = 0; i < 20; i++)
        arr[i] = i + 1;
    arr = realloc(arr, 10 * sizeof(int));
    for(int i = 0; i < 20; i++)
        printf("%d\n", arr[i]);
    return 0; 
}

/*
RGB CMBK ....

char* internal_memory_buffer = "P6\n10 10\n255\n000000";

pointer = malloc(4 + 4 + elements*type)
N TYPE ERROR elements
pointer2 = pointer1


enum vecN_structure {
    N = 0,
    TYPE,
    ERROR,
    ELEMENTS,
}
pointer a, b;

(_______________)



#define ACCESS(TYPE, ARRAY, INDEX) ((TYPE*)(ARRAY+INDEX))
ACCESS(int, a, ELEMENTS);
(int*)(a + ELEMENTS);
b[ELEMENTS];
sizeof()
a.elements

void*
N TYPE ERROR element_pointer actual_elements
vecN a = *((vecN*)malloc(1 + 1 + 1 + elements_size));



tagged_union negatiation(b){
    alldatapointer = ...; // you have put long int* into the data pointer
    tagged_union.tag = LINT;
    tagged_union.data = alldatapointer;
    return tagged_union;
    }
    
    both matrix and vector operations are equivalent:
    negation
    addition
    elementwise_multiplication
    exponentiation
    
    only Matrix:
    matrix multiplication
    
    
    vector_method(){
        // ...
        }
        
matrix_method(){
   // a bit of preparation
   vector_method(M)
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


matrix_method(M){
    vector_n v = turn_pointer_into_vector_n(M.datapointer, M.rows * M.cols, );
    vector_method(v)
    error_rerouting
}

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