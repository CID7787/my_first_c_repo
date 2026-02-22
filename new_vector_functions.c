#ifndef headerfile
    #include <stdio.h>
    #include <stdlib.h>
    #include <stdint.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
    #include "additional_functions.c"
#endif

#define matrix_elem(m, n, col) col * m + n

#define B1type_i_elements(...) (alldatapointer){ .b1 = (B1type){ .i = __VA_ARGS__}}
#define B1type_ui_elements(...) (alldatapointer){ .b1 = (B1type){ .ui = __VA_ARGS__}}
#define B4type_i_elements(...) (alldatapointer){ .b4 = (B4type){ .i = __VA_ARGS__}} 
#define B4type_ui_elements(...) (alldatapointer){ .b4 = (B4type){ .ui = __VA_ARGS__}}
#define B4type_f_elements(...) (alldatapointer){ .b4 = (B4type){ .f = __VA_ARGS__}}
#define B8type_i_elements(...) (alldatapointer){ .b8 = (B8type){ .i = __VA_ARGS__}} 
#define B8type_ui_elements(...) (alldatapointer){ .b8 = (B8type){ .ui = __VA_ARGS__}} 
#define B8type_d_elements(...) (alldatapointer){ .b8 = (B8type){ .d = __VA_ARGS__}} 

void print_vector(vecN a){
    unsigned int i = 0;
    printf("Vector type: %s, elements amount: %u, vector error: %s\nvector elements:\n", fund_types_sting_arr[a.type], a.n, errors_sting_arr[a.v_error]);
    while(i < a.n){
        print_fund_type_func_arr[a.type]((datapointer){ .i8 = a.elements.b1.i + i * amount_of_type_bytes(a.type) } );
        printf(", ");
        i++;
    }    
}

void print_matrix(matrix_t a){
    unsigned int i = 0;
    printf("Matrix type: %s, size: %ux%u, matrix error: %s\nmatrix elements:\n", fund_types_sting_arr[a.type], a.dim.row, a.dim.col, errors_sting_arr[a.m_err]);
    while(i < a.dim.row * a.dim.col){
        if(!(i % a.dim.col)){ printf("\n"); }
        print_fund_type_func_arr[a.type]((datapointer){ .i8 = a.elements.b1.i + ((i / a.dim.col) * a.dim.col + (i % a.dim.col)) * amount_of_type_bytes(a.type) } );
        printf(" ");
        i++;
    }    
}

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

