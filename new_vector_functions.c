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
    int cond;
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n-- ){
        switch(r.type){
            case CHAR: 
                cond = a.elements.b1.i[a.n] == MIN_CHAR;
                r.v_error = (-cond & POSITIVE_OVERFLOW) | (-cond & r.v_error); 
                r.elements.b1.i[a.n] = -a.elements.b1.i[a.n];
            break;
            case INT: 
            cond = a.elements.b1.i[a.n] == MIN_INT;
            r.v_error = (-cond & POSITIVE_OVERFLOW) | (-cond & r.v_error);
            r.elements.b4.i[a.n] =  -a.elements.b4.i[a.n]; 
            break;
            case FLOAT: 
            r.elements.b4.f[a.n] = -a.elements.b4.f[a.n]; 
            break;
            case LINT: 
            cond = a.elements.b1.i[a.n] == MIN_LINT;
            r.v_error = (-cond & POSITIVE_OVERFLOW) | (-cond & r.v_error);
            r.elements.b8.i[a.n] = -a.elements.b8.i[a.n]; 
            break;
            case DOUBLE: 
            r.elements.b8.d[a.n] = -a.elements.b8.d[a.n]; 
            break;
            default: return r;
        }
    }
    return r;
}

vecN vector_addition(vecN a, vecN b){
    if(a.type != b.type){ return a; }// TODO: some types should be able to add up
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case CHAR: 
                r.elements.b1.i[a.n] = safe_char_addition(a.elements.b1.i[a.n], b.elements.b1.i[a.n], &r.v_error);
            break;
            case UCHAR:
                r.elements.b1.ui[a.n] = safe_uchar_addition(a.elements.b1.ui[a.n], b.elements.b1.ui[a.n], &r.v_error); 
            break;
            case INT: 
                r.elements.b4.i[a.n] = safe_int_addition(a.elements.b4.i[a.n], b.elements.b4.i[a.n], &r.v_error); 
            break;
            case UINT:
                r.elements.b4.ui[a.n] = safe_uint_addition(a.elements.b4.ui[a.n], b.elements.b4.ui[a.n], &r.v_error); 
            break;
            case LINT: 
            r.elements.b8.i[a.n] = safe_lint_addition(a.elements.b8.i[a.n], b.elements.b8.i[a.n], &r.v_error);
            break;
            case LUINT:
            r.elements.b8.ui[a.n] = safe_luint_addition(a.elements.b8.ui[a.n], b.elements.b8.ui[a.n], &r.v_error);
            break;            
            case FLOAT: 
                r.elements.b4.f[a.n] = safe_float_addition((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b4.f[a.n] }, &r.v_error);
            break;
            case DOUBLE:
                r.elements.b8.d[a.n] = safe_double_addition((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b8.d[a.n]}, &r.v_error);
            break;
            default: return r;
        }
    }
    return r;
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
