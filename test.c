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


int function(void){
    unsigned int array[] = { 1,325,2156,164,14};
    unsigned int variable = (unsigned int)*(array + 2 * sizeof(unsigned int));
    +(array, 2);
}
 
typedef union one_byte_all_types{
    int8_t i;
    uint8_t ui;
}B1type;

typedef union four_byte_all_types{
    int32_t i;
    uint32_t ui;
    float f;
}B4type;

typedef union eight_byte_all_types{
    int64_t i;
    uint64_t ui;
    double d;
}B8type;


union POINTERS_TO_ALL_DATA_TYPES {
    B1type* b1;
    B4type* b4;
    B8type* b8;
} typedef alldatapointer;

struct vector_n{
    datatype type;
    unsigned int n;
    unsigned char element_size;
    alldatapointer elements;
    error v_error;
} typedef vecN;




vecN vector_exponentiation(vecN a, vecN b){
    unsigned char a_elem_size = amount_of_type_bytes(a.type), b_elem_size = amount_of_type_bytes(b.type), r_element_size; 
    r_element_size = ternary(a_elem_size > b_elem_size, a_elem_size, b_elem_size);
    vecN r = {a.type, a.n, r_element_size, malloc(a.n * r_element_size), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case DOUBLE:
                switch(b.type){
                    case DOUBLE: r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b8[a.n].d);  break;
                    case FLOAT:  r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b4[a.n].f);  break; 
                    case CHAR:   r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b1[a.n].i);  break;
                    case UCHAR:  r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b1[a.n].ui); break;
                    case INT:    r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b4[a.n].i);  break;
                    case UINT:   r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b4[a.n].ui); break;
                    case LINT:   r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b8[a.n].i);  break;
                    case LUINT:  r.elements.b8[a.n].d = pow(a.elements.b8[a.n].d, b.elements.b8[a.n].ui); break;
                }
            case FLOAT:
                switch(b.type){
                    case DOUBLE: r.type = DOUBLE; r.elements.b8[a.n].d = pow(a.elements.b4[a.n].f, b.elements.b8[a.n].d); break;
                    case FLOAT:  r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b4[a.n].f);  break; 
                    case CHAR:   r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b1[a.n].i);  break;
                    case UCHAR:  r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b1[a.n].ui); break;
                    case INT:    r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b4[a.n].i);  break;
                    case UINT:   r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b4[a.n].ui); break;
                    case LINT:   r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b8[a.n].i);  break;
                    case LUINT:  r.elements.b4[a.n].f = pow(a.elements.b4[a.n].f, b.elements.b8[a.n].ui); break;
                }
            default:
                switch(b.type){
                    case DOUBLE: 
                        switch(a.type){
                            case CHAR:  r.elements.b1[a.n].i  = pow(a.elements.b1[a.n].i,  b.elements.b8[a.n].d); break;
                            case UCHAR: r.elements.b1[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b8[a.n].d); break;
                            case INT:   r.elements.b4[a.n].i  = pow(a.elements.b4[a.n].i,  b.elements.b8[a.n].d); break;
                            case UINT:  r.elements.b4[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b8[a.n].d); break;
                            case LINT:  r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i,  b.elements.b8[a.n].d); break;
                            case LUINT: r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b8[a.n].d); break;
                        }
                    break;
                    case FLOAT: 
                    switch(a.type){
                        case CHAR:  r.elements.b1[a.n].i  = pow(a.elements.b1[a.n].i,  b.elements.b4[a.n].f); break;
                        case UCHAR: r.elements.b1[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b4[a.n].f); break;
                        case INT:   r.elements.b4[a.n].i  = pow(a.elements.b4[a.n].i,  b.elements.b4[a.n].f); break;
                        case UINT:  r.elements.b4[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b4[a.n].f); break;
                        case LINT:  r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i,  b.elements.b4[a.n].f); break;
                        case LUINT: r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b4[a.n].f); break;
                    }

                    break;
                    default:
                        r.element_size = max(a.element_size, b.element_size);// TODO: delete this and stuff related to this too
                        if (r.element_size == a.element_size) { b.type = a.type; }
                        else{ a.type = b.type; }
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