vecN vector_addition_of_arg1type(vecN a, vecN b){
    unsigned int i, r_n = ternary(a.n > b.n, a.n, b.n), ai, bi, cond;
    vecN r = {a.type, r_n, malloc(r_n * amount_of_type_bytes(a.type)), NO_ERROR};
    vecN *vec_arr[3] = {&(vecN){CHAR, 1, B8type_i_elements((long int[]){0}), NO_ERROR}};
    for(i = 0; i < r_n; i++){
        ai = i < a.n;
        bi = i < b.n;
        switch(a.type){
            case CHAR:
                switch(b.type){
                    case CHAR: 
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], vec_arr[bi << 1]->elements.b1.i[bi * i], &r.v_error);
                    break;
                    case UCHAR:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b1.ui[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], vec_arr[bi << 1]->elements.b1.ui[bi * i], &r.v_error);
                    break;
                    case INT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.i[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], vec_arr[bi << 1]->elements.b4.i[bi * i], &r.v_error);                      
                    break;
                    case UINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.ui[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], vec_arr[bi << 1]->elements.b4.ui[bi * i], &r.v_error);
                      break;
                    case LINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.i[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], vec_arr[bi << 1]->elements.b8.i[bi * i], &r.v_error);                      
                    break;
                    case LUINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], vec_arr[bi << 1]->elements.b8.ui[bi * i], &r.v_error);
                    break;
                    case FLOAT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], (vec_arr[bi << 1]->elements.b4.f[bi * i]), &r.v_error);                      
                    break; 
                    case DOUBLE:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.d[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.d[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], (vec_arr[bi << 1]->elements.b8.d[bi * i]), &r.v_error);                      
                } 
            break;
            case UCHAR:
                switch(b.type){
                    case CHAR: 
                        cond = vec_arr[bi << 1]->elements.b1.i[bi * i] < 0;
                        cond &= int_absolute_value(vec_arr[bi << 1]->elements.b1.i[bi * i]) > vec_arr[ai]->elements.b1.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[ai * i] + (int8_t)(vec_arr[bi << 1]->elements.b1.i[bi * i] & (char)-cond), 
                                                                  vec_arr[bi << 1]->elements.b1.i[bi * i] & (int8_t)-(!cond), &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[ai * i], vec_arr[bi << 1]->elements.b1.ui[bi * i], &r.v_error);
                    break;
                    case INT:
                        cond = vec_arr[bi << 1]->elements.b4.i[bi * i] < 0;
                        cond &= int_absolute_value(vec_arr[bi << 1]->elements.b4.i[bi * i]) > vec_arr[ai]->elements.b1.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[ai * i] + (int32_t)(vec_arr[bi << 1]->elements.b4.i[bi * i] & (int)-cond), 
                                                                  vec_arr[bi << 1]->elements.b4.i[bi * i] & (int32_t)-(!cond), &r.v_error);
                    break;
                    case UINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.ui[bi * i] > MAX_UCHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[i], vec_arr[bi << 1]->elements.b4.ui[i], &r.v_error);
                    break;
                    case LINT:
                        cond = vec_arr[bi << 1]->elements.b8.i[bi * i] < 0;
                        cond &= lint_absolute_value(vec_arr[bi << 1]->elements.b8.i[bi * i]) > vec_arr[ai]->elements.b1.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[ai * i] + (int64_t)(vec_arr[bi << 1]->elements.b8.i[bi * i] & (long int)-cond),
                                                                  vec_arr[bi << 1]->elements.b8.i[bi * i] & (int64_t)-(!cond), &r.v_error);
                    break;
                    case LUINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_UCHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[i], vec_arr[bi << 1]->elements.b8.ui[i], &r.v_error);
                    break;
                    case FLOAT:
                        cond = vec_arr[bi << 1]->elements.b4.f[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = vec_arr[bi << 1]->elements.b4.f[bi * i] }) > vec_arr[ai]->elements.b1.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[ai * i] + vec_arr[bi << 1]->elements.b4.f[bi * i] * cond, 
                                                                  vec_arr[bi << 1]->elements.b4.f[bi * i] * !cond, &r.v_error);
                    break; 
                    case DOUBLE:
                        cond = vec_arr[bi << 1]->elements.b8.d[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = vec_arr[bi]->elements.b8.d[bi * i] }) > vec_arr[ai]->elements.b1.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary((double)vec_arr[bi << 1]->elements.b8.d[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[i] = safe_uchar_addition(vec_arr[ai]->elements.b1.ui[ai * i] + vec_arr[bi << 1]->elements.b8.d[bi * i] * cond, 
                                                                  vec_arr[bi << 1]->elements.b8.d[bi * i] * !cond, &r.v_error);
                } 
            break;
            case INT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], vec_arr[bi << 1]->elements.b1.i[bi * i], &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], vec_arr[bi << 1]->elements.b1.ui[bi * i], &r.v_error);
                    break;
                    case INT:
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], vec_arr[bi << 1]->elements.b4.i[bi * i], &r.v_error);                      
                    break;
                    case UINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.ui[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], vec_arr[bi << 1]->elements.b4.ui[bi * i], &r.v_error);
                        break;
                    case LINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.i[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.i[bi * i] < MIN_INT, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], vec_arr[bi << 1]->elements.b8.i[bi * i], &r.v_error);                      
                    break;
                    case LUINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], vec_arr[bi << 1]->elements.b8.ui[bi * i], &r.v_error);
                    break;
                    case FLOAT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] < MIN_INT, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], (vec_arr[bi << 1]->elements.b4.f[bi * i]), &r.v_error);                      
                    break; 
                    case DOUBLE:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.d[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.d[bi * i] < MIN_INT, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b4.i[i] = safe_int_addition(vec_arr[ai]->elements.b4.i[ai * i], (vec_arr[bi << 1]->elements.b8.d[bi * i]), &r.v_error);                      
                }
            break;
            case UINT:
                switch(b.type){
                    case CHAR: 
                        cond = vec_arr[bi << 1]->elements.b1.i[bi * i] < 0;
                        cond &= int_absolute_value(vec_arr[bi << 1]->elements.b1.i[bi * i]) > vec_arr[ai]->elements.b4.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[ai * i] + (int8_t)(vec_arr[bi << 1]->elements.b1.i[bi * i] & (char)-cond), 
                                                                vec_arr[bi << 1]->elements.b1.i[bi * i] & (int8_t)-(!cond), &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b4.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[ai * i], vec_arr[bi << 1]->elements.b1.ui[bi * i], &r.v_error);
                    break;
                    case INT:
                        cond = vec_arr[bi << 1]->elements.b4.i[bi * i] < 0;
                        cond &= int_absolute_value(vec_arr[bi << 1]->elements.b4.i[bi * i]) > vec_arr[ai]->elements.b4.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[ai * i] + (int32_t)(vec_arr[bi << 1]->elements.b4.i[bi * i] & (int)-cond), 
                                                                vec_arr[bi << 1]->elements.b4.i[bi * i] & (int32_t)-(!cond), &r.v_error);
                    break;
                    case UINT:
                        r.elements.b1.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[i], vec_arr[bi << 1]->elements.b4.ui[i], &r.v_error);
                    break;
                    case LINT:
                        cond = vec_arr[bi << 1]->elements.b8.i[bi * i] < 0;
                        cond &= lint_absolute_value(vec_arr[bi << 1]->elements.b8.i[bi * i]) > vec_arr[ai]->elements.b4.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[ai * i] + (int64_t)(vec_arr[bi << 1]->elements.b8.i[bi * i] & (long int)-cond),
                                                                vec_arr[bi << 1]->elements.b8.i[bi * i] & (int64_t)-(!cond), &r.v_error);
                    break;
                    case LUINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[i], vec_arr[bi << 1]->elements.b8.ui[i], &r.v_error);
                    break;
                    case FLOAT:
                        cond = vec_arr[bi << 1]->elements.b4.f[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = vec_arr[bi << 1]->elements.b4.f[bi * i] }) > vec_arr[ai]->elements.b4.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[ai * i] + vec_arr[bi << 1]->elements.b4.f[bi * i] * cond, 
                                                                vec_arr[bi << 1]->elements.b4.f[bi * i] * !cond, &r.v_error);
                    break; 
                    case DOUBLE:
                        cond = vec_arr[bi << 1]->elements.b8.d[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = vec_arr[bi]->elements.b8.d[bi * i] }) > vec_arr[ai]->elements.b4.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary((double)vec_arr[bi << 1]->elements.b8.d[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[i] = safe_uint_addition(vec_arr[ai]->elements.b4.ui[ai * i] + vec_arr[bi << 1]->elements.b8.d[bi * i] * cond, 
                                                                vec_arr[bi << 1]->elements.b8.d[bi * i] * !cond, &r.v_error);
                }
            break;
            case LINT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], vec_arr[bi << 1]->elements.b1.i[bi * i], &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], vec_arr[bi << 1]->elements.b1.ui[bi * i], &r.v_error);
                    break;
                    case INT:
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], vec_arr[bi << 1]->elements.b4.i[bi * i], &r.v_error);                      
                    break;
                    case UINT:
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], vec_arr[bi << 1]->elements.b4.ui[bi * i], &r.v_error);
                        break;
                    case LINT:
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], vec_arr[bi << 1]->elements.b8.i[bi * i], &r.v_error);                      
                    break;
                    case LUINT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_LINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], vec_arr[bi << 1]->elements.b8.ui[bi * i], &r.v_error);
                    break;
                    case FLOAT:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] > MAX_LINT, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] < MIN_LINT, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], (vec_arr[bi << 1]->elements.b4.f[bi * i]), &r.v_error);                      
                    break; 
                    case DOUBLE:
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.d[bi * i] > MAX_LINT, POSITIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.d[bi * i] < MIN_LINT, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b8.i[i] = safe_lint_addition(vec_arr[ai]->elements.b8.i[ai * i], (vec_arr[bi << 1]->elements.b8.d[bi * i]), &r.v_error);                      
                }
            break;
            case LUINT:
                switch(b.type){
                    case CHAR: 
                        cond = vec_arr[bi << 1]->elements.b1.i[bi * i] < 0;
                        cond &= int_absolute_value(vec_arr[bi << 1]->elements.b1.i[bi * i]) > vec_arr[ai]->elements.b8.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b8.ui[ai * i] + (int8_t)(vec_arr[bi << 1]->elements.b1.i[bi * i] & (char)-cond), 
                                                                vec_arr[bi << 1]->elements.b1.i[bi * i] & (int8_t)-(!cond), &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b8.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b4.ui[ai * i], vec_arr[bi << 1]->elements.b1.ui[bi * i], &r.v_error);
                    break;
                    case INT:
                        cond = vec_arr[bi << 1]->elements.b4.i[bi * i] < 0;
                        cond &= int_absolute_value(vec_arr[bi << 1]->elements.b4.i[bi * i]) > vec_arr[ai]->elements.b8.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b8.ui[ai * i] + (int32_t)(vec_arr[bi << 1]->elements.b4.i[bi * i] & (int)-cond), 
                                                                vec_arr[bi << 1]->elements.b4.i[bi * i] & (int32_t)-(!cond), &r.v_error);
                    break;
                    case UINT:
                        r.elements.b1.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b4.ui[i], vec_arr[bi << 1]->elements.b4.ui[i], &r.v_error);
                    break;
                    case LINT:
                        cond = vec_arr[bi << 1]->elements.b8.i[bi * i] < 0;
                        cond &= lint_absolute_value(vec_arr[bi << 1]->elements.b8.i[bi * i]) > vec_arr[ai]->elements.b8.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b8.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b8.ui[ai * i] + (int64_t)(vec_arr[bi << 1]->elements.b8.i[bi * i] & (long int)-cond),
                                                                vec_arr[bi << 1]->elements.b8.i[bi * i] & (int64_t)-(!cond), &r.v_error);
                    break;
                    case LUINT:
                        r.elements.b8.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b4.ui[i], vec_arr[bi << 1]->elements.b8.ui[i], &r.v_error);
                    break;
                    case FLOAT:
                        cond = vec_arr[bi << 1]->elements.b4.f[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = vec_arr[bi << 1]->elements.b4.f[bi * i] }) > vec_arr[ai]->elements.b8.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(vec_arr[bi << 1]->elements.b4.f[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b8.ui[ai * i] + vec_arr[bi << 1]->elements.b4.f[bi * i] * cond, 
                                                                vec_arr[bi << 1]->elements.b4.f[bi * i] * !cond, &r.v_error);
                    break; 
                    case DOUBLE:
                        cond = vec_arr[bi << 1]->elements.b8.d[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = vec_arr[bi]->elements.b8.d[bi * i] }) > vec_arr[ai]->elements.b8.ui[ai * i];
                        r.v_error = ternary(cond, NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary((double)vec_arr[bi << 1]->elements.b8.d[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[i] = safe_luint_addition(vec_arr[ai]->elements.b8.ui[ai * i] + vec_arr[bi << 1]->elements.b8.d[bi * i] * cond, 
                                                                vec_arr[bi << 1]->elements.b8.d[bi * i] * !cond, &r.v_error);
                }
            break;
            case FLOAT: 
                switch(b.type){
                    case CHAR: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b1.i[i] }, &r.v_error);
                    break;
                    case UCHAR: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b1.ui[i] }, &r.v_error);
                    break;
                    case INT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b4.i[i] }, &r.v_error);
                    break;
                    case UINT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b4.ui[i] }, &r.v_error);
                    break;
                    case LINT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b8.i[i] }, &r.v_error);
                    break;
                    case LUINT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b8.ui[i] }, &r.v_error);
                    break;
                    case FLOAT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b4.f[i] }, &r.v_error);
                    break;
                    case DOUBLE: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = vec_arr[ai]->elements.b4.f[i] }, (fbits){ .f = vec_arr[bi << 1]->elements.b8.d[i] }, &r.v_error);
                }
            break;
            case DOUBLE:
                switch(b.type){
                    case CHAR: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b1.i[i] }, &r.v_error);
                    break;
                    case UCHAR: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b1.ui[i] }, &r.v_error);
                    break;
                    case INT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b4.i[i] }, &r.v_error);
                    break;
                    case UINT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b4.ui[i] }, &r.v_error);
                    break;
                    case LINT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b8.i[i] }, &r.v_error);
                    break;
                    case LUINT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b8.ui[i] }, &r.v_error);
                    break;
                    case FLOAT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b4.f[i] }, &r.v_error);
                    break;
                    case DOUBLE: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = vec_arr[ai]->elements.b8.d[i] }, (dbits){ .d = vec_arr[bi << 1]->elements.b8.d[i] }, &r.v_error);
                }
            break;
            default: return r;
        }
    }
    return r;
}

