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
// #include "new_vector_functions.c"
// #include "test_file.c"
#include "print_binary.c"

void main(int argc, char *argv[] )  {  
  
    printf("Program name is: %s\n", argv[0]);  
    
    if(argc < 2){  
       printf("No argument passed through command line.\n");  
    }  
    else{  
       printf("First argument is: %s\n", argv[1]);  
    }  
 }  