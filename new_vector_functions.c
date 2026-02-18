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

#define B1type_i_elements(...) (alldatapointer){ .b1 = (B1type){ .i = __VA_ARGS__}}
#define B1type_ui_elements(...) (alldatapointer){ .b1 = (B1type){ .ui = __VA_ARGS__}}
#define B4type_i_elements(...) (alldatapointer){ .b4 = (B4type){ .i = __VA_ARGS__}} 
#define B4type_ui_elements(...) (alldatapointer){ .b4 = (B4type){ .ui = __VA_ARGS__}}
#define B4type_f_elements(...) (alldatapointer){ .b4 = (B4type){ .f = __VA_ARGS__}}
#define B8type_i_elements(...) (alldatapointer){ .b8 = (B8type){ .i = __VA_ARGS__}} 
#define B8type_ui_elements(...) (alldatapointer){ .b8 = (B8type){ .ui = __VA_ARGS__}} 
#define B8type_d_elements(...) (alldatapointer){ .b8 = (B8type){ .d = __VA_ARGS__}} 

unsigned char amount_of_type_bytes(datatype t){
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

unsigned char is_integral(datatype type){
    return !((type == FLOAT) | (type == DOUBLE));
}

unsigned char is_unsigned(datatype type){
    return (type == UCHAR) | (type == UINT) | (type == LUINT);
}

vecN vector_creation(datatype type, unsigned int n, alldatapointer elements){
    unsigned int r_element_size = amount_of_type_bytes(type);
    vecN r = {type, n, malloc(n * r_element_size), NO_ERROR};
    int* ptr = (int*)elements.b1.i;
    while(n--){
        switch(type | -(!ptr)){
            case CHAR:  r.elements.b1.i[n]  = elements.b1.i[n]; break;
            case UCHAR: r.elements.b1.ui[n] = elements.b1.ui[n]; break;
            case INT:   r.elements.b4.i[n]  = elements.b4.i[n]; break;
            case UINT:  r.elements.b4.ui[n] = elements.b4.ui[n]; break;
            case FLOAT: r.elements.b4.f[n]  = elements.b4.f[n]; break;
            case LINT:  r.elements.b8.i[n]  = elements.b8.i[n]; break;
            case LUINT: r.elements.b8.ui[n] = elements.b8.ui[n]; break;
            case DOUBLE:r.elements.b8.d[n]  = elements.b8.d[n]; break;
            case -1:    
                switch(r_element_size){
                    case 1:   r.elements.b1.i[n] = 0; break;
                    case 4:   r.elements.b4.i[n] = 0; break;
                    case 8:   r.elements.b8.i[n] = 0; break;
                }
            break; 
        }    
    }
    return r;
}   

vecN vector_negation(vecN a){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n-- ){
        switch(r.type){
            case CHAR: 
                r.v_error = ternary(a.elements.b1.i[a.n] == MIN_CHAR, POSITIVE_OVERFLOW, r.v_error);  
                r.elements.b1.i[a.n] = -a.elements.b1.i[a.n];
            break;
            case UCHAR: r.elements.b1.ui[a.n] = a.elements.b1.ui[a.n];
            break;
            case INT: 
                r.v_error = ternary(a.elements.b4.i[a.n] == MIN_INT, POSITIVE_OVERFLOW, r.v_error);
                r.elements.b4.i[a.n] =  -a.elements.b4.i[a.n]; 
                break;
            case UINT: r.elements.b4.ui[a.n] = a.elements.b4.ui[a.n];
            break;
            case FLOAT: 
                r.elements.b4.f[a.n] = -a.elements.b4.f[a.n]; 
                break;
            case LINT: 
                r.v_error = ternary(a.elements.b8.i[a.n] == MIN_LINT, POSITIVE_OVERFLOW, r.v_error);
                r.elements.b8.i[a.n] = -a.elements.b8.i[a.n]; 
                break;
            case LUINT: r.elements.b8.ui[a.n] = a.elements.b8.ui[a.n];
            break;
            case DOUBLE: 
                r.elements.b8.d[a.n] = -a.elements.b8.d[a.n]; 
                break;
            default: 
                return r;
        }
    }
    return r;
}