struct int_8_bit_parts{
    int8_t mang: 7;
    int8_t sign: 1;
}typedef int8_parts;

union int_8_bit_construction{
    int8_parts parts;
    int8_t val;
}typedef int8_constr;

struct int_32_bit_parts{
    int32_t mang: 31;
    int32_t sign: 1;
}typedef int32_parts;

union int_32_bit_construction{
    int32_parts parts;
    int32_t val;
}typedef int32_constr;

struct int_64_bit_parts{
    int64_t sign: 63;
    int64_t mang: 1;
}typedef int64_parts;

union int_64_bit_construction{
    int64_parts parts;
    int64_t val;
}typedef int64_constr;

vecN vector_multiplication_of_arg1type(vecN a, vecN b){
    vecN r = {a.type, a.n, malloc(amount_of_type_bytes(a.type) * a.n), NO_ERROR};
    int8_constr int8_p;
    int32_constr int32_p;
    int64_constr int64_p;
    fbits f32_p;
    dbits d64_p;
    while(a.n--){
        switch(a.type){
            case CHAR: 
                switch(b.type){
                    case CHAR: 
                        r.elements.b1.i[a.n] = safe_char_multiplication(a.elements.b1.i[a.n], b.elements.b1.i[a.n], &r.v_error);
                    break;
                    case UCHAR:
                        r.v_error = ternary(a.elements.b1.i[a.n] && b.elements.b1.ui[a.n] > MAX_CHAR, ternary(a.elements.b1.i[a.n] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b1.i[a.n] = safe_char_multiplication(a.elements.b1.i[a.n], b.elements.b1.ui[a.n], &r.v_error);
                    break;
                    case INT:
                        int8_p.val = a.elements.b1.i[a.n];
                        int32_p.val = b.elements.b4.i[a.n];
                        r.v_error = ternary(int8_p.val && (int32_p.val > MAX_CHAR), ternary(int8_p.parts.sign ^ int32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int8_p.val && (int32_p.val < MIN_CHAR), ternary(int8_p.parts.sign ^ int32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b1.i[a.n] = safe_char_multiplication(int8_p.val, int32_p.val, &r.v_error);
                    break;
                    case UINT:
                        r.v_error = ternary(a.elements.b1.i[a.n] && b.elements.b4.ui[a.n] > MAX_CHAR, ternary(a.elements.b1.i[a.n] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b1.i[a.n] = safe_char_multiplication(a.elements.b1.i[a.n], b.elements.b4.ui[a.n], &r.v_error);                    
                    break;
                    case LINT:
                        int8_p.val = a.elements.b1.i[a.n];
                        int64_p.val = b.elements.b8.i[a.n];
                        r.v_error = ternary(int8_p.val && (int64_p.val > MAX_CHAR), ternary(int8_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int8_p.val && (int64_p.val < MIN_CHAR), ternary(int8_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b1.i[a.n] = safe_char_multiplication(int8_p.val, int64_p.val, &r.v_error);
                    break;
                    case LUINT:
                        r.v_error = ternary(a.elements.b1.i[a.n] && b.elements.b8.ui[a.n] > MAX_CHAR, ternary(a.elements.b1.i[a.n] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b1.i[a.n] = safe_char_multiplication(a.elements.b1.i[a.n], b.elements.b8.ui[a.n], &r.v_error);
                    break;
                    case FLOAT:
                        int8_p.val = a.elements.b1.i[a.n];
                        f32_p.f = b.elements.b4.f[a.n];
                        r.v_error = ternary(int8_p.val && (f32_p.f > MAX_CHAR), ternary(int8_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int8_p.val && (f32_p.f < MIN_CHAR), ternary(int8_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b1.i[a.n] = safe_char_multiplication(int8_p.val, f32_p.f, &r.v_error);
                    break;
                    case DOUBLE:
                        int8_p.val = a.elements.b1.i[a.n];
                        d64_p.d = b.elements.b8.d[a.n];
                        r.v_error = ternary(int8_p.val && (d64_p.d > MAX_CHAR), ternary(int8_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int8_p.val && (d64_p.d < MIN_CHAR), ternary(int8_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b1.i[a.n] = safe_char_multiplication(int8_p.val, d64_p.d, &r.v_error);
                }
            break;
            case UCHAR:
                switch(b.type){
                    case CHAR: 
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b1.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b1.i[a.n], &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b1.ui[a.n], &r.v_error);                        
                    break;
                    case INT:
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b4.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b4.i[a.n] > MAX_UCHAR), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b4.i[a.n], &r.v_error);
                    break;
                    case UINT:
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b4.ui[a.n] > MAX_UCHAR), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b4.ui[a.n], &r.v_error);                        
                    break;
                    case LINT:
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b8.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b8.i[a.n] > MAX_UCHAR), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b8.i[a.n], &r.v_error);
                    break;
                    case LUINT:
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b8.ui[a.n] > MAX_UCHAR), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b8.ui[a.n], &r.v_error);                        
                    break;
                    case FLOAT:
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b4.f[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b4.f[a.n] > MAX_UCHAR), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b4.f[a.n], &r.v_error);
                    break;
                    case DOUBLE:
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b8.d[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b1.ui[a.n] && (b.elements.b8.d[a.n] > MAX_UCHAR), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b1.ui[a.n] = safe_uchar_multiplication(a.elements.b1.ui[a.n], b.elements.b8.d[a.n], &r.v_error);
                }
            break;
            case INT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b1.i[a.n] = safe_int_multiplication(a.elements.b4.i[a.n], b.elements.b1.i[a.n], &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b4.i[a.n] = safe_int_multiplication(a.elements.b4.i[a.n], b.elements.b1.ui[a.n], &r.v_error);
                    break;
                    case INT:
                        r.elements.b4.i[a.n] = safe_int_multiplication(a.elements.b4.i[a.n], b.elements.b4.i[a.n], &r.v_error);
                    break;
                    case UINT:
                        r.v_error = ternary(a.elements.b4.i[a.n] && b.elements.b4.ui[a.n] > MAX_CHAR, ternary(a.elements.b1.i[a.n] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b4.i[a.n] = safe_int_multiplication(a.elements.b4.i[a.n], b.elements.b4.ui[a.n], &r.v_error);                    
                    break;
                    case LINT:
                        int32_p.val = a.elements.b4.i[a.n];
                        int64_p.val = b.elements.b8.i[a.n];
                        r.v_error = ternary(int32_p.val && (int64_p.val > MAX_INT), ternary(int32_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int32_p.val && (int64_p.val < MIN_INT), ternary(int32_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b4.i[a.n] = safe_int_multiplication(int32_p.val, int64_p.val, &r.v_error);
                    break;
                    case LUINT:
                        r.v_error = ternary(a.elements.b4.i[a.n] && b.elements.b8.ui[a.n] > MAX_INT, ternary(a.elements.b4.i[a.n] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b4.i[a.n] = safe_int_multiplication(a.elements.b4.i[a.n], b.elements.b8.ui[a.n], &r.v_error);
                    break;
                    case FLOAT:
                        int32_p.val = a.elements.b4.i[a.n];
                        f32_p.f = b.elements.b4.f[a.n];
                        r.v_error = ternary(int32_p.val && (f32_p.f > MAX_INT), ternary(int32_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int32_p.val && (f32_p.f < MIN_INT), ternary(int32_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b4.i[a.n] = safe_int_multiplication(int32_p.val, f32_p.f, &r.v_error);
                    break;
                    case DOUBLE:
                        int32_p.val = a.elements.b4.i[a.n];
                        d64_p.d = b.elements.b8.d[a.n];
                        r.v_error = ternary(int32_p.val && (d64_p.d > MAX_INT), ternary(int32_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int32_p.val && (d64_p.d < MIN_INT), ternary(int32_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b4.i[a.n] = safe_int_multiplication(int32_p.val, d64_p.d, &r.v_error);
                }
            break;
            case UINT:
                switch(b.type){
                    case CHAR: 
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b4.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b1.i[a.n], &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b1.ui[a.n], &r.v_error);                        
                    break;
                    case INT:
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b4.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b4.i[a.n], &r.v_error);
                    break;
                    case UINT:
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b4.ui[a.n], &r.v_error);                        
                    break;
                    case LINT:
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b8.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b8.i[a.n] > MAX_UINT), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b8.i[a.n], &r.v_error);
                    break;
                    case LUINT:
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b8.ui[a.n] > MAX_UINT), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b8.ui[a.n], &r.v_error);                        
                    break;
                    case FLOAT:
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b4.f[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b4.f[a.n] > MAX_UINT), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b4.f[a.n], &r.v_error);
                    break;
                    case DOUBLE:
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b8.d[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b8.d[a.n] > MAX_UINT), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b4.ui[a.n] = safe_uint_multiplication(a.elements.b4.ui[a.n], b.elements.b8.d[a.n], &r.v_error);
                }
            break; 
            case LINT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b8.i[a.n] = safe_lint_multiplication(a.elements.b8.i[a.n], b.elements.b1.i[a.n], &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b8.i[a.n] = safe_lint_multiplication(a.elements.b8.i[a.n], b.elements.b1.ui[a.n], &r.v_error);
                    break;
                    case INT:
                        r.elements.b8.i[a.n] = safe_lint_multiplication(a.elements.b8.i[a.n], b.elements.b4.i[a.n], &r.v_error);
                    break;
                    case UINT:
                        r.elements.b8.i[a.n] = safe_lint_multiplication(a.elements.b8.i[a.n], b.elements.b4.ui[a.n], &r.v_error);                    
                    break;
                    case LINT:
                        r.elements.b8.i[a.n] = safe_lint_multiplication(int32_p.val, int64_p.val, &r.v_error);
                    break;
                    case LUINT:
                        r.v_error = ternary(a.elements.b8.i[a.n] && b.elements.b8.ui[a.n] > MAX_LINT, ternary(a.elements.b8.i[a.n] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b8.i[a.n] = safe_lint_multiplication(a.elements.b8.i[a.n], b.elements.b8.ui[a.n], &r.v_error);
                    break;
                    case FLOAT:
                        int64_p.val = a.elements.b8.i[a.n];
                        f32_p.f = b.elements.b4.f[a.n];
                        r.v_error = ternary(int64_p.val && (f32_p.f > MAX_LINT), ternary(int64_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int64_p.val && (f32_p.f < MIN_LINT), ternary(int64_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b8.i[a.n] = safe_lint_multiplication(int64_p.val, f32_p.f, &r.v_error);
                    break;
                    case DOUBLE:
                        int64_p.val = a.elements.b8.i[a.n];
                        d64_p.d = b.elements.b8.d[a.n];
                        r.v_error = ternary(int64_p.val && (d64_p.d > MAX_LINT), ternary(int64_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.v_error = ternary(int64_p.val && (d64_p.d < MIN_LINT), ternary(int64_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.v_error);
                        r.elements.b8.i[a.n] = safe_lint_multiplication(int64_p.val, d64_p.d, &r.v_error);
                }
            break;
            case LUINT:
                switch(b.type){
                    case CHAR: 
                        r.v_error = ternary(a.elements.b4.ui[a.n] && (b.elements.b4.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b1.i[a.n], &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b1.ui[a.n], &r.v_error);                        
                    break;
                    case INT:
                        r.v_error = ternary(a.elements.b8.ui[a.n] && (b.elements.b4.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b4.i[a.n], &r.v_error);
                    break;
                    case UINT:
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b4.ui[a.n], &r.v_error);                        
                    break;
                    case LINT:
                        r.v_error = ternary(a.elements.b8.ui[a.n] && (b.elements.b8.i[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b8.i[a.n], &r.v_error);
                    break;
                    case LUINT:
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b8.ui[a.n], &r.v_error);                        
                    break;
                    case FLOAT:
                        r.v_error = ternary(a.elements.b8.ui[a.n] && (b.elements.b4.f[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b8.ui[a.n] && (b.elements.b4.f[a.n] > MAX_LUINT), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b8.ui[a.n], b.elements.b4.f[a.n], &r.v_error);
                    break;
                    case DOUBLE:
                        r.v_error = ternary(a.elements.b8.ui[a.n] && (b.elements.b8.d[a.n] < 0), NEGATIVE_OVERFLOW, r.v_error);
                        r.v_error = ternary(a.elements.b8.ui[a.n] && (b.elements.b8.d[a.n] > MAX_LUINT), POSITIVE_OVERFLOW, r.v_error);
                        r.elements.b8.ui[a.n] = safe_luint_multiplication(a.elements.b4.ui[a.n], b.elements.b8.d[a.n], &r.v_error);
                }
            break;
            case FLOAT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b1.i[a.n] }, &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b1.ui[a.n] }, &r.v_error);
                    break;
                    case INT:
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b4.i[a.n] }, &r.v_error);
                    break;
                    case UINT:
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b4.ui[a.n] }, &r.v_error);
                    break;
                    case LINT:
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b8.i[a.n] }, &r.v_error);
                    break;
                    case LUINT:
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b8.ui[a.n] }, &r.v_error);
                    break;
                    case FLOAT:
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b4.f[a.n] }, &r.v_error);
                    break;
                    case DOUBLE:
                        r.elements.b4.f[a.n] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.n] }, (fbits){ .f = b.elements.b8.d[a.n] }, &r.v_error);
                }
            break;
            case DOUBLE:
                switch(b.type){
                    case CHAR: 
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b1.i[a.n] }, &r.v_error);
                    break;
                    case UCHAR:
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b1.ui[a.n] }, &r.v_error);
                    break;
                    case INT:
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b4.i[a.n] }, &r.v_error);
                    break;
                    case UINT:
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b4.ui[a.n] }, &r.v_error);
                    break;
                    case LINT:
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b8.i[a.n] }, &r.v_error);
                    break;
                    case LUINT:
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b8.ui[a.n] }, &r.v_error);
                    break;
                    case FLOAT:
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b4.f[a.n] }, &r.v_error);
                    break;
                    case DOUBLE:
                        r.elements.b8.d[a.n] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.n] }, (dbits){ .d = b.elements.b8.d[a.n] }, &r.v_error);
                }
            break;
            default: return r;
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

/* 
switch(a.type){
    case INT:
    switch(b.type){
            case INT:
                operation_between_INT_and_INT(a, b);
            break;
            case UINT:
                operation_between_INT_and_UINT(a, b);
            break;
        }
    break;
    case UINT:
        switch(b.type){
            case INT:
                // ,,,
                operation_between_UINT_and_INT(a,b);
                // ..
            break;
            case UINT:
                operation_between_UINT_and_UINT(a,b);
            break;
        }
    break;
}

figure out what are advantages of realization of above switch and switch below with size, integral type, float type etc.
why did I picked up the realization of switch as is above?

switch(a.type.size):
    case 4:
    case 8:
        switch(b.type.size):
            case 4: operation_between_8_and_4(a, b);
            case 8: operation_between_8_and_8(a, b);
            case 16: operation_between_8_and_16(a, b);
    case 16:  
*/

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
