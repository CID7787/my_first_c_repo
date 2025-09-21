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
  
  double exp_double2float(dbits a, fbits b, error* err){
      if(!err){ return a.d; }
      unsigned char b_overfl_cond = b.parts.exp > MAX_NORM_FLOAT_EXP;
      *err = ternary(b_overfl_cond, ternary(b.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
      *err = ternary(b_overfl_cond && b.parts.mantissa, QNAN, *err);// check for NaN
      *err = ternary(have_frac_part((dbits){ .d = b.f}), ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER, *err); 
      *err = ternary(!(a.d) && !(b.f), ZERO_TO_ZERO, *err);
      dbits result = (dbits){ .d = 1}; 
      if(b.f < 0){ 
          while(b.f++ && !(*err)){  
            result.d = safe_double_division_with_rounding(result, a, err);
          }
      }
      else{
          while(b.f-- && !(*err)){  
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
    *err = ternary(have_frac_part((dbits){ .d = b.f}), ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER, *err); 
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
  
  
  vecN vector_exponentiation(vecN a, vecN b){
      unsigned char a_elem_size = amount_of_type_bytes(a.type), b_elem_size = amount_of_type_bytes(b.type), r_element_size;
      r_element_size = ternary(a_elem_size > b_elem_size, a_elem_size, b_elem_size);
      vecN r = {a.type, a.n, r_element_size, malloc(a.n * r_element_size), NO_ERROR};
      while(a.n--){
          switch(a.type){
              case DOUBLE:
                  switch(b.type){
                      case DOUBLE: r.elements.b8[a.n].d = exp_double2double((dbits){ .d = a.elements.b8[a.n].d}, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break;// DONE
                      case FLOAT:  r.elements.b8[a.n].d = exp_double2float( (dbits){ .d = a.elements.b8[a.n].d}, (fbits){ .f = b.elements.b4[a.n].f}, &r.v_error); break;// DONE
                      case CHAR:   r.elements.b8[a.n].d = exp_double2lint(  (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b1[a.n].i,  &r.v_error); break;// DONE
                      case UCHAR:  r.elements.b8[a.n].d = exp_double2luint( (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b1[a.n].ui, &r.v_error); break;// DONE
                      case INT:    r.elements.b8[a.n].d = exp_double2lint(  (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b4[a.n].i,  &r.v_error); break;// DONE
                      case UINT:   r.elements.b8[a.n].d = exp_double2luint( (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b4[a.n].ui, &r.v_error); break;// DONE
                      case LINT:   r.elements.b8[a.n].d = exp_double2lint(  (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b8[a.n].i,  &r.v_error); break;// DONE
                      case LUINT:  r.elements.b8[a.n].d = exp_double2luint( (dbits){ .d = a.elements.b8[a.n].d},               b.elements.b8[a.n].ui, &r.v_error); break;// DONE
                  }
              case FLOAT:
                  switch(b.type){
                      case DOUBLE: r.type = DOUBLE; 
                                   r.elements.b8[a.n].d = exp_double2double((dbits){ .d = a.elements.b4[a.n].f}, (dbits){ .d = b.elements.b8[a.n].d}, &r.v_error); break;// DONE
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
                              case CHAR:  r.elements.b8[a.n].i  = pow(a.elements.b1[a.n].i,  b.elements.b8[a.n].d); break; // TODO
                              case UCHAR: r.elements.b8[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b8[a.n].d); break; // TODO
                              case INT:   r.elements.b8[a.n].i  = pow(a.elements.b4[a.n].i,  b.elements.b8[a.n].d); break; // TODO
                              case UINT:  r.elements.b8[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b8[a.n].d); break; // TODO
                              case LINT:  r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i,  b.elements.b8[a.n].d); break; // TODO
                              case LUINT: r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b8[a.n].d); break; // TODO
                          }
                      break;
                      case FLOAT: 
                      switch(a.type){
                          case CHAR:  r.elements.b4[a.n].i  = pow(a.elements.b1[a.n].i,  b.elements.b4[a.n].f); break; // TODO
                          case UCHAR: r.elements.b4[a.n].ui = pow(a.elements.b1[a.n].ui, b.elements.b4[a.n].f); break; // TODO
                          case INT:   r.elements.b4[a.n].i  = pow(a.elements.b4[a.n].i,  b.elements.b4[a.n].f); break; // TODO
                          case UINT:  r.elements.b4[a.n].ui = pow(a.elements.b4[a.n].ui, b.elements.b4[a.n].f); break; // TODO
                          case LINT:  r.elements.b8[a.n].i  = pow(a.elements.b8[a.n].i,  b.elements.b4[a.n].f); break; // TODO
                          case LUINT: r.elements.b8[a.n].ui = pow(a.elements.b8[a.n].ui, b.elements.b4[a.n].f); break; // TODO
                      }
                      break;
                      default:
                          if (amount_of_type_bytes(r.type) == amount_of_type_bytes(a.type)) { b.type = a.type; }
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