vecNv2 vector_addition(vecNv2 a, vecNv2 b){// adjustments: a, b different types; r optional type
    unsigned int i, r_n = ternary(a.n > b.n, a.n, b.n);
    vecNv2 r = {a.type, a.n, malloc(r.n * amount_of_type_bytes(a.type)), NO_ERROR};
    for(i = 0; i < r_n; i++){
        switch(a.type){
            case CHAR: 
                if((i < a.n) & i < (b.n)){ r.elements.b1.integ.i[i] = safe_char_addition(a.elements.b1.integ.i[i], b.elements.b1.integ.i[i], &r.v_error); }
                else{
                    if(i < a.n){ r.elements.b1.integ.i[i] = a.elements.b1.integ.i[i]; }
                    else{ r.elements.b1.integ.i[i] = b.elements.b1.integ.i[i]; }
                }
            break;
            case UCHAR:
                if((i < a.n) & i < (b.n)){ r.elements.b1.integ.ui[i] = safe_uchar_addition(a.elements.b1.integ.ui[i], b.elements.b1.integ.ui[i], &r.v_error); }
                else{
                    if(i < a.n){ r.elements.b1.integ.ui[i] = a.elements.b1.integ.ui[i]; }
                    else{ r.elements.b1.integ.ui[i] = b.elements.b1.integ.ui[i]; }
                }
            break;
            case INT:
                if((i < a.n) & i < (b.n)){ r.elements.b4.integ.i[i] = safe_int_addition(a.elements.b4.integ.i[i], b.elements.b4.integ.i[i], &r.v_error); }
                else{
                    if(i < a.n){ r.elements.b4.integ.i[i] = a.elements.b4.integ.i[i]; }
                    else{ r.elements.b4.integ.i[i] = b.elements.b4.integ.i[i]; }
                }
            break;
            case UINT:
                if((i < a.n) & i < (b.n)){ r.elements.b4.integ.ui[i] = safe_uint_addition(a.elements.b4.integ.ui[i], b.elements.b4.integ.ui[i], &r.v_error); }
                else{
                    if(i < a.n){ r.elements.b4.integ.ui[i] = a.elements.b4.integ.ui[i]; }
                    else{ r.elements.b4.integ.ui[i] = b.elements.b4.integ.ui[i]; }
                }
            break;
            case LINT:
                if((i < a.n) & i < (b.n)){ r.elements.b8.integ.i[i] = safe_lint_addition(a.elements.b8.integ.i[i], b.elements.b8.integ.i[i], &r.v_error); }
                    else{
                        if(i < a.n){ r.elements.b8.integ.i[i] = a.elements.b8.integ.i[i]; }
                        else{ r.elements.b8.integ.i[i] = b.elements.b8.integ.i[i]; }
                } 
            break;
            case LUINT:
                if((i < a.n) & i < (b.n)){ r.elements.b8.integ.ui[i] = safe_luint_addition(a.elements.b8.integ.ui[i], b.elements.b8.integ.ui[i], &r.v_error); }
                else{
                    if(i < a.n){ r.elements.b8.integ.ui[i] = a.elements.b8.integ.ui[i]; }
                    else{ r.elements.b8.integ.ui[i] = b.elements.b8.integ.ui[i]; }
                }            break;            
            case FLOAT: 
                if((i < a.n) & i < (b.n)){ r.elements.b4.flt[i] = safe_float_addition((fbits){ .f = a.elements.b4.flt[i] }, (fbits){ .f = b.elements.b4.flt[i] }, &r.v_error); }
                else{
                    if(i < a.n){ r.elements.b4.flt[i] = a.elements.b4.flt[i]; }
                    else{ r.elements.b4.flt[i] = b.elements.b4.flt[i]; }
                }            break;
            case DOUBLE:
                if((i < a.n) & i < (b.n)){ r.elements.b8.flt[i] = safe_double_addition((dbits){ .d = a.elements.b8.flt[i] }, (dbits){ .d = b.elements.b8.flt[i]}, &r.v_error); }
                    else{
                        if(i < a.n){ r.elements.b8.flt[i] = a.elements.b8.flt[i]; }
                        else{ r.elements.b8.flt[i] = b.elements.b8.flt[i]; }
                }
            break;
            default: return r;
        }
    }
    return r;
}


