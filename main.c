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



void main()  {  
   matrix_t a = {DOUBLE, 3, 3, B8type_d_elements((double[]){2, 1, 0, 0, 2, 1, 0, 0, 2}), NO_ERROR},
            c = matrix_multiplication(a, a), 
            b = matrix_multiplication(c, a);
   print_matrix(b);
}  