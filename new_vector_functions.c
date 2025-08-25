#ifndef headerfile
    #include <stdio.h>
    #include <stdlib.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
#endif

unsigned int amount_of_type_bytes(datatype t){
    switch(t) {  
      case CHAR: return sizeof(char); break;
      case UCHAR: return sizeof(unsigned char); break;
      case INT: return sizeof(int); break;
      case UINT: return sizeof(unsigned int); break;
      case FLOAT: return sizeof(float); break;
      case LINT: return sizeof(long int); break;
      case LUINT: return sizeof(long unsigned int); break;
      case DOUBLE: return sizeof(double); break;
      default: return sizeof(datatype);
    }
}

vecN vector_creation(datatype type, unsigned int n, datapointer elements){
    vecN r = {type, n, malloc(n * amount_of_type_bytes(type))};
    while(n--){
        switch(type){
            case CHAR:   r.elements.c[n]  = elements.c[n]; break;
            case UCHAR:  r.elements.uc[n] = elements.uc[n]; break;
            case INT:    r.elements.i[n]  = elements.i[n]; break;
            case UINT:   r.elements.ui[n] = elements.ui[n]; break;
            case FLOAT:  r.elements.f[n]  = elements.f[n]; break;
            case LINT:   r.elements.li[n] = elements.li[n]; break;
            case LUINT:  r.elements.lui[n]= elements.lui[n]; break;
            case DOUBLE: r.elements.d[n]  = elements.d[n]; break;
        }
    }
    return r;
}   

vecN vector_negation(vecN a){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), a.v_error};
    while(a.n--){
        switch(r.type){
            case CHAR: 
                r.v_error = ternary(a.elements.c[a.n] == MIN_CHAR, POSITIVE_OVERFLOW, r.v_error); 
                r.elements.c[a.n] = -a.elements.c[a.n]; 
            break;
            case UCHAR: 
                r.elements.uc[a.n] = a.elements.uc[a.n]; 
            break;
            case INT: 
                r.v_error = ternary(a.elements.i[a.n] == MIN_INT, POSITIVE_OVERFLOW, r.v_error); 
                r.elements.i[a.n] =  -a.elements.i[a.n]; 
            break;
            case UINT: 
                r.elements.ui[a.n] = a.elements.ui[a.n]; 
            break;
            case FLOAT: 
                r.elements.f[a.n] = -a.elements.f[a.n]; 
            break;
            case LINT: 
                r.v_error = ternary(a.elements.li[a.n] == MIN_LINT, POSITIVE_OVERFLOW, r.v_error); 
                r.elements.li[a.n] = -a.elements.li[a.n]; 
            break;
            case DOUBLE: 
                r.elements.d[a.n] = -a.elements.d[a.n]; 
            break;
        }
    }
    return r;
}

vecN vector_addition(vecN a, vecN b){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), a.v_error};
    unsigned int uint_float_representations = 0x7e7fffff;
	float MAX_FLOAT = *( (float*)(&uint_float_representations) );
    uint_float_representations |= 1u << 31;
    float MIN_FLOAT = *( (float*)(&uint_float_representations) );
    while(a.n && a.n--){
        switch(a.type){
            case CHAR: 
                r.elements.c[a.n] = safe_char_addition(a.elements.c[a.n], b.elements.c[a.n], &r.v_error);
            break;
            case UCHAR:
                r.elements.uc[a.n] = safe_unsigned_char_addition(a.elements.uc[a.n], b.elements.uc[a.n], &r.v_error); 
            break;
            case INT: 
                r.elements.i[a.n] = safe_int_addition(a.elements.i[a.n], b.elements.i[a.n], &r.v_error); 
            break;
            case UINT:
                r.elements.ui[a.n] = safe_uint_addition(a.elements.ui[a.n], b.elements.ui[a.n], &r.v_error); 
            break;
            case LINT: 
            r.elements.li[a.n] = safe_lint_addition(a.elements.li[a.n], b.elements.li[a.n], &r.v_error);
            break;
            case LUINT:
            r.elements.lui[a.n] = safe_luint_addition(a.elements.lui[a.n], b.elements.lui[a.n], &r.v_error);
            break;            
            case FLOAT: 
                r.elements.f[a.n] = safe_float_addition((fbits){ .f = a.elements.f[a.n] }, (fbits){ .f = b.elements.f[a.n] }, &r.v_error);
            break;
            case DOUBLE:
                r.elements.d[a.n] = safe_double_addition((dbits){ .d = a.elements.d[a.n] }, (dbits){ .d = b.elements.d[a.n]}, &r.v_error);
            break;
            default: 
                r.elements.i[a.n] = a.elements.i[a.n]; 
            break;
        }
    }

    return r;
}