vecN vector_addition_v2(vecNv2 a, vecNv2 b, datatype r_type){
    char chv;
    unsigned char uchv;
    int iv;
    unsigned int uiv, r_n = ternary(a.n > b.n, a.n, b.n), i = 0, r_type_s = amount_of_type_bytes(r_type), a_type_s = amount_of_type_bytes(a.type), b_type_s = amount_of_type_bytes(b.type);
    long unsigned int luiv;
    long int liv;
    double dv;
    vecN r = {r_type, r_n, malloc(r_type_s * r_n), NO_ERROR};
    while(i++ < r_n){
        switch(a_type_s & -(i < a.n)){
            case 1:
                switch(b_type_s & -(i < b.n)){
                    case 1: 
                        if(is_unsigned(a.type)){
                            if(is_unsigned(b.type)){
                            }
                            else{
                                // iv = safe_int_addition(a.elements.b1.integ.ui);

                            }
                        }
                        else{
                            if(is_unsigned(b.type)){

                            }
                            else{
                                
                            }
                        }
                    break;
                    case 4:
                    break;
                    case 8:
                    break;
                    default: 
                }
            break;
            case 4:
                switch(b_type_s & -(i < b.n)){
                    case 1:
                    break;
                    case 4:
                    break;
                    case 8:
                    break;
                    default:
                }
            break;
            case 8:
                switch(b_type_s & -(i < b.n)){
                    case 1:
                    break;
                    case 4:
                    break;
                    case 8:
                    break;
                    default:
                }
            break;
            default:

        }
        switch(r_type_s){
            case 1:
            break;
            case 4:
            break;
            case 8:

        }
    }
}



vecN vector_multiplication(vecN a, vecN b){
    if(a.type != b.type){ return a; }// TODO: some types should be able to multiply with each other
    vecN r = {a.type, a.n, malloc(amount_of_type_bytes(a.type) * a.n), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case CHAR: 
                r.elements.b1.i[a.n] = safe_char_multiplication(a.elements.b1.i[a.n], b.elements.b1.i[a.n], &r.v_error);
            break;
            case UCHAR: 
                r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b1.ui[a.n], &r.v_error); 
            break;
            case INT: 
                r.elements.b4.i[a.n] = safe_int_multiplication(a.elements.b4.i[a.n], b.elements.b4.i[a.n], &r.v_error); 
            break;
            case UINT:
                r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b4.ui[a.n], &r.v_error); 
            break;
            case LINT: 
            r.elements.b8.i[a.n] = safe_lint_multiplication(a.elements.b8.i[a.n], b.elements.b8.i[a.n], &r.v_error);
            break;
            case LUINT: 
            r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b8.ui[a.n], &r.v_error);
            break;            
            case FLOAT: 
                r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b4.f[a.n] }, &r.v_error);
            break;
            case DOUBLE:    
                r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error);
            break;
            default: 
                r.elements.b1.i[a.n] = a.elements.b1.i[a.n]; 
            break; 
        }

    }
    return r;
}
 
