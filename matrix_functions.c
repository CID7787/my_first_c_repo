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

#define MATRIX_MAX_ELEM_BYTE_SIZE MAX_UINT32


matrix_t matrix_create(datatype type, uint32_t row, uint32_t col){
    uint32_t condition = ((row * col * amount_of_type_bytes(type)) <= MATRIX_MAX_ELEM_BYTE_SIZE);
    row &= -condition;
    void* r = malloc(sizeof(datatype) 
                + (sizeof(uint32_t) << 1) 
                + sizeof(error) 
                + (row * col * amount_of_type_bytes(type)) );

    matrix_t result;
        result.type     = (datatype*)   r;
        result.row      = (uint32_t*)  (result.type + 1);
        result.col      = (uint32_t*)  (result.row  + 1);
        result.err      = (error*)     (result.col  + 1);
        result.elements = (datapointer)(result.err  + 1);

    result.type[0] = type;
    result.row [0] = row;
    result.col [0] = col;
    result.err [0] = ternary(condition, NO_ERROR, MEMORY_LIMIT_EXCESS);
    return result;
}


matrix_t matrix_filler(matrix_t m, datapointer arr){// TODO: what if amount of elements in data is less than n
    if(!(m.type && m.row && m.col && m.err && m.elements.i32 && arr.i32)){ 
        if(m.err){ m.err[0] = NULL_POINTER; }
        return m;
    }
    uint32_t i = m.row[0] * m.col[0] * amount_of_type_bytes(m.type[0]);
    while(i--){ m.elements.i8[i] = arr.i8[i]; }
    return m;
}


matrix_t matrix_scaler_in_place(matrix_t m, datatype scl_type, fundtypeunion scale){// TODO: what if amount of elements in data is less than n
    if(m.row && m.col){ 
        if(m.err){ m.err[0] = NULL_POINTER; }
        return m; 
    }
    uint32_t n = uint32_mult(m.row[0], m.col[0], m.err);
    vecN vec = { .type = m.type, .err = m.err, .elements = m.elements, .n = &n };
    return m;
}


matrix_t matrix_add_first_arg_type(matrix_t a, matrix_t b){// TODO: what if amount of elements in data is less than n
    if(!(a.col && a.row && a.elements.i32 && a.err && a.type && b.col && b.row && b.elements.i32 && b.err && b.type)){ 
        if(a.err){ a.err[0] = NULL_POINTER; }    
        return a; 
    }
    if((a.row[0] ^ b.row[0]) | (a.col[0] ^ b.col[0])){ return a; }
}

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