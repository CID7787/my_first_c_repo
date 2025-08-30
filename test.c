#ifndef headerfile
    #include <stdint.h>
    #include <stdlib.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
#endif



// 1. You want to preserve type
// 2. You want to preserve value

// 1. You want to bring both to a smaller one 
// 2. You want to bring both to a larger one
/*

if (a.type != DOUBLE
&& a.type != FLOAT
&& b.type != DOUBLE
&& b.type != FLOAT) {
    r.element_size = max(a.element_size, b.element_size);
    if (r.element_size == a.element_size) { recast(b, a.type); }
    if (r.element_size == b.element_size) { recast(a, b.type); }
    }
*/

union one_byte_all_types{
    int8_t i;
    uint8_t ui;
};
union four_byte_all_types{
    int32_t i;
    uint32_t ui;
    float f;
};
union eight_byte_all_types{
    int64_t i;
    uint64_t ui;
    double d;
};
typedef union variable_all_types{
    union one_byte_all_types b1;
    union four_byte_all_types b4;
    union eight_byte_all_types b8;
    double d;
    long int li;
    long unsigned int lui;
    char c;
    float f;
    int i;
    unsigned int ui;
} all;

union POINTERS_TO_ALL_DATA_TYPES {
    union one_byte_all_types* b1;
    union four_byte_all* b4;
    union eight_byte_all* b8;
} typedef alldatapointer;


struct vector_n {
    datatype type;
    unsigned int n;
    unsigned int element_size;
    all* elements;
    error v_error;
} typedef vecN;

vecN vector_exponentiation(vecN a, vecN b){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n-- && !r.v_error){
        switch(a.type){
            case DOUBLE:
                switch(b.type){
                    case DOUBLE: break;
                    case FLOAT: break;
                    default: break;
                }
            case FLOAT:
                switch(b.type){
                    case DOUBLE: break;
                    case FLOAT: break;
                    default: break;
                }
                default:
                switch(b.type){
                    case DOUBLE: break;
                    case FLOAT: break;
                    default:
                    // integer types
                        r.element_size = max(a.element_size, b.element_size);
                        if (r.element_size == a.element_size) { recast(b, a.type); }
                        if (r.element_size == b.element_size) { recast(a, b.type); }
                        break;
                }
        }  
        
    }
}