vecN vector_exponentiation(vecN a, vecN b){
    unsigned char a_elem_size = amount_of_type_bytes(a.type), b_elem_size = amount_of_type_bytes(b.type), r_elem_size = -(a_elem_size > b_elem_size);
    r_elem_size = (r_elem_size & a_elem_size) | (~r_elem_size & b_elem_size);
    vecN r = {a.type, a.n, malloc(a.n * r_elem_size), NO_ERROR};
    long int a_lint, b_lint;
    long unsigned int a_luint, b_luint; 
    while(a.n--){
        switch(a.type){
            case DOUBLE:
                switch(b.type){
                    case DOUBLE: r.elements.b8.d[a.n] = exp_double2double((dbits){ .d = a.elements.b8.d[a.n]}, (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); break;// DONE
                    case FLOAT:  r.elements.b8.d[a.n] = exp_double2double((dbits){ .d = a.elements.b8.d[a.n]}, (dbits){ .d = b.elements.b4.f[a.n]}, &r.v_error); break;// DONE
                    case CHAR:   r.elements.b8.d[a.n] = exp_double2lint(  (dbits){ .d = a.elements.b8.d[a.n]},               b.elements.b1.i[a.n],  &r.v_error); break;// DONE
                    case UCHAR:  r.elements.b8.d[a.n] = exp_double2luint( (dbits){ .d = a.elements.b8.d[a.n]},               b.elements.b1.ui[a.n], &r.v_error); break;// DONE
                    case INT:    r.elements.b8.d[a.n] = exp_double2lint(  (dbits){ .d = a.elements.b8.d[a.n]},               b.elements.b4.i[a.n],  &r.v_error); break;// DONE
                    case UINT:   r.elements.b8.d[a.n] = exp_double2luint( (dbits){ .d = a.elements.b8.d[a.n]},               b.elements.b4.ui[a.n], &r.v_error); break;// DONE
                    case LINT:   r.elements.b8.d[a.n] = exp_double2lint(  (dbits){ .d = a.elements.b8.d[a.n]},               b.elements.b8.i[a.n],  &r.v_error); break;// DONE
                    case LUINT:  r.elements.b8.d[a.n] = exp_double2luint( (dbits){ .d = a.elements.b8.d[a.n]},               b.elements.b8.ui[a.n], &r.v_error); break;// DONE
                }
            case FLOAT:
                switch(b.type){
                    case DOUBLE: r.elements.b8.d[a.n] = exp_double2double((dbits){ .d = a.elements.b4.f[a.n]}, (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); r.type = DOUBLE; break;// DONE
                    case FLOAT:  r.elements.b4.f[a.n] = exp_float2float((fbits){ .f = a.elements.b4.f[a.n]},   (fbits){ .f = b.elements.b4.f[a.n]}, &r.v_error); break;// DONE
                    case CHAR:   r.elements.b4.f[a.n] = exp_float2lint( (fbits){ .f = a.elements.b4.f[a.n]},               b.elements.b1.i[a.n], &r.v_error);    break;// DONE
                    case UCHAR:  r.elements.b4.f[a.n] = exp_float2luint((fbits){ .f = a.elements.b4.f[a.n]},               b.elements.b1.ui[a.n], &r.v_error);   break;// DONE
                    case INT:    r.elements.b4.f[a.n] = exp_float2lint( (fbits){ .f = a.elements.b4.f[a.n]},               b.elements.b4.i[a.n], &r.v_error);    break;// DONE
                    case UINT:   r.elements.b4.f[a.n] = exp_float2luint((fbits){ .f = a.elements.b4.f[a.n]},               b.elements.b4.ui[a.n], &r.v_error);   break;// DONE
                    case LINT:   r.elements.b8.d[a.n] = exp_float2lint( (fbits){ .f = a.elements.b4.f[a.n]},               b.elements.b8.i[a.n], &r.v_error);    break;// DONE
                    case LUINT:  r.elements.b8.d[a.n] = exp_float2luint((fbits){ .f = a.elements.b4.f[a.n]},               b.elements.b8.ui[a.n], &r.v_error);   break;// DONE
                }
            default:
                switch(b.type){
                    case DOUBLE: 
                        switch(a.type){
                            case CHAR:  r.elements.b8.i[a.n]  = exp_lint2double( a.elements.b1.i[a.n],  (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); break; // DONE
                            case UCHAR: r.elements.b8.ui[a.n] = exp_luint2double(a.elements.b1.ui[a.n], (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); break; // DONE
                            case INT:   r.elements.b8.i[a.n]  = exp_lint2double( a.elements.b4.i[a.n],  (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); break; // DONE
                            case UINT:  r.elements.b8.ui[a.n] = exp_luint2double(a.elements.b4.ui[a.n], (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); break; // DONE
                            case LINT:  r.elements.b8.i[a.n]  = exp_lint2double( a.elements.b8.i[a.n],  (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); break; // DONE
                            case LUINT: r.elements.b8.ui[a.n] = exp_luint2double(a.elements.b8.ui[a.n], (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error); break; // DONE
                        }
                    break;
                    case FLOAT: 
                        switch(a.type){
                            case CHAR:  r.elements.b4.i[a.n]  = exp_lint2double( a.elements.b1.i[a.n],  (dbits){ .d = b.elements.b4.f[a.n]}, &r.v_error); break; // DONE
                            case UCHAR: r.elements.b4.ui[a.n] = exp_luint2double(a.elements.b1.ui[a.n], (dbits){ .d = b.elements.b4.f[a.n]}, &r.v_error); break; // DONE
                            case INT:   r.elements.b4.i[a.n]  = exp_lint2double( a.elements.b4.i[a.n],  (dbits){ .d = b.elements.b4.f[a.n]}, &r.v_error); break; // DONE
                            case UINT:  r.elements.b4.ui[a.n] = exp_luint2double(a.elements.b4.ui[a.n], (dbits){ .d = b.elements.b4.f[a.n]}, &r.v_error); break; // DONE
                            case LINT:  r.elements.b8.i[a.n]  = exp_lint2double( a.elements.b8.i[a.n],  (dbits){ .d = b.elements.b4.f[a.n]}, &r.v_error); break; // DONE
                            case LUINT: r.elements.b8.ui[a.n] = exp_luint2double(a.elements.b8.ui[a.n], (dbits){ .d = b.elements.b4.f[a.n]}, &r.v_error); break; // DONE
                        }
                    break;
                    default: {
                        switch(b.type){
                            case CHAR:  b_lint  = b.elements.b1.i[a.n];  break;
                            case INT:   b_lint  = b.elements.b4.i[a.n];  break;
                            case LINT:  b_lint  = b.elements.b8.i[a.n];  break;
                            case UCHAR: b_luint = b.elements.b1.ui[a.n]; break;
                            case UINT:  b_luint = b.elements.b4.ui[a.n]; break;
                            case LUINT: b_luint = b.elements.b8.ui[a.n]; break;
                        }
                        switch(a.type){
                            case CHAR:  a_lint  = a.elements.b1.i[a.n];  break;
                            case INT:   a_lint  = a.elements.b4.i[a.n];  break;
                            case LINT:  a_lint  = a.elements.b8.i[a.n];  break;
                            case UCHAR: a_luint = a.elements.b1.ui[a.n]; break;
                            case UINT:  a_luint = a.elements.b4.ui[a.n]; break;
                            case LUINT: a_luint = a.elements.b8.ui[a.n]; break;
                        }
                        switch(r_elem_size){
                            case 1:
                                switch(a.type){
                                    case CHAR:  r.v_error = ternary((b.type == CHAR) & (b_lint < 0), UNDERFLOW, r.v_error);
                                                r.elements.b1.i[a.n] = exp_lint2luint(a_lint, b_luint, &r.v_error);
                                    break; 
                                    case UCHAR: r.v_error = ternary((b.type == CHAR) & (b_lint < 0), UNDERFLOW, r.v_error);
                                                r.elements.b1.ui[a.n] = exp_uchar2uchar(a_luint, b_luint, &r.v_error);
                                    break;       
                                } 
                            break;  
                            case 4:
                                switch(a.type){
                                    case CHAR:
                                    case INT:   r.v_error = ternary(( (b.type == CHAR) | (b.type == INT) ) & (b_lint < 0), UNDERFLOW, r.v_error);
                                                r.elements.b4.i[a.n] = exp_lint2luint(a_lint, b_luint, &r.v_error);
                                    break;
                                    case UCHAR:
                                    case UINT: r.v_error = ternary(( (b.type == CHAR) | (b.type == INT) ) & (b_lint < 0), UNDERFLOW, r.v_error);
                                               r.elements.b4.ui[a.n] = exp_uint2uint(a_luint, b_luint, &r.v_error);
                                    break; 
                                }
                            break;
                            case 8:
                                switch(a.type){
                                    case CHAR:
                                    case INT:
                                    case LINT:  r.v_error = ternary(( (b.type == CHAR) | (b.type == INT) | (b.type == LINT) ) & (b_lint < 0), UNDERFLOW, r.v_error);
                                                r.elements.b8.i[a.n] = exp_lint2luint(a_lint, b_luint, &r.v_error);
                                    break;
                                    case UCHAR:
                                    case UINT:
                                    case LUINT: r.v_error = ternary(( (b.type == CHAR) | (b.type == INT) | (b.type == LINT) ) & (b_lint < 0), UNDERFLOW, r.v_error);
                                                r.elements.b8.ui[a.n] = exp_luint2luint(a_luint, b_luint, &r.v_error);
                                    break;
                                }
                            break;
                        }
                    }
                }
        }      
    }
    return r;
}

/*sample

error {
    const char* function_of_origin: ...;
    error_type errT: ...;
    const int* buffer: ...;
    unisgned int buffer_length: 10000;
}

struct error GLOBAL_ERROR_HANDLER = { ... };

f2(int x){
    if (x < 0) {
        GLOBAL_ERROR_HANDLER->function_of_origin = "f2";
        GLOBAL_ERROR_HANDLER.errT = VECTOR_ADDITION_ERROR:
        GLOBAL_ERROR_HANDLER->buffer[0] = vector_length
        for (int i = 1; i < GLOBAL_ERROR_HANDLER.buffer_length; i++){
            GLOBAL_ERROR_HANDLER->buffer = vector[i];   
        }
    }
    ... 
}

print_error(error e){
    switch(e.errT){
        case VECTOR_ADDITION_ERROR: 
            int vector_length = e->buffer[0];
        case VECTOR_ADDITION_ERROR: 
            int vector_length = e->buffer[0];
            printf("An error happened during vector addition inside function %s, and here is the relevant info. Size: %d. Elements: ", e.function_of_origin, vector_length);
            for (int i = 0; i < vector_length; i++){
                printf("%d", e->buffer[i+1]);
            }
            break;
        case MATRIX_MULTIPLICATION_ERROR:
            int matrix_columns = e->buffer[0];
            int matrix_rows = e->buffer[1];
            for (int i = 0; i < matrix_columns; i++){
                for (int j = 0; j < matrix_rows; j++){

                }
            }
    }
}


*/
