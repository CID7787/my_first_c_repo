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
#include "new_vector_functions.c"
// #include "test_file.c"
#include "print_binary.c"



void main(){  
    int width  = 10,
        height = 10,
        size = width * height * 12;
    char image_data[size + 1];
    printf("P3\n%d %d\n255\n", width, height);
    for(int i = 0; i < size; i += 12){
        image_data[i + 0] = '0';
        image_data[i + 1] = '5';
        image_data[i + 2] = '5';
        image_data[i + 3] = ' ';

        image_data[i + 4] = '1';
        image_data[i + 5] = '2';
        image_data[i + 6] = '0';
        image_data[i + 7] = ' ';
        
        image_data[i + 8] = ' ';
        image_data[i + 9] = ' ';
        image_data[i +10] = '0';
        image_data[i +11] = '\n';
        // off by 1 error  https://en.wikipedia.org/wiki/Off-by-one_error
    }
    image_data[size] = 0;
    printf("%s", image_data);
}

/*

safe_int_addition(void* a, negation(b));

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

// 0 0 0 0 0 0 0 0 0
// MEMORY   INTERPRETATION
              VECTOR
              MATRIX
              ...
// 0 0 0 0 0 0 0 0 0


   MEMORY    INTERPRETATION
// 0101        INT
               UINT
               FLOAT
               ...
// 


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

only matrix:
    matrix_multiplication



unsigned int l = len([3505050505, 45050500505]); // = sqrt(3505050505*3505050505 + 45050500505*45050500505) = MAX_UINT > x > MAX_INT

addition(v, negation(l));
            // -x?
v - l;
j - l;
hello



memory allocation          create_matrix(type, rows, cols, pointer)   allocate memory
memory initialization      populate_matrix()    filling the allocated memory with values

data processing            transform_matrix()

Option #1: group allocation and initalization
                            create_matrix(type, rows, cols, pointer, extra_parameter_related_to_initialization)

Option #2: group initalization and processing

                            matrix_t M = create_matrix(type, rows, cols, pointer) // default [ 1 0 ]
                                                                                             [ 0 1 ]
                            multiply(M, 2)


create_matrix(type, rows, cols, pointer, int (*initialization_function_pointer)(matrix_t M))

int fill_with_zeros(matrix_t M){    
    uint32_t size = M.rows * M.cols;
    while(size--){
        // M[i] = 0;
    }
}


matrix_t A = create_matrix(int, 10, 10, ptr, &fill_with_zeros);
matrix_t B = create_matrix(int, 10, 10, ptr);

matrix_t B = create_matrix(int, 10, 10, fill_with_identity_diagonal(10, 10));


matrix_t C = create_matrix(int, 10, 10, NULL); zigzag_fill_matrix(C, 10, 20, 'A', '*');


int fill_with_identity_diagonal(matrix_t M){
    uint32_t size = M.rows * M.cols;
    while(size--){
        // M[size] = !(i % (Math.min(M.rows, M.cols) + 1));
    }
}

matrix_t C = create_matrix(int, 10, 10, NULL, &func);
*/