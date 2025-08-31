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

typedef union one_byte_all_types{
    int8_t i;
    uint8_t ui;
}one_byte_dt;

typedef union four_byte_all_types{
    int32_t i;
    uint32_t ui;
    float f;
}four_byte_dt;

typedef union eight_byte_all_types{
    int64_t i;
    uint64_t ui;
    double d;
}eight_byte_dt;

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
    one_byte_dt* b1;
    four_byte_dt* b4;
    eight_byte_dt* b8;
} typedef alldatapointer;

struct vector_n {
    datatype type;
    unsigned int n;
    unsigned int element_size;
    alldatapointer elements;
    error v_error;
} typedef vecN;




vecN vector_exponentiation(vecN a, vecN b){
    vecN r = {a.type, a.n, malloc(a.n * a.element_size), NO_ERROR};
    unsigned int cond;
    while(a.n--){
        switch(a.type){
            case DOUBLE:
                switch(b.type){
                    case DOUBLE: r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b8[a.n].d); break;
                    case FLOAT:  r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b4[a.n].f); break;
                    default: 
                        r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b1[a.n].i);
                        r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b1[a.n].ui);
                        r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b4[a.n].i);
                        r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b4[a.n].ui);
                        r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b8[a.n].i);
                        r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b8[a.n].ui);
                    break;

                }
            case FLOAT:
                switch(b.type){
                    case DOUBLE: r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b8[a.n].d); break;
                    case FLOAT: r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b4[a.n].f); break;
                    default: 
                        r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b1[a.n].i);
                        r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b1[a.n].ui);
                        r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b4[a.n].i);
                        r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b4[a.n].ui);
                        r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b8[a.n].i);
                        r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b8[a.n].ui);    
                    break;
                }
                default:
                switch(b.type){
                    case DOUBLE: 
                        r.elements.b1[a.n].i  = pow(a.elements.b1[a.n].i,  b.elements.b8[a.n].d);
                        r.elements.b1[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b8[a.n].d);
                        r.elements.b4[a.n].i  = pow(a.elements.b4[a.n].i,  b.elements.b8[a.n].d);
                        r.elements.b4[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b8[a.n].d);
                        r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i,  b.elements.b8[a.n].d);
                        r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b8[a.n].d);
                    break;
                    case FLOAT: 
                        r.elements.b1[a.n].i  = pow(a.elements.b1[a.n].i,  b.elements.b4[a.n].f);
                        r.elements.b1[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b4[a.n].f);
                        r.elements.b4[a.n].i  = pow(a.elements.b4[a.n].i,  b.elements.b4[a.n].f);
                        r.elements.b4[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b4[a.n].f);
                        r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i,  b.elements.b4[a.n].f);
                        r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b4[a.n].f);
                    
                    break;
                    default:
                        r.element_size = max(a.element_size, b.element_size);
                        if (r.element_size == a.element_size) { b.type = a.type; b.element_size = a.element_size; }
                        else{ a.type = b.type; a.element_size = b.element_size; }
                        r.elements.b1[a.n].i  = pow(a.elements.b1[a.n].i , b.elements.b1[a.n].i);
                        r.elements.b1[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b1[a.n].ui);
                        r.elements.b4[a.n].i  = pow(a.elements.b4[a.n].i , b.elements.b4[a.n].i);
                        r.elements.b4[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b4[a.n].ui);
                        r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i , b.elements.b8[a.n].i);
                        r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b8[a.n].ui);
                        break; 
                }
        }  
        
    }
}

