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
    alldatapointer elements;
    error v_error;
} typedef vecN;


double exp_double2double(dbits a, dbits b, error* err){
  if(!err){ return a.d; }
  dbits result = (dbits){ .d = 1};
  *err = ternary(have_frac_part(b), ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER, *err); 
  *err = ternary(!(a.d) && !(b.d), ZERO_TO_ZERO, *err);
  if(b.d < 0){ 
    while(b.d++ && !(*err)){  
      result.d = safe_double_division_with_rounding(result, a, err);
    }
  }
  else{
    while(b.d-- && !(*err)){  
      result.d = safe_double_multiplication_with_rounding(result, a, err);
    }
  }
  return result.d;
}


double exp_double2lint(dbits a, long int b, error* err){
    if(!err){ return a.d; }
    dbits r = (dbits){ .d = 1.0 };
    *err = ternary(!(a.d) && !b, ZERO_TO_ZERO, *err);
    if(b < 0){ 
        while(b++ && !(*err)){
            r.d = safe_double_division_with_rounding(r, a, err);
        }    
    }
    else{ 
        while(b-- && !(*err)){
            r.d = safe_double_multiplication_with_rounding(r, a, err);
        }
    }
    return r.d;
}

double exp_double2luint(dbits a, long unsigned int b, error* err){
    if(!err){ return a.d; }
    *err = ternary(!(a.d) && !b, ZERO_TO_ZERO, *err);
    dbits r = (dbits){ .d = 1.0 };
    while(b-- && !(*err)){
        r.d = safe_double_multiplication_with_rounding(r, a, err);
    }
    return r.d;
}


float exp_float2float(fbits a, fbits b, error* err){
  if(!err){ return a.f; }
  fbits result = (fbits){ .f = 1}; 
  *err = ternary(b.f - have_frac_part((dbits){.d = b.f}), ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER, *err); 
  *err = ternary(!(a.f) && !(b.f), ZERO_TO_ZERO, *err);
  if(b.f < 0){
    while(b.f++ && !(*err)){
      result.f = safe_float_division_with_rounding(result, a, err);
    }
  }
  else{
      while(b.f-- && !(*err)){
      result.f = safe_float_multiplication_with_rounding(result, a, err);
      }
  }
  return result.f;
}

float exp_float2lint(fbits a, long int b, error* err){
    if(!err){ return a.f; }
    *err = ternary(!(a.f) && !b, ZERO_TO_ZERO, *err);
    fbits result = (fbits){ .f = 1.0 };
    if(b < 0){ 
        while(b++ && !(*err)){
            a.f = safe_float_division_with_rounding(result, a, err);
        }
    }
    else{
        while(b-- && !(*err)){
            a.f = safe_float_multiplication_with_rounding(result, a, err);
        }
    }
    return result.f; 
}

float exp_float2luint(fbits a, long unsigned int b, error* err){
    if(!err){ return a.f; }
    *err = ternary(!(a.f) && !b, ZERO_TO_ZERO, *err);
    fbits result = (fbits){ .f = 1.0 };
    while(b-- && !(*err)){
        a.f = safe_float_multiplication_with_rounding(result, a, err);
    }
    return result.f; 
}

long int exp_lint2double(long int a, dbits b, error* err){
    if(!err){ return a; }
    long int r = 1;
    unsigned int cond = -have_frac_part(b);
    *err = (cond & ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER) | (cond & *err);
    cond = -(!a & !b.d);
    *err = (cond & ZERO_TO_ZERO) | (cond & *err);
    cond = -(b.d < 0);
    *err = (cond & UNDERFLOW) | (cond & *err);
    while(b.d-- && !(*err)){
        r = safe_lint_multiplication(r, a, err);
    } 
    return r;
}

long unsigned int exp_luint2double(long unsigned int a, dbits b, error* err){
    if(!err){ return a; }
    long unsigned int r = 1;
    unsigned int cond = -have_frac_part(b);
    *err = (cond & ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER) | (cond & *err);
    cond = -(!a & !b.d);
    *err = (cond & ZERO_TO_ZERO) | (cond & *err);
    cond = -(b.d < 0);
    *err = (cond & UNDERFLOW) | (cond & *err);
    while(b.d-- && !(*err)){
        r = safe_luint_multiplication(r, a, err);
    } 
    return r;
}

