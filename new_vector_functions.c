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
    if(a.type != b.type){ return a; }
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n--){
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




vecN vector_multiplication(vecN a, vecN b){
    if(a.type != b.type){ return a; }
    vecN r = {a.type, a.n, malloc(amount_of_type_bytes(a.type) * a.n), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case CHAR: 
                r.elements.c[a.n] = safe_char_multiplication(a.elements.c[a.n], b.elements.c[a.n], &r.v_error);
            break;
            case UCHAR: 
                r.elements.uc[a.n] = safe_unsigned_char_multiplication(a.elements.uc[a.n], b.elements.uc[a.n], &r.v_error); 
            break;
            case INT: 
                r.elements.i[a.n] = safe_int_multiplication(a.elements.i[a.n], b.elements.i[a.n], &r.v_error); 
            break;
            case UINT:
                r.elements.ui[a.n] = safe_uint_multiplication(a.elements.ui[a.n], b.elements.ui[a.n], &r.v_error); 
            break;
            case LINT: 
            r.elements.li[a.n] = safe_lint_multiplication(a.elements.li[a.n], b.elements.li[a.n], &r.v_error);
            break;
            case LUINT: 
            r.elements.lui[a.n] = safe_luint_multipication(a.elements.lui[a.n], b.elements.lui[a.n], &r.v_error);
            break;            
            case FLOAT: 
                r.elements.f[a.n] = safe_float_multiplication_rounding((fbits){ .f = a.elements.f[a.n] }, (fbits){ .f = b.elements.f[a.n] }, &r.v_error);
            break;
            case DOUBLE:
                r.elements.d[a.n] = safe_double_mutltiplication_with_rounding((dbits){ .d = a.elements.d[a.n] }, (dbits){ .d = b.elements.d[a.n]}, &r.v_error);
            break;
            default: 
                r.elements.i[a.n] = a.elements.i[a.n]; 
            break; 
        }

    }
}

vecN vectors_multiplication_via_dot_product(vecN v1, vecN v2){
    if(v1.type != v2.type){ return v1; }
    vecN var = vector_multiplication(v1, v2);
    // free(var.elements);
    // var.elements = malloc(...);
    // long unsigned int var1 = 0;
    // long unsigned int *ptr = &var1;
    vecN result;
    result.type = v1.type;
    while(v1.n--){
      switch(var.type){
        case CHAR:  { result.elements.c[0]  = safe_char_addition(result.elements.c[0] , result.elements.c[v1.n] , &result.v_error); } break;
        case UCHAR: { result.elements.uc[0] = safe_char_addition(result.elements.uc[0] , result.elements.uc[v1.n] , &result.v_error); } break;
        case INT:   { result.elements.i[0]  = safe_int_addition( result.elements.i[0] , result.elements.i[v1.n] , &result.v_error); } break;
        case UINT:  { result.elements.ui[0] = safe_uint_addition(result.elements.ui[0], result.elements.ui[v1.n], &result.v_error); } break;
        case LINT:  { result.elements.li[0] = safe_uint_addition(result.elements.li[0], result.elements.li[v1.n], &result.v_error); } break;
        case LUINT: { result.elements.lui[0]= safe_uint_addition(result.elements.lui[0], result.elements.lui[v1.n], &result.v_error); } break;
        case FLOAT: { result.elements.f[0]  = safe_float_addition( (fbits){ .f = result.elements.f[0] } , (fbits){ .f = result.elements.f[v1.n] } , &result.v_error); } break;
        case DOUBLE:{ result.elements.d[0]  = safe_double_addition((dbits){ .d = result.elements.d[0] }, (dbits){ .d = result.elements.d[v1.n] }, &result.v_error); } break;
      }
    }
   return result; 
}

vecN vector_exponentiation(vecN a, vecN b){
    if(a.type != b.type){ return a; }
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case CHAR: 
                r.elements.c[a.n] = exp_char2char(a.elements.c[a.n], b.elements.c[a.n], &r.v_error); 
            break;
            case UCHAR:
                r.elements.uc[a.n] = exp_uchar2uchar(a.elements.uc[a.n], b.elements.uc[a.n], &r.v_error);
            break;
            case INT:   
                r.elements.i[a.n] = exp_int2int(a.elements.i[a.n], b.elements.i[a.n], &r.v_error);
            break;
            case UINT: 
                r.elements.ui[a.n] = exp_uint2uint(a.elements.ui[a.n], b.elements.ui[a.n], &r.v_error);
            break;
            case LINT:  
                r.elements.li[a.n] = exp_lint2lint(a.elements.li[a.n], b.elements.li[a.n], &r.v_error);
            break;
            case LUINT: 
                r.elements.lui[a.n] = exp_luint2luint(a.elements.lui[a.n], b.elements.lui[a.n], &r.v_error);
            break;
            case FLOAT: 
                r.elements.f[a.n] = exp_float2float((fbits){ .f = a.elements.f[a.n] }, (fbits){ .f = b.elements.f[a.n] }, &r.v_error);
            break;
            case DOUBLE:
                r.elements.d[a.n] = exp_double2double((dbits){ .d = a.elements.d[a.n] }, (dbits){ .d = b.elements.d[a.n] }, &r.v_error);
            break; 
        }

    }
    return r;
}

vecN vector_exponentiation_dif_types(vecN a, vecN b){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type))};

    return r;
}