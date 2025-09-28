#ifndef headerfile
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
#endif

#define B1type_elements(...) (alldatapointer){ . b1 = (B1type)__VA_ARGS__} 
#define B4type_elements(...) (alldatapointer){ . b4 = (B4type)__VA_ARGS__} 
#define B8type_elements(...) (alldatapointer){ . b8 = (B8type)__VA_ARGS__} 

unsigned int amount_of_type_bytes(datatype t){
    switch(t) {
      case CHAR:   return sizeof(int8_t); break;
      case UCHAR:  return sizeof(int8_t); break;
      case INT:    return sizeof(int32_t); break;
      case UINT:   return sizeof(int32_t); break;
      case FLOAT:  return sizeof(float); break;
      case LINT:   return sizeof(int64_t); break;
      case LUINT:  return sizeof(int64_t); break;
      case DOUBLE: return sizeof(double); break;
      default:     return sizeof(datatype);
    }
}


vecN vector_creation(datatype type, unsigned int n, alldatapointer elements){
    unsigned int r_element_size = amount_of_type_bytes(type);
    vecN r = {type, n, malloc(n * r_element_size), NO_ERROR};
    int* ptr = elements.b1;
    while(n--){
        switch(type | -(!ptr)){
            case CHAR:   r.elements.b1[n].i  = elements.b1[n].i; break;
            case UCHAR:  r.elements.b1[n].ui = elements.b4[n].ui; break;
            case INT:    r.elements.b4[n].i  = elements.b4[n].i; break;
            case UINT:   r.elements.b4[n].ui = elements.b4[n].ui; break;
            case FLOAT:  r.elements.b4[n].f  = elements.b4[n].f; break;
            case LINT:   r.elements.b8[n].i  = elements.b8[n].i; break;
            case LUINT:  r.elements.b8[n].ui = elements.b8[n].ui; break;
            case DOUBLE: r.elements.b8[n].d  = elements.b8[n].d; break;
            case -1: switch(r_element_size){
                        case 1:   r.elements.b1[n].i = 0; break;
                        case 4:   r.elements.b4[n].i = 0; break;
                        case 8:   r.elements.b8[n].i = 0; break;
                     }
            break; 
        }    }
    return r;
}   


vecN vector_negation(vecN a){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n-- ){
        switch(r.type){
            case CHAR: 
                r.v_error = ternary(a.elements.b1[a.n].i == MIN_CHAR, POSITIVE_OVERFLOW, r.v_error); 
                r.elements.b1[a.n].i = -a.elements.b1[a.n].i;
            break;
            case UCHAR:
                r.elements.b1[a.n].ui = a.elements.b1[a.n].ui; 
            break;
            case INT: 
                r.v_error = ternary(a.elements.b4[a.n].i == MIN_INT, POSITIVE_OVERFLOW, r.v_error); 
                r.elements.b4[a.n].i =  -a.elements.b4[a.n].i; 
            break;
            case UINT: 
                r.elements.b4[a.n].ui = a.elements.b4[a.n].ui; 
            break;
            case FLOAT: 
                r.elements.b4[a.n].f = -a.elements.b4[a.n].f; 
            break;
            case LINT: 
                r.v_error = ternary(a.elements.b8[a.n].i == MIN_LINT, POSITIVE_OVERFLOW, r.v_error); 
                r.elements.b8[a.n].i = -a.elements.b8[a.n].i; 
            break;
            case LUINT: 
                r.elements.b8[a.n].ui = a.elements.b8[a.n].ui; 
            break;
            case DOUBLE: 
                r.elements.b8[a.n].d = -a.elements.b8[a.n].d; 
            break;
        }
    }
    return r;
}

vecN vector_addition(vecN a, vecN b){
    if(a.type != b.type){ return a; }
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case CHAR: 
                r.elements.b1[a.n].i = safe_char_addition(a.elements.b1[a.n].i, b.elements.b1[a.n].i, &r.v_error);
            break;
            case UCHAR:
                r.elements.b1[a.n].ui = safe_unsigned_char_addition(a.elements.b1[a.n].ui, b.elements.b1[a.n].ui, &r.v_error); 
            break;
            case INT: 
                r.elements.b4[a.n].i = safe_int_addition(a.elements.b4[a.n].i, b.elements.b4[a.n].i, &r.v_error); 
            break;
            case UINT:
                r.elements.b4[a.n].ui = safe_uint_addition(a.elements.b4[a.n].ui, b.elements.b4[a.n].ui, &r.v_error); 
            break;
            case LINT: 
            r.elements.b8[a.n].i = safe_lint_addition(a.elements.b4[a.n].i, b.elements.b4[a.n].i, &r.v_error);
            break;
            case LUINT:
            r.elements.b8[a.n].ui = safe_luint_addition(a.elements.b8[a.n].ui, b.elements.b8[a.n].ui, &r.v_error);
            break;            
            case FLOAT: 
                r.elements.b4[a.n].f = safe_float_addition((fbits){ .f = a.elements.b4[a.n].f }, (fbits){ .f = b.elements.b4[a.n].f }, &r.v_error);
            break;
            case DOUBLE:
                r.elements.b8[a.n].d = safe_double_addition((dbits){ .d = a.elements.b8[a.n].d }, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error);
            break;
            default: 
                r.elements.b1[a.n].i = a.elements.b1[a.n].i; 
            break;
        }
    }

    return r;
}




vecN vector_multiplication(vecN a, vecN b){
    if(a.type != b.type){ return a; }
    vecN r = {a.type, a.n, malloc(amount_of_type_bytes(a.type) * a.n), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case CHAR: 
                r.elements.b1[a.n].i = safe_char_multiplication(a.elements.b1[a.n].i, b.elements.b1[a.n].i, &r.v_error);
            break;
            case UCHAR: 
                r.elements.b1[a.n].ui = safe_uchar_multiplication(a.elements.b1[a.n].ui, b.elements.b1[a.n].ui, &r.v_error); 
            break;
            case INT: 
                r.elements.b4[a.n].i = safe_int_multiplication(a.elements.b4[a.n].i, b.elements.b4[a.n].i, &r.v_error); 
            break;
            case UINT:
                r.elements.b4[a.n].ui = safe_uint_multiplication(a.elements.b4[a.n].ui, b.elements.b4[a.n].ui, &r.v_error); 
            break;
            case LINT: 
            r.elements.b8[a.n].i = safe_lint_multiplication(a.elements.b8[a.n].i, b.elements.b8[a.n].i, &r.v_error);
            break;
            case LUINT: 
            r.elements.b8[a.n].ui = safe_luint_multipication(a.elements.b8[a.n].ui, b.elements.b8[a.n].ui, &r.v_error);
            break;            
            case FLOAT: 
                r.elements.b4[a.n].f = safe_float_multiplication_rounding((fbits){ .f = a.elements.b4[a.n].f }, (fbits){ .f = b.elements.b4[a.n].f }, &r.v_error);
            break;
            case DOUBLE:
                r.elements.b8[a.n].d = safe_double_mutltiplication_with_rounding((dbits){ .d = a.elements.b8[a.n].d }, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error);
            break;
            default: 
                r.elements.b1[a.n].i = a.elements.b1[a.n].i; 
            break; 
        }

    }
    return r;
}