vecN vector_exponentiation(vecN a, vecN b){
    unsigned char a_elem_size = amount_of_type_bytes(a.type), b_elem_size = amount_of_type_bytes(b.type), r_elem_size = -(a_elem_size > b_elem_size);
    r_elem_size = (r_elem_size & a_elem_size) | (r_elem_size & b_elem_size);
    vecN r = {a.type, a.n, r_elem_size, malloc(a.n * r_elem_size), NO_ERROR};
    while(a.n--){
        switch(a.type){
            case DOUBLE:
                switch(b.type){
                    case DOUBLE: r.elements.b8[a.n].d = exp_double2double((dbits){ .d = a.elements.b8[a.n].d}, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break;// DONE
                    case FLOAT:  r.elements.b8[a.n].d = exp_double2double((dbits){ .d = a.elements.b8[a.n].d}, (dbits){ .d = b.elements.b4[a.n].f}, &r.v_error); break;// DONE
                    case CHAR:   r.elements.b8[a.n].d = exp_double2lint(  (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b1[a.n].i,  &r.v_error); break;// DONE
                    case UCHAR:  r.elements.b8[a.n].d = exp_double2luint( (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b1[a.n].ui, &r.v_error); break;// DONE
                    case INT:    r.elements.b8[a.n].d = exp_double2lint(  (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b4[a.n].i,  &r.v_error); break;// DONE
                    case UINT:   r.elements.b8[a.n].d = exp_double2luint( (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b4[a.n].ui, &r.v_error); break;// DONE
                    case LINT:   r.elements.b8[a.n].d = exp_double2lint(  (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b8[a.n].i,  &r.v_error); break;// DONE
                    case LUINT:  r.elements.b8[a.n].d = exp_double2luint( (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b8[a.n].ui, &r.v_error); break;// DONE
                }
            case FLOAT:
                switch(b.type){
                    case DOUBLE: r.elements.b8[a.n].d = exp_double2double((dbits){ .d = a.elements.b4[a.n].f}, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); r.type = DOUBLE; break;// DONE
                    case FLOAT:  r.elements.b4[a.n].f = exp_float2float((fbits){ .f = a.elements.b4[a.n].f},   (fbits){ .f = b.elements.b4[a.n].f}, &r.v_error); break;// DONE
                    case CHAR:   r.elements.b4[a.n].f = exp_float2lint( (fbits){ .f = a.elements.b4[a.n].f},               b.elements.b1[a.n].i, &r.v_error);    break;// DONE
                    case UCHAR:  r.elements.b4[a.n].f = exp_float2luint((fbits){ .f = a.elements.b4[a.n].f},               b.elements.b1[a.n].ui, &r.v_error);   break;// DONE
                    case INT:    r.elements.b4[a.n].f = exp_float2lint( (fbits){ .f = a.elements.b4[a.n].f},               b.elements.b4[a.n].i, &r.v_error);    break;// DONE
                    case UINT:   r.elements.b4[a.n].f = exp_float2luint((fbits){ .f = a.elements.b4[a.n].f},               b.elements.b4[a.n].ui, &r.v_error);   break;// DONE
                    case LINT:   r.elements.b8[a.n].d = exp_float2lint( (fbits){ .f = a.elements.b4[a.n].f},               b.elements.b8[a.n].i, &r.v_error);    break;// DONE
                    case LUINT:  r.elements.b8[a.n].d = exp_float2luint((fbits){ .f = a.elements.b4[a.n].f},               b.elements.b8[a.n].ui, &r.v_error);   break;// DONE
                }
            default:
                switch(b.type){
                    case DOUBLE: 
                        switch(a.type){
                            case CHAR:  r.elements.b8[a.n].i  = exp_lint2double( a.elements.b1[a.n].i,  (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break; // DONE
                            case UCHAR: r.elements.b8[a.n].ui = exp_luint2double(a.elements.b1[a.n].ui, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break; // DONE
                            case INT:   r.elements.b8[a.n].i  = exp_lint2double( a.elements.b4[a.n].i,  (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break; // DONE
                            case UINT:  r.elements.b8[a.n].ui = exp_luint2double(a.elements.b4[a.n].ui, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break; // DONE
                            case LINT:  r.elements.b8[a.n].i  = exp_lint2double( a.elements.b8[a.n].i,  (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break; // DONE
                            case LUINT: r.elements.b8[a.n].ui = exp_luint2double(a.elements.b8[a.n].ui, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break; // DONE
                        }
                    break;
                    case FLOAT: 
                    switch(a.type){
                        case CHAR:  r.elements.b4[a.n].i  = exp_lint2double( a.elements.b1[a.n].i,  (dbits){ .d = b.elements.b4[a.n].f}, &r.v_error); break; // DONE
                        case UCHAR: r.elements.b4[a.n].ui = exp_luint2double(a.elements.b1[a.n].ui, (dbits){ .d = b.elements.b4[a.n].f}, &r.v_error); break; // DONE
                        case INT:   r.elements.b4[a.n].i  = exp_lint2double( a.elements.b4[a.n].i,  (dbits){ .d = b.elements.b4[a.n].f}, &r.v_error); break; // DONE
                        case UINT:  r.elements.b4[a.n].ui = exp_luint2double(a.elements.b4[a.n].ui, (dbits){ .d = b.elements.b4[a.n].f}, &r.v_error); break; // DONE
                        case LINT:  r.elements.b8[a.n].i  = exp_lint2double( a.elements.b8[a.n].i,  (dbits){ .d = b.elements.b4[a.n].f}, &r.v_error); break; // DONE
                        case LUINT: r.elements.b8[a.n].ui = exp_luint2double(a.elements.b8[a.n].ui, (dbits){ .d = b.elements.b4[a.n].f}, &r.v_error); break; // DONE
                    }
                    break;
                    default:
                        // if (r.element_size == a.element_size) { b.type = a.type; }
                        // else{ a.type = b.type; }
                        // r.elements.b1[a.n].i  = pow(a.elements.b1[a.n].i , b.elements.b1[a.n].i); 
                        // r.elements.b1[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b1[a.n].ui);
                        // r.elements.b4[a.n].i  = pow(a.elements.b4[a.n].i , b.elements.b4[a.n].i);
                        // r.elements.b4[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b4[a.n].ui);
                        // r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i , b.elements.b8[a.n].i);
                        // r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b8[a.n].ui);
                    break; 
                }
        }      
    }
    return r;
}
