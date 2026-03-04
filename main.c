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
    time_t start = clock();
    vecN a = {DOUBLE, 4, B8type_d_elements((double[]){1,2,3,4}), NO_ERROR};
    vector_negation_in_place_v2(a);
    print_vector(a);
    printf("\ntime%llu", clock() - start);
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