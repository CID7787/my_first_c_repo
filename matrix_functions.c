#ifndef headerfile
    #include <stdio.h> 
    #include <time.h>  
    #include <stdlib.h>
    #include <math.h>  
    #include <stdint.h> 
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "additional_functions.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
    #include "new_vector_functions.c"
#endif


/*
What if memory is smaller than N? Then, we access it out of bounds. That's a problem. What do we do?
A) Do not touch, make sure logic is airtight
B) Write checks
    1) every time, within your program, you re-write checks
    2.1) you write checks 1 time, put them in a set of structs and functions
    2.2) just use it everywhere

Memory safety is a big, unsolved problem. Different languages have different approaches:
    Rust - Borrow Checker
    JS, Python, ... - automatic memory management
    Swift, Zig - compiler checks
    Fil-C  - C library

*/
/*
Errors:
    Terminating errors
        some bit is always 1
    Non-terminating errors
        that same bit is always 0
    if (error >> 31) { return; } // terminating error
    else {} // non-terminating error


*/