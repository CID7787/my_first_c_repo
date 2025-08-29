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
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case CHAR: { 
                switch(b.type){
                    case CHAR:   r.elements.c[a.n] = exp_char2char(a.elements.c[a.n], b.elements.c[a.n], &r.v_error); break;
                    case UCHAR:  r.elements.c[a.n] = exp_char2uchar(a.elements.c[a.n], b.elements.uc[a.n], &r.v_error); break;// TODO
                    case INT:    r.elements.c[a.n] = exp_char2int(a.elements.c[a.n], b.elements.i[a.n], &r.v_error); break;// TODO
                    case UINT:   r.elements.c[a.n] = exp_char2uint(a.elements.c[a.n], b.elements.ui[a.n], &r.v_error); break;// TODO
                    case LINT:   r.elements.c[a.n] = exp_char2lint(a.elements.c[a.n], b.elements.li[a.n], &r.v_error); break;// TODO
                    case LUINT:  r.elements.c[a.n] = exp_char2luint(a.elements.c[a.n], b.elements.lui[a.n], &r.v_error); break;// TODO;
                    case FLOAT:  r.elements.c[a.n] = exp_char2float(a.elements.c[a.n], b.elements.f[a.n], &r.v_error); break;// TODO
                    case DOUBLE: r.elements.c[a.n] = exp_char2double(a.elements.c[a.n], b.elements.d[a.n], &r.v_error); break;// TODO
                }
            break; }
            case UCHAR:
                switch(b.type){
                    case CHAR:   r.elements.uc[a.n] = exp_uchar2char(a.elements.uc[a.n], b.elements.c[a.n], &r.v_error); break; // TODO
                    case UCHAR:  r.elements.uc[a.n] = exp_uchar2uchar(a.elements.uc[a.n], b.elements.uc[a.n], &r.v_error); break;
                    case INT:    r.elements.uc[a.n] = exp_uchar2int(a.elements.uc[a.n], b.elements.i[a.n], &r.v_error); break;// TODO
                    case UINT:   r.elements.uc[a.n] = exp_uchar2uint(a.elements.uc[a.n], b.elements.ui[a.n], &r.v_error); break;// TODO
                    case LINT:   r.elements.uc[a.n] = exp_uchar2lint(a.elements.uc[a.n], b.elements.li[a.n], &r.v_error); break;// TODO
                    case LUINT:  r.elements.uc[a.n] = exp_uchar2luint(a.elements.uc[a.n], b.elements.lui[a.n], &r.v_error); break;// TODO
                    case FLOAT:  r.elements.uc[a.n] = exp_uchar2float(a.elements.uc[a.n], b.elements.f[a.n], &r.v_error); break;// TODO
                    case DOUBLE: r.elements.uc[a.n] = exp_uchar2double(a.elements.uc[a.n], b.elements.d[a.n], &r.v_error); break;// TODO
                }

            break;
            case INT:   
                switch(b.type){
                    case CHAR:   r.elements.i[a.n] = exp_int2char(a.elements.i[a.n], b.elements.c[a.n], &r.v_error); break; // TODO
                    case UCHAR:  r.elements.i[a.n] = exp_int2uchar(a.elements.i[a.n], b.elements.uc[a.n], &r.v_error); break; // TODO
                    case INT:    r.elements.i[a.n] = exp_int2int(a.elements.i[a.n], b.elements.i[a.n], &r.v_error); break; 
                    case UINT:   r.elements.i[a.n] = exp_int2uint(a.elements.i[a.n], b.elements.ui[a.n], &r.v_error); break; // TODO
                    case LINT:   r.elements.i[a.n] = exp_int2lint(a.elements.i[a.n], b.elements.li[a.n], &r.v_error); break; // TODO
                    case LUINT:  r.elements.i[a.n] = exp_int2luint(a.elements.i[a.n], b.elements.lui[a.n], &r.v_error); break; // TODO
                    case FLOAT:  r.elements.i[a.n] = exp_int2float(a.elements.i[a.n], b.elements.f[a.n], &r.v_error); break; // TODO
                    case DOUBLE: r.elements.i[a.n] = exp_int2double(a.elements.i[a.n], b.elements.d[a.n], &r.v_error); break; // TODO
                }
            break;
            case UINT: 
                switch(b.type){ 
                    case CHAR:   r.elements.ui[a.n] = exp_uint2char(a.elements.ui[a.n], b.elements.c[a.n], &r.v_error); break;// TODO
                    case UCHAR:  r.elements.ui[a.n] = exp_uint2uchar(a.elements.ui[a.n], b.elements.uc[a.n], &r.v_error); break;// TODO
                    case INT:    r.elements.ui[a.n] = exp_uint2int(a.elements.ui[a.n], b.elements.i[a.n], &r.v_error); break;// TODO
                    case UINT:   r.elements.ui[a.n] = exp_uint2uint(a.elements.ui[a.n], b.elements.ui[a.n], &r.v_error); break;
                    case LINT:   r.elements.ui[a.n] = exp_uint2lint(a.elements.ui[a.n], b.elements.li[a.n], &r.v_error); break;// TODO
                    case LUINT:  r.elements.ui[a.n] = exp_uint2luint(a.elements.ui[a.n], b.elements.lui[a.n], &r.v_error); break;// TODO
                    case FLOAT:  r.elements.ui[a.n] = exp_uint2float(a.elements.ui[a.n], b.elements.f[a.n], &r.v_error); break;// TODO
                    case DOUBLE: r.elements.ui[a.n] = exp_uint2double(a.elements.ui[a.n], b.elements.d[a.n], &r.v_error); break;// TODO
                }
            break;
            case LINT: 
                switch(b.type){
                    case CHAR:   r.elements.li[a.n] = exp_lint2char(a.elements.li[a.n], b.elements.c[a.n], &r.v_error); break;// TODO
                    case UCHAR:  r.elements.li[a.n] = exp_lint2uchar(a.elements.li[a.n], b.elements.uc[a.n], &r.v_error); break;// TODO
                    case INT:    r.elements.li[a.n] = exp_lint2int(a.elements.li[a.n], b.elements.i[a.n], &r.v_error); break;// TODO
                    case UINT:   r.elements.li[a.n] = exp_lint2uint(a.elements.li[a.n], b.elements.ui[a.n], &r.v_error); break;// TODO
                    case LINT:   r.elements.li[a.n] = exp_lint2lint(a.elements.li[a.n], b.elements.li[a.n], &r.v_error); break;
                    case LUINT:  r.elements.li[a.n] = exp_lint2luint(a.elements.li[a.n], b.elements.lui[a.n], &r.v_error); break;// TODO
                    case FLOAT:  r.elements.li[a.n] = exp_lint2float(a.elements.li[a.n], b.elements.f[a.n], &r.v_error); break;// TODO
                    case DOUBLE: r.elements.li[a.n] = exp_lint2double(a.elements.li[a.n], b.elements.d[a.n], &r.v_error); break;// TODO
                }
            break;
            case LUINT: 
                switch(b.type){
                    case CHAR:   r.elements.lui[a.n] = exp_luint2char(a.elements.lui[a.n], b.elements.c[a.n], &r.v_error); break;// TODO
                    case UCHAR:  r.elements.lui[a.n] = exp_luint2uchar(a.elements.lui[a.n], b.elements.uc[a.n], &r.v_error); break;// TODO
                    case INT:    r.elements.lui[a.n] = exp_luint2int(a.elements.lui[a.n], b.elements.i[a.n], &r.v_error); break;// TODO
                    case UINT:   r.elements.lui[a.n] = exp_luint2uint(a.elements.lui[a.n], b.elements.ui[a.n], &r.v_error); break;// TODO
                    case LINT:   r.elements.lui[a.n] = exp_luint2lint(a.elements.lui[a.n], b.elements.li[a.n], &r.v_error); break;// TODO
                    case LUINT:  r.elements.lui[a.n] = exp_luint2luint(a.elements.lui[a.n], b.elements.lui[a.n], &r.v_error); break;
                    case FLOAT:  r.elements.lui[a.n] = exp_luint2float(a.elements.lui[a.n], b.elements.f[a.n], &r.v_error); break;// TODO
                    case DOUBLE: r.elements.lui[a.n] = exp_luint2double(a.elements.lui[a.n], b.elements.d[a.n], &r.v_error); break;// TODO
                }
            break;
            case FLOAT: 
                switch(b.type){
                    case CHAR:   r.elements.f[a.n] = exp_float2char((fbits){ .f = a.elements.f[a.n] }, b.elements.c[a.n], &r.v_error); break;// TOODO
                    case UCHAR:  r.elements.f[a.n] = exp_float2uchar((fbits){ .f = a.elements.f[a.n] }, b.elements.uc[a.n], &r.v_error); break;// TOODO
                    case INT:    r.elements.f[a.n] = exp_float2int((fbits){ .f = a.elements.f[a.n] }, b.elements.i[a.n], &r.v_error); break;// TOODO
                    case UINT:   r.elements.f[a.n] = exp_float2uint((fbits){ .f = a.elements.f[a.n] }, b.elements.ui[a.n], &r.v_error); break;// TOODO
                    case LINT:   r.elements.f[a.n] = exp_float2lint((fbits){ .f = a.elements.f[a.n] }, b.elements.li[a.n], &r.v_error); break;// TOODO
                    case LUINT:  r.elements.f[a.n] = exp_float2luint((fbits){ .f = a.elements.f[a.n] }, b.elements.lui[a.n], &r.v_error); break;// TOODO
                    case FLOAT:  r.elements.f[a.n] = exp_float2float((fbits){ .f = a.elements.f[a.n] }, (fbits){ .f = b.elements.f[a.n] }, &r.v_error); break;
                    case DOUBLE: r.elements.f[a.n] = exp_float2double((fbits){ .f = a.elements.f[a.n] }, b.elements.d[a.n], &r.v_error); break;// TOODO
                }
            break;
            case DOUBLE:
                switch(b.type){
                    case CHAR:   r.elements.d[a.n] = exp_double2char((dbits){ .d = a.elements.d[a.n] }, b.elements.c[a.n], &r.v_error); break;// TODO
                    case UCHAR:  r.elements.d[a.n] = exp_double2uchar((dbits){ .d = a.elements.d[a.n] }, b.elements.uc[a.n], &r.v_error); break;// TODO
                    case INT:    r.elements.d[a.n] = exp_double2int((dbits){ .d = a.elements.d[a.n] }, b.elements.i[a.n], &r.v_error); break;// TODO
                    case UINT:   r.elements.d[a.n] = exp_double2uint((dbits){ .d = a.elements.d[a.n] }, b.elements.ui[a.n], &r.v_error); break;// TODO
                    case LINT:   r.elements.d[a.n] = exp_double2lint((dbits){ .d = a.elements.d[a.n] }, b.elements.li[a.n], &r.v_error); break;// TODO
                    case LUINT:  r.elements.d[a.n] = exp_double2luint((dbits){ .d = a.elements.d[a.n] }, b.elements.lui[a.n], &r.v_error); break;// TODO
                    case FLOAT:  r.elements.d[a.n] = exp_double2float((dbits){ .d = a.elements.d[a.n] }, b.elements.f[a.n], &r.v_error); break;// TODO
                    case DOUBLE: r.elements.d[a.n] = exp_double2double((dbits){ .d = a.elements.d[a.n] }, (dbits){ .d = b.elements.d[a.n] }, &r.v_error); break;
                }
            break; 
        }

    }
    return r;
}