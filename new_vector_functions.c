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
    printf("Matrix type: %s, size: %ux%u, matrix error: %s\nmatrix elements:\n", fund_types_sting_arr[a.type], a.row, a.col, errors_sting_arr[a.m_err]);
    while(i < a.row * a.col){
        if(!(i % a.col)){ printf("\n"); }
        print_fund_type_func_arr[a.type]((datapointer){ .i8 = a.elements.b1.i + ((i / a.col) * a.col + (i % a.col)) * amount_of_type_bytes(a.type) } );
        printf(" ");
        i++;
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
        }
    }
    return r;
}   

matrix_t matrix_creation(datatype type, unsigned int row, unsigned int col, alldatapointer elements, datapointer lambda){
    col = ternary(!!(uint64_t)elements.b1.i, col, row);
    unsigned int n = row * col, r_elem_size = amount_of_type_bytes(type);
    matrix_t r = {type, row, col, malloc(row * col * r_elem_size), NO_ERROR};
    int32_t *ptr1 = elements.b4.i, *ptr2 = lambda.i32;
    while(n--){
        switch(type | -(!ptr1)){
            case CHAR:   r.elements.b1.i [n] = elements.b1.i [n]; break;
            case UCHAR:  r.elements.b1.ui[n] = elements.b1.ui[n]; break;
            case INT:    r.elements.b4.ui[n] = elements.b4.i [n]; break;
            case UINT:   r.elements.b4.ui[n] = elements.b4.ui[n]; break;
            case LINT:   r.elements.b8.i [n] = elements.b8.i [n]; break;
            case LUINT:  r.elements.b8.ui[n] = elements.b8.ui[n]; break;
            case FLOAT:  r.elements.b4.f [n] = elements.b4.f [n]; break;
            case DOUBLE: r.elements.b8.d [n] = elements.b8.d [n]; break;
            default:
                switch(type | -(!ptr2)){
                    case CHAR:   r.elements.b1.i [n] = !(n % (row + 1)) * *(lambda.i8  ); break;
                    case UCHAR:  r.elements.b1.ui[n] = !(n % (row + 1)) * *(lambda.ui8 ); break;
                    case INT:    r.elements.b4.i [n] = !(n % (row + 1)) * *(lambda.i32 ); break;
                    case UINT:   r.elements.b4.ui[n] = !(n % (row + 1)) * *(lambda.ui32); break;
                    case LINT:   r.elements.b1.i [n] = !(n % (row + 1)) * *(lambda.i64 ); break;
                    case LUINT:  r.elements.b8.ui[n] = !(n % (row + 1)) * *(lambda.ui64); break;
                    case FLOAT:  r.elements.b4.f [n] = !(n % (row + 1)) * *(lambda.f32 ); break;
                    case DOUBLE: r.elements.b8.d [n] = !(n % (row + 1)) * *(lambda.f64 ); break;
                    default: 
                        switch(type){
                            case CHAR:   r.elements.b1.i [n] = !(n % (row + 1)) * 1; break;
                            case UCHAR:  r.elements.b1.ui[n] = !(n % (row + 1)) * 1; break;
                            case INT:    r.elements.b4.i [n] = !(n % (row + 1)) * 1; break;
                            case UINT:   r.elements.b4.ui[n] = !(n % (row + 1)) * 1; break;
                            case LINT:   r.elements.b1.i [n] = !(n % (row + 1)) * 1; break;
                            case LUINT:  r.elements.b8.ui[n] = !(n % (row + 1)) * 1; break;
                            case FLOAT:  r.elements.b4.f [n] = !(n % (row + 1)) * 1; break;
                            case DOUBLE: r.elements.b8.d [n] = !(n % (row + 1)) * 1;   
                        }
                }
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

vecN vector_negation_in_place(vecN a){
    uint32_t i = a.n;
    while(i-- ){
        switch(a.type){
            case CHAR: 
                a.v_error = ternary(a.elements.b1.i[i] == MIN_CHAR, POSITIVE_OVERFLOW, a.v_error);  
                a.elements.b1.i[i] = -a.elements.b1.i[i];
            break;
            case INT: 
                a.v_error = ternary(a.elements.b4.i[i] == MIN_INT, POSITIVE_OVERFLOW, a.v_error);
                a.elements.b4.i[i] =  -a.elements.b4.i[i]; 
                break;
            case FLOAT: 
                a.elements.b4.f[i] = -a.elements.b4.f[i]; 
                break;
            case LINT: 
                a.v_error = ternary(a.elements.b8.i[i] == MIN_LINT, POSITIVE_OVERFLOW, a.v_error);
                a.elements.b8.i[i] = -a.elements.b8.i[i]; 
                break;
            case DOUBLE: 
                a.elements.b8.d[i] = -a.elements.b8.d[i]; 
                break;
            default: 
                return a;
        }
    }
    return a;
}

matrix_t matrix_negation(matrix_t matr){
    unsigned int n = matr.col * matr.row;
    matrix_t r = {matr.type, matr.row, matr.col, malloc(matr.col * matr.row * amount_of_type_bytes(matr.type)), NO_ERROR};
    while(n--){
        switch(matr.type){
            case CHAR: 
            matr.m_err = ternary(matr.elements.b1.i[n] == MIN_CHAR, POSITIVE_OVERFLOW, matr.m_err);
            r.elements.b1.i[n] = -matr.elements.b1.i[n]; 
            break;
            case UCHAR: 
                r.elements.b1.ui[n] = matr.elements.b1.ui[n]; 
            break;
            case INT: 
                r.elements.b1.i[n] = -matr.elements.b1.i[n]; 
            break;
            case UINT: 
                r.elements.b4.ui[n] = matr.elements.b4.ui[n]; 
            break;
            case LINT: 
            r.elements.b1.i[n] = -matr.elements.b1.i[n]; 
            break;
            case LUINT: 
                r.elements.b8.ui[n] = matr.elements.b8.ui[n]; 
            break;
            case FLOAT: 
                r.elements.b4.f[n] = -matr.elements.b4.f[n]; 
            break;
            case DOUBLE: 
                r.elements.b8.d[n] = -matr.elements.b8.d[n]; 
            break;
            default: return r;
        }
    }
    return r;
}

matrix_t matrix_negation_in_place(matrix_t matr){
    unsigned int n = matr.col * matr.row;
    while(n--){
        switch(matr.type){
            case CHAR: 
                matr.m_err = ternary(matr.elements.b1.i[n] == MIN_CHAR, POSITIVE_OVERFLOW, matr.m_err);
                matr.elements.b1.i[n] = -matr.elements.b1.i[n]; 
            break;
            case INT: 
                matr.elements.b1.i[n] = -matr.elements.b1.i[n]; 
            break;
            break;
            case LINT: 
                matr.elements.b1.i[n] = -matr.elements.b1.i[n]; 
            break;
            break;
            case FLOAT: 
                matr.elements.b4.f[n] = -matr.elements.b4.f[n]; 
            break;
            case DOUBLE: 
                matr.elements.b8.d[n] = -matr.elements.b8.d[n]; 
            break;
            default: return matr;
        }
    }
    return matr;
}

vecN vector_addition_of_arg1type(vecN a, vecN b){
    uint32_t i, r_n = ternary(a.n > b.n, a.n, b.n);
    vecN r = {a.type, r_n, malloc(r_n * amount_of_type_bytes(a.type)), NO_ERROR};
    uint8_t ai, bi, cond;
    vecN *vec_arr[3] = {&(vecN){CHAR, 1, B8type_i_elements((long int[]){0}), NO_ERROR}};
    for(i = 0; i < r_n; i++){
        ai = i < a.n;
        bi = i < b.n;
        switch(a.type){
            case CHAR:
                switch(b.type){
                    case CHAR: 
                        r.elements.b1.i[i] = safe_char_addition(vec_arr[ai]->elements.b1.i[ai * i], vec_arr[bi << 1]->elements.b1.i[bi * i], &a.v_error);
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

matrix_t matrix_addition(matrix_t a, matrix_t b){
    uint32_t row = ternary(a.row > b.row, a.row, b.row), col = ternary(a.col > b.col, a.col, b.col), i = col * row;
    matrix_t *m_arr[3] = { &(matrix_t){ a.type, 1, 1, B8type_i_elements( (int64_t[]){0} ), NO_ERROR }, &a, &b }, 
                r = { a.type, row, col, malloc(col * row * amount_of_type_bytes(a.type)), NO_ERROR };
    uint8_t ai, bi, cond;
    while(i--){
        ai = (row < a.row) & (col < a.col);
        bi = (row < b.row) & (col < b.col);
        switch(a.type){
            case CHAR:
                switch(b.type){
                    case CHAR: 
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], m_arr[bi << 1]->elements.b1.i[bi * i], &r.m_err);
                    break;
                    case UCHAR:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b1.ui[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], m_arr[bi << 1]->elements.b1.ui[bi * i], &r.m_err);
                    break;
                    case INT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.i[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], m_arr[bi << 1]->elements.b4.i[bi * i], &r.m_err);                      
                    break;
                    case UINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.ui[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], m_arr[bi << 1]->elements.b4.ui[bi * i], &r.m_err);
                    break;
                    case LINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.i[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], m_arr[bi << 1]->elements.b8.i[bi * i], &r.m_err);                      
                    break;
                    case LUINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], m_arr[bi << 1]->elements.b8.ui[bi * i], &r.m_err);
                    break;
                    case FLOAT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], (m_arr[bi << 1]->elements.b4.f[bi * i]), &r.m_err);                      
                    break; 
                    case DOUBLE:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.d[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.d[bi * i] < MIN_CHAR, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b1.i[i] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * i], (m_arr[bi << 1]->elements.b8.d[bi * i]), &r.m_err);                      
                } 
            break;
            case UCHAR:
                switch(b.type){
                    case CHAR: 
                        cond = m_arr[bi << 1]->elements.b1.i[bi * i] < 0;
                        cond &= int_absolute_value(m_arr[bi << 1]->elements.b1.i[bi * i]) > m_arr[ai]->elements.b1.ui[ai * i];
                        r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[ai * i] + (int8_t)(m_arr[bi << 1]->elements.b1.i[bi * i] & (char)-cond), 
                                                                m_arr[bi << 1]->elements.b1.i[bi * i] & (int8_t)-(!cond), &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[ai * i], m_arr[bi << 1]->elements.b1.ui[bi * i], &r.m_err);
                    break;
                    case INT:
                        cond = m_arr[bi << 1]->elements.b4.i[bi * i] < 0;
                        cond &= int_absolute_value(m_arr[bi << 1]->elements.b4.i[bi * i]) > m_arr[ai]->elements.b1.ui[ai * i];
                        r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[ai * i] + (int32_t)(m_arr[bi << 1]->elements.b4.i[bi * i] & (int)-cond), 
                                                                m_arr[bi << 1]->elements.b4.i[bi * i] & (int32_t)-(!cond), &r.m_err);
                    break;
                    case UINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.ui[bi * i] > MAX_UCHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[i], m_arr[bi << 1]->elements.b4.ui[i], &r.m_err);
                    break;
                    case LINT:
                        cond = m_arr[bi << 1]->elements.b8.i[bi * i] < 0;
                        cond &= lint_absolute_value(m_arr[bi << 1]->elements.b8.i[bi * i]) > m_arr[ai]->elements.b1.ui[ai * i];
                        r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.i[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[ai * i] + (int64_t)(m_arr[bi << 1]->elements.b8.i[bi * i] & (long int)-cond),
                                                                m_arr[bi << 1]->elements.b8.i[bi * i] & (int64_t)-(!cond), &r.m_err);
                    break;
                    case LUINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_UCHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[i], m_arr[bi << 1]->elements.b8.ui[i], &r.m_err);
                    break;
                    case FLOAT:
                        cond = m_arr[bi << 1]->elements.b4.f[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = m_arr[bi << 1]->elements.b4.f[bi * i] }) > m_arr[ai]->elements.b1.ui[ai * i];
                        r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[ai * i] + m_arr[bi << 1]->elements.b4.f[bi * i] * cond, 
                                                                m_arr[bi << 1]->elements.b4.f[bi * i] * !cond, &r.m_err);
                    break; 
                    case DOUBLE:
                        cond = m_arr[bi << 1]->elements.b8.d[bi * i] < 0;
                        cond &= double_absolute_value((dbits){ .d = m_arr[bi]->elements.b8.d[bi * i] }) > m_arr[ai]->elements.b1.ui[ai * i];
                        r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary((double)m_arr[bi << 1]->elements.b8.d[bi * i] > MAX_CHAR, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[ai * i] + m_arr[bi << 1]->elements.b8.d[bi * i] * cond, 
                                                                m_arr[bi << 1]->elements.b8.d[bi * i] * !cond, &r.m_err);
                } 
            break;
            case INT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], m_arr[bi << 1]->elements.b1.i[bi * i], &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], m_arr[bi << 1]->elements.b1.ui[bi * i], &r.m_err);
                    break;
                    case INT:
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], m_arr[bi << 1]->elements.b4.i[bi * i], &r.m_err);                      
                    break;
                    case UINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.ui[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], m_arr[bi << 1]->elements.b4.ui[bi * i], &r.m_err);
                        break;
                    case LINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.i[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.i[bi * i] < MIN_INT, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], m_arr[bi << 1]->elements.b8.i[bi * i], &r.m_err);                      
                    break;
                    case LUINT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], m_arr[bi << 1]->elements.b8.ui[bi * i], &r.m_err);
                    break;
                    case FLOAT:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] < MIN_INT, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], (m_arr[bi << 1]->elements.b4.f[bi * i]), &r.m_err);                      
                    break; 
                    case DOUBLE:
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.d[bi * i] > MAX_INT, POSITIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(m_arr[bi << 1]->elements.b8.d[bi * i] < MIN_INT, NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b4.i[i] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * i], (m_arr[bi << 1]->elements.b8.d[bi * i]), &r.m_err);                      
                }
            break;
            case UINT:
            switch(b.type){
                case CHAR: 
                    cond = m_arr[bi << 1]->elements.b1.i[bi * i] < 0;
                    cond &= int_absolute_value(m_arr[bi << 1]->elements.b1.i[bi * i]) > m_arr[ai]->elements.b4.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.elements.b4.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[ai * i] + (int8_t)(m_arr[bi << 1]->elements.b1.i[bi * i] & (char)-cond), 
                                                            m_arr[bi << 1]->elements.b1.i[bi * i] & (int8_t)-(!cond), &r.m_err);
                break;
                case UCHAR:
                    r.elements.b4.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[ai * i], m_arr[bi << 1]->elements.b1.ui[bi * i], &r.m_err);
                break;
                case INT:
                    cond = m_arr[bi << 1]->elements.b4.i[bi * i] < 0;
                    cond &= int_absolute_value(m_arr[bi << 1]->elements.b4.i[bi * i]) > m_arr[ai]->elements.b4.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b4.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b4.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[ai * i] + (int32_t)(m_arr[bi << 1]->elements.b4.i[bi * i] & (int)-cond), 
                                                            m_arr[bi << 1]->elements.b4.i[bi * i] & (int32_t)-(!cond), &r.m_err);
                break;
                case UINT:
                    r.elements.b1.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[i], m_arr[bi << 1]->elements.b4.ui[i], &r.m_err);
                break;
                case LINT:
                    cond = m_arr[bi << 1]->elements.b8.i[bi * i] < 0;
                    cond &= lint_absolute_value(m_arr[bi << 1]->elements.b8.i[bi * i]) > m_arr[ai]->elements.b4.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b8.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b4.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[ai * i] + (int64_t)(m_arr[bi << 1]->elements.b8.i[bi * i] & (long int)-cond),
                                                            m_arr[bi << 1]->elements.b8.i[bi * i] & (int64_t)-(!cond), &r.m_err);
                break;
                case LUINT:
                    r.m_err = ternary(m_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b4.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[i], m_arr[bi << 1]->elements.b8.ui[i], &r.m_err);
                break;
                case FLOAT:
                    cond = m_arr[bi << 1]->elements.b4.f[bi * i] < 0;
                    cond &= double_absolute_value((dbits){ .d = m_arr[bi << 1]->elements.b4.f[bi * i] }) > m_arr[ai]->elements.b4.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b4.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[ai * i] + m_arr[bi << 1]->elements.b4.f[bi * i] * cond, 
                                                            m_arr[bi << 1]->elements.b4.f[bi * i] * !cond, &r.m_err);
                break; 
                case DOUBLE:
                    cond = m_arr[bi << 1]->elements.b8.d[bi * i] < 0;
                    cond &= double_absolute_value((dbits){ .d = m_arr[bi]->elements.b8.d[bi * i] }) > m_arr[ai]->elements.b4.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary((double)m_arr[bi << 1]->elements.b8.d[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b4.ui[i] = safe_uint_addition(m_arr[ai]->elements.b4.ui[ai * i] + m_arr[bi << 1]->elements.b8.d[bi * i] * cond, 
                                                            m_arr[bi << 1]->elements.b8.d[bi * i] * !cond, &r.m_err);
            }
        break;
        case LINT:
            switch(b.type){
                case CHAR: 
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], m_arr[bi << 1]->elements.b1.i[bi * i], &r.m_err);
                break;
                case UCHAR:
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], m_arr[bi << 1]->elements.b1.ui[bi * i], &r.m_err);
                break;
                case INT:
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], m_arr[bi << 1]->elements.b4.i[bi * i], &r.m_err);                      
                break;
                case UINT:
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], m_arr[bi << 1]->elements.b4.ui[bi * i], &r.m_err);
                    break;
                case LINT:
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], m_arr[bi << 1]->elements.b8.i[bi * i], &r.m_err);                      
                break;
                case LUINT:
                    r.m_err = ternary(m_arr[bi << 1]->elements.b8.ui[bi * i] > MAX_LINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], m_arr[bi << 1]->elements.b8.ui[bi * i], &r.m_err);
                break;
                case FLOAT:
                    r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] > MAX_LINT, POSITIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] < MIN_LINT, NEGATIVE_OVERFLOW, r.m_err);
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], (m_arr[bi << 1]->elements.b4.f[bi * i]), &r.m_err);                      
                break; 
                case DOUBLE:
                    r.m_err = ternary(m_arr[bi << 1]->elements.b8.d[bi * i] > MAX_LINT, POSITIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b8.d[bi * i] < MIN_LINT, NEGATIVE_OVERFLOW, r.m_err);
                    r.elements.b8.i[i] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * i], (m_arr[bi << 1]->elements.b8.d[bi * i]), &r.m_err);                      
            }
        break;
        case LUINT:
            switch(b.type){
                case CHAR: 
                    cond = m_arr[bi << 1]->elements.b1.i[bi * i] < 0;
                    cond &= int_absolute_value(m_arr[bi << 1]->elements.b1.i[bi * i]) > m_arr[ai]->elements.b8.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.elements.b8.ui[i] = safe_luint_addition(m_arr[ai]->elements.b8.ui[ai * i] + (int8_t)(m_arr[bi << 1]->elements.b1.i[bi * i] & (char)-cond), 
                                                            m_arr[bi << 1]->elements.b1.i[bi * i] & (int8_t)-(!cond), &r.m_err);
                break;
                case UCHAR:
                    r.elements.b8.ui[i] = safe_luint_addition(m_arr[ai]->elements.b4.ui[ai * i], m_arr[bi << 1]->elements.b1.ui[bi * i], &r.m_err);
                break;
                case INT:
                    cond = m_arr[bi << 1]->elements.b4.i[bi * i] < 0;
                    cond &= int_absolute_value(m_arr[bi << 1]->elements.b4.i[bi * i]) > m_arr[ai]->elements.b8.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b4.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b8.ui[i] = safe_luint_addition(m_arr[ai]->elements.b8.ui[ai * i] + (int32_t)(m_arr[bi << 1]->elements.b4.i[bi * i] & (int)-cond), 
                                                            m_arr[bi << 1]->elements.b4.i[bi * i] & (int32_t)-(!cond), &r.m_err);
                break;
                case UINT:
                    r.elements.b1.ui[i] = safe_luint_addition(m_arr[ai]->elements.b4.ui[i], m_arr[bi << 1]->elements.b4.ui[i], &r.m_err);
                break;
                case LINT:
                    cond = m_arr[bi << 1]->elements.b8.i[bi * i] < 0;
                    cond &= lint_absolute_value(m_arr[bi << 1]->elements.b8.i[bi * i]) > m_arr[ai]->elements.b8.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b8.i[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b8.ui[i] = safe_luint_addition(m_arr[ai]->elements.b8.ui[ai * i] + (int64_t)(m_arr[bi << 1]->elements.b8.i[bi * i] & (long int)-cond),
                                                            m_arr[bi << 1]->elements.b8.i[bi * i] & (int64_t)-(!cond), &r.m_err);
                break;
                case LUINT:
                    r.elements.b8.ui[i] = safe_luint_addition(m_arr[ai]->elements.b4.ui[i], m_arr[bi << 1]->elements.b8.ui[i], &r.m_err);
                break;
                case FLOAT:
                    cond = m_arr[bi << 1]->elements.b4.f[bi * i] < 0;
                    cond &= double_absolute_value((dbits){ .d = m_arr[bi << 1]->elements.b4.f[bi * i] }) > m_arr[ai]->elements.b8.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary(m_arr[bi << 1]->elements.b4.f[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b8.ui[i] = safe_luint_addition(m_arr[ai]->elements.b8.ui[ai * i] + m_arr[bi << 1]->elements.b4.f[bi * i] * cond, 
                                                            m_arr[bi << 1]->elements.b4.f[bi * i] * !cond, &r.m_err);
                break; 
                case DOUBLE:
                    cond = m_arr[bi << 1]->elements.b8.d[bi * i] < 0;
                    cond &= double_absolute_value((dbits){ .d = m_arr[bi]->elements.b8.d[bi * i] }) > m_arr[ai]->elements.b8.ui[ai * i];
                    r.m_err = ternary(cond, NEGATIVE_OVERFLOW, r.m_err);
                    r.m_err = ternary((double)m_arr[bi << 1]->elements.b8.d[bi * i] > MAX_UINT, POSITIVE_OVERFLOW, r.m_err);
                    r.elements.b8.ui[i] = safe_luint_addition(m_arr[ai]->elements.b8.ui[ai * i] + m_arr[bi << 1]->elements.b8.d[bi * i] * cond, 
                                                            m_arr[bi << 1]->elements.b8.d[bi * i] * !cond, &r.m_err);
            }
        break;
            case FLOAT: 
            switch(b.type){
                case CHAR: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b1.i[i] }, &r.m_err);
                break;
                case UCHAR: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b1.ui[i] }, &r.m_err);
                break;
                case INT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b4.i[i] }, &r.m_err);
                break;
                case UINT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b4.ui[i] }, &r.m_err);
                break;
                case LINT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b8.i[i] }, &r.m_err);
                break;
                case LUINT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b8.ui[i] }, &r.m_err);
                break;
                case FLOAT: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b4.f[i] }, &r.m_err);
                break;
                case DOUBLE: r.elements.b4.f[i] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[i] }, (fbits){ .f = m_arr[bi << 1]->elements.b8.d[i] }, &r.m_err);
            }
        break;
        case DOUBLE:
            switch(b.type){
                case CHAR: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b1.i[i] }, &r.m_err);
                break;
                case UCHAR: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b1.ui[i] }, &r.m_err);
                break;
                case INT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b4.i[i] }, &r.m_err);
                break;
                case UINT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b4.ui[i] }, &r.m_err);
                break;
                case LINT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b8.i[i] }, &r.m_err);
                break;
                case LUINT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b8.ui[i] }, &r.m_err);
                break;
                case FLOAT: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b4.f[i] }, &r.m_err);
                break;
                case DOUBLE: r.elements.b8.d[i] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[i] }, (dbits){ .d = m_arr[bi << 1]->elements.b8.d[i] }, &r.m_err);
            }
        break;
        default: return r;            
    }
    }
    return r;
}

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
                        r.elements.b8.i[a.n] = safe_lint_multiplication(a.elements.b8.i[a.n], b.elements.b8.i[a.n], &r.v_error);
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

matrix_t matrix_multiplication_elementwise(matrix_t a, matrix_t b){
    if((a.row ^ b.row) | (a.col ^ b.col)){ return (matrix_t){a.type, a.row, a.col, {0}, NO_ERROR}; }
    matrix_t r = {a.type, a.row,  a.col, malloc(amount_of_type_bytes(a.type) * a.col * a.row), NO_ERROR};
    uint32_t i = r.row * r.col;
    int8_constr int8_p;
    int32_constr int32_p;
    int64_constr int64_p;
    fbits f32_p;
    dbits d64_p;
    while(i--){
        switch(a.type){
            case CHAR: 
                switch(b.type){
                    case CHAR: 
                        r.elements.b1.i[i] = safe_char_multiplication(a.elements.b1.i[i], b.elements.b1.i[i], &r.m_err);
                    break;
                    case UCHAR:
                        r.m_err = ternary(a.elements.b1.i[i] && b.elements.b1.ui[i] > MAX_CHAR, ternary(a.elements.b1.i[i] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b1.i[i] = safe_char_multiplication(a.elements.b1.i[i], b.elements.b1.ui[i], &r.m_err);
                    break;
                    case INT:
                        int8_p.val = a.elements.b1.i[i];
                        int32_p.val = b.elements.b4.i[i];
                        r.m_err = ternary(int8_p.val && (int32_p.val > MAX_CHAR), ternary(int8_p.parts.sign ^ int32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int8_p.val && (int32_p.val < MIN_CHAR), ternary(int8_p.parts.sign ^ int32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b1.i[i] = safe_char_multiplication(int8_p.val, int32_p.val, &r.m_err);
                    break;
                    case UINT:
                        r.m_err = ternary(a.elements.b1.i[i] && b.elements.b4.ui[i] > MAX_CHAR, ternary(a.elements.b1.i[i] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b1.i[i] = safe_char_multiplication(a.elements.b1.i[i], b.elements.b4.ui[i], &r.m_err);                    
                    break;
                    case LINT:
                        int8_p.val = a.elements.b1.i[i];
                        int64_p.val = b.elements.b8.i[i];
                        r.m_err = ternary(int8_p.val && (int64_p.val > MAX_CHAR), ternary(int8_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int8_p.val && (int64_p.val < MIN_CHAR), ternary(int8_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b1.i[i] = safe_char_multiplication(int8_p.val, int64_p.val, &r.m_err);
                    break;
                    case LUINT:
                        r.m_err = ternary(a.elements.b1.i[i] && b.elements.b8.ui[i] > MAX_CHAR, ternary(a.elements.b1.i[i] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b1.i[i] = safe_char_multiplication(a.elements.b1.i[i], b.elements.b8.ui[i], &r.m_err);
                    break;
                    case FLOAT:
                        int8_p.val = a.elements.b1.i[i];
                        f32_p.f = b.elements.b4.f[i];
                        r.m_err = ternary(int8_p.val && (f32_p.f > MAX_CHAR), ternary(int8_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int8_p.val && (f32_p.f < MIN_CHAR), ternary(int8_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b1.i[i] = safe_char_multiplication(int8_p.val, f32_p.f, &r.m_err);
                    break;
                    case DOUBLE:
                        int8_p.val = a.elements.b1.i[i];
                        d64_p.d = b.elements.b8.d[i];
                        r.m_err = ternary(int8_p.val && (d64_p.d > MAX_CHAR), ternary(int8_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int8_p.val && (d64_p.d < MIN_CHAR), ternary(int8_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b1.i[i] = safe_char_multiplication(int8_p.val, d64_p.d, &r.m_err);
                }
            break;
            case UCHAR:
                switch(b.type){
                    case CHAR: 
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b1.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b1.i[i], &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b1.ui[i], &r.m_err);                        
                    break;
                    case INT:
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b4.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b4.i[i] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b4.i[i], &r.m_err);
                    break;
                    case UINT:
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b4.ui[i] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b4.ui[i], &r.m_err);                        
                    break;
                    case LINT:
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b8.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b8.i[i] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b8.i[i], &r.m_err);
                    break;
                    case LUINT:
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b8.ui[i] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b8.ui[i], &r.m_err);                        
                    break;
                    case FLOAT:
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b4.f[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b4.f[i] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b4.f[i], &r.m_err);
                    break;
                    case DOUBLE:
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b8.d[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b1.ui[i] && (b.elements.b8.d[i] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b1.ui[i] = safe_uchar_multiplication(a.elements.b1.ui[i], b.elements.b8.d[i], &r.m_err);
                }
            break;
            case INT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b4.i[i] = safe_int_multiplication(a.elements.b4.i[i], b.elements.b1.i[i], &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b4.i[i] = safe_int_multiplication(a.elements.b4.i[i], b.elements.b1.ui[i], &r.m_err);
                    break;
                    case INT:
                        r.elements.b4.i[i] = safe_int_multiplication(a.elements.b4.i[i], b.elements.b4.i[i], &r.m_err);
                    break;
                    case UINT:
                        r.m_err = ternary(a.elements.b4.i[i] && b.elements.b4.ui[i] > MAX_CHAR, ternary(a.elements.b4.i[i] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b4.i[i] = safe_int_multiplication(a.elements.b4.i[i], b.elements.b4.ui[i], &r.m_err);                    
                    break;
                    case LINT:
                        int32_p.val = a.elements.b4.i[i];
                        int64_p.val = b.elements.b8.i[i];
                        r.m_err = ternary(int32_p.val && (int64_p.val > MAX_INT), ternary(int32_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int32_p.val && (int64_p.val < MIN_INT), ternary(int32_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b4.i[i] = safe_int_multiplication(int32_p.val, int64_p.val, &r.m_err);
                    break;
                    case LUINT:
                        r.m_err = ternary(a.elements.b4.i[i] && b.elements.b8.ui[i] > MAX_INT, ternary(a.elements.b4.i[i] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b4.i[i] = safe_int_multiplication(a.elements.b4.i[i], b.elements.b8.ui[i], &r.m_err);
                    break;
                    case FLOAT:
                        int32_p.val = a.elements.b4.i[i];
                        f32_p.f = b.elements.b4.f[i];
                        r.m_err = ternary(int32_p.val && (f32_p.f > MAX_INT), ternary(int32_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int32_p.val && (f32_p.f < MIN_INT), ternary(int32_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b4.i[i] = safe_int_multiplication(int32_p.val, f32_p.f, &r.m_err);
                    break;
                    case DOUBLE:
                        int32_p.val = a.elements.b4.i[i];
                        d64_p.d = b.elements.b8.d[i];
                        r.m_err = ternary(int32_p.val && (d64_p.d > MAX_INT), ternary(int32_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int32_p.val && (d64_p.d < MIN_INT), ternary(int32_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b4.i[i] = safe_int_multiplication(int32_p.val, d64_p.d, &r.m_err);
                }
            break;
            case UINT:
                switch(b.type){
                    case CHAR: 
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b4.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b1.i[i], &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b1.ui[i], &r.m_err);                        
                    break;
                    case INT:
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b4.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b4.i[i], &r.m_err);
                    break;
                    case UINT:
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b4.ui[i], &r.m_err);                        
                    break;
                    case LINT:
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b8.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b8.i[i] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b8.i[i], &r.m_err);
                    break;
                    case LUINT:
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b8.ui[i] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b8.ui[i], &r.m_err);                        
                    break;
                    case FLOAT:
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b4.f[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b4.f[i] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b4.f[i], &r.m_err);
                    break;
                    case DOUBLE:
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b8.d[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b8.d[i] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b4.ui[i] = safe_uint_multiplication(a.elements.b4.ui[i], b.elements.b8.d[i], &r.m_err);
                }
            break; 
            case LINT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b8.i[i] = safe_lint_multiplication(a.elements.b8.i[i], b.elements.b1.i[i], &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b8.i[i] = safe_lint_multiplication(a.elements.b8.i[i], b.elements.b1.ui[i], &r.m_err);
                    break;
                    case INT:
                        r.elements.b8.i[i] = safe_lint_multiplication(a.elements.b8.i[i], b.elements.b4.i[i], &r.m_err);
                    break;
                    case UINT:
                        r.elements.b8.i[i] = safe_lint_multiplication(a.elements.b8.i[i], b.elements.b4.ui[i], &r.m_err);                    
                    break;
                    case LINT:
                        r.elements.b8.i[i] = safe_lint_multiplication(a.elements.b8.i[i], b.elements.b8.i[i], &r.m_err);
                    break;
                    case LUINT:
                        r.m_err = ternary(a.elements.b8.i[i] && b.elements.b8.ui[i] > MAX_LINT, ternary(a.elements.b8.i[i] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b8.i[i] = safe_lint_multiplication(a.elements.b8.i[i], b.elements.b8.ui[i], &r.m_err);
                    break;
                    case FLOAT:
                        int64_p.val = a.elements.b8.i[i];
                        f32_p.f = b.elements.b4.f[i];
                        r.m_err = ternary(int64_p.val && (f32_p.f > MAX_LINT), ternary(int64_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int64_p.val && (f32_p.f < MIN_LINT), ternary(int64_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b8.i[i] = safe_lint_multiplication(int64_p.val, f32_p.f, &r.m_err);
                    break;
                    case DOUBLE:
                        int64_p.val = a.elements.b8.i[i];
                        d64_p.d = b.elements.b8.d[i];
                        r.m_err = ternary(int64_p.val && (d64_p.d > MAX_LINT), ternary(int64_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.m_err = ternary(int64_p.val && (d64_p.d < MIN_LINT), ternary(int64_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                        r.elements.b8.i[i] = safe_lint_multiplication(int64_p.val, d64_p.d, &r.m_err);
                }
            break;
            case LUINT:
                switch(b.type){
                    case CHAR: 
                        r.m_err = ternary(a.elements.b4.ui[i] && (b.elements.b4.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b8.ui[i], b.elements.b1.i[i], &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b8.ui[i], b.elements.b1.ui[i], &r.m_err);                        
                    break;
                    case INT:
                        r.m_err = ternary(a.elements.b8.ui[i] && (b.elements.b4.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b8.ui[i], b.elements.b4.i[i], &r.m_err);
                    break;
                    case UINT:
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b8.ui[i], b.elements.b4.ui[i], &r.m_err);                        
                    break;
                    case LINT:
                        r.m_err = ternary(a.elements.b8.ui[i] && (b.elements.b8.i[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b8.ui[i], b.elements.b8.i[i], &r.m_err);
                    break;
                    case LUINT:
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b8.ui[i], b.elements.b8.ui[i], &r.m_err);                        
                    break;
                    case FLOAT:
                        r.m_err = ternary(a.elements.b8.ui[i] && (b.elements.b4.f[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b8.ui[i] && (b.elements.b4.f[i] > MAX_LUINT), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b8.ui[i], b.elements.b4.f[i], &r.m_err);
                    break;
                    case DOUBLE:
                        r.m_err = ternary(a.elements.b8.ui[i] && (b.elements.b8.d[i] < 0), NEGATIVE_OVERFLOW, r.m_err);
                        r.m_err = ternary(a.elements.b8.ui[i] && (b.elements.b8.d[i] > MAX_LUINT), POSITIVE_OVERFLOW, r.m_err);
                        r.elements.b8.ui[i] = safe_luint_multiplication(a.elements.b4.ui[i], b.elements.b8.d[i], &r.m_err);
                }
            break;
            case FLOAT:
                switch(b.type){
                    case CHAR: 
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b1.i[i] }, &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b1.ui[i] }, &r.m_err);
                    break;
                    case INT:
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b4.i[i] }, &r.m_err);
                    break;
                    case UINT:
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b4.ui[i] }, &r.m_err);
                    break;
                    case LINT:
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b8.i[i] }, &r.m_err);
                    break;
                    case LUINT:
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b8.ui[i] }, &r.m_err);
                    break;
                    case FLOAT:
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b4.f[i] }, &r.m_err);
                    break;
                    case DOUBLE:
                        r.elements.b4.f[i] = safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[i] }, (fbits){ .f = b.elements.b8.d[i] }, &r.m_err);
                }
            break;
            case DOUBLE:
                switch(b.type){
                    case CHAR: 
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b1.i[i] }, &r.m_err);
                    break;
                    case UCHAR:
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b1.ui[i] }, &r.m_err);
                    break;
                    case INT:
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b4.i[i] }, &r.m_err);
                    break;
                    case UINT:
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b4.ui[i] }, &r.m_err);
                    break;
                    case LINT:
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b8.i[i] }, &r.m_err);
                    break;
                    case LUINT:
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b8.ui[i] }, &r.m_err);
                    break;
                    case FLOAT:
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b4.f[i] }, &r.m_err);
                    break;
                    case DOUBLE:
                        r.elements.b8.d[i] = safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[i] }, (dbits){ .d = b.elements.b8.d[i] }, &r.m_err);
                }
            break;
            default: return r;
        }
    }
    return r;
}

matrix_t matrix_multiplication(matrix_t a, matrix_t b){
    if(a.col ^ b.row){ return (matrix_t){a.type, a.row, b.col, { 0 }, UNDEFINED_BEHAVIOR}; }
    matrix_t r = {a.type, a.row, b.col, malloc(amount_of_type_bytes(a.type) * a.row * b.col), NO_ERROR};
    uint32_t first;
    int8_constr int8_p;
    int32_constr int32_p;
    int64_constr int64_p;
    fbits f32_p;
    dbits d64_p;
    while(b.col--){
       first = 1;
       a.row = r.row;
       while(a.row--){
          a.col = b.row;
          switch(r.type & -first){
                case CHAR:   r.elements.b1.i [a.row * r.col + b.col] = 0; first = 0; break;
                case UCHAR:  r.elements.b1.ui[a.row * r.col + b.col] = 0; first = 0; break;
                case INT:    r.elements.b4.i [a.row * r.col + b.col] = 0; first = 0; break;
                case UINT:   r.elements.b4.ui[a.row * r.col + b.col] = 0; first = 0; break;
                case LINT:   r.elements.b8.i [a.row * r.col + b.col] = 0; first = 0; break;
                case LUINT:  r.elements.b8.ui[a.row * r.col + b.col] = 0; first = 0; break;
                case FLOAT:  r.elements.b4.f [a.row * r.col + b.col] = 0; first = 0; break;
                case DOUBLE: r.elements.b8.d [a.row * r.col + b.col] = 0; first = 0; 
          }
          while(a.col--){
             switch(a.type){
                   case CHAR: 
                      switch(b.type){
                         case CHAR: 
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(a.elements.b1.i[a.row * b.row + a.col], b.elements.b1.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UCHAR:
                               r.m_err = ternary(a.elements.b1.i[a.row * b.row + a.col] && b.elements.b1.ui[a.col * r.col + b.col] > MAX_CHAR, ternary(a.elements.b1.i[a.row * b.row + a.col] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(a.elements.b1.i[a.row * b.row + a.col], b.elements.b1.ui[a.col * r.col + b.col], &r.m_err);
                         break;
                         case INT:
                               int8_p.val = a.elements.b1.i[a.row * b.row + a.col];
                               int32_p.val = b.elements.b4.i[a.col * r.col + b.col];
                               r.m_err = ternary(int8_p.val && (int32_p.val > MAX_CHAR), ternary(int8_p.parts.sign ^ int32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int8_p.val && (int32_p.val < MIN_CHAR), ternary(int8_p.parts.sign ^ int32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(int8_p.val, int32_p.val, &r.m_err);
                         break;
                         case UINT:
                               r.m_err = ternary(a.elements.b1.i[a.row * b.row + a.col] && b.elements.b4.ui[a.col * r.col + b.col] > MAX_CHAR, ternary(a.elements.b1.i[a.row * b.row + a.col] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(a.elements.b1.i[a.row * b.row + a.col], b.elements.b4.ui[a.col * r.col + b.col], &r.m_err);                    
                         break;
                         case LINT:
                               int8_p.val = a.elements.b1.i[a.row * b.row + a.col];
                               int64_p.val = b.elements.b8.i[a.col * r.col + b.col];
                               r.m_err = ternary(int8_p.val && (int64_p.val > MAX_CHAR), ternary(int8_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int8_p.val && (int64_p.val < MIN_CHAR), ternary(int8_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(int8_p.val, int64_p.val, &r.m_err);
                         break;
                         case LUINT:
                               r.m_err = ternary(a.elements.b1.i[a.row * b.row + a.col] && b.elements.b8.ui[a.col * r.col + b.col] > MAX_CHAR, ternary(a.elements.b1.i[a.row * b.row + a.col] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(a.elements.b1.i[a.row * b.row + a.col], b.elements.b8.ui[a.col * r.col + b.col], &r.m_err);
                         break;
                         case FLOAT:
                               int8_p.val = a.elements.b1.i[a.row * b.row + a.col];
                               f32_p.f = b.elements.b4.f[a.col * r.col + b.col];
                               r.m_err = ternary(int8_p.val && (f32_p.f > MAX_CHAR), ternary(int8_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int8_p.val && (f32_p.f < MIN_CHAR), ternary(int8_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(int8_p.val, f32_p.f, &r.m_err);
                         break;
                         case DOUBLE:
                               int8_p.val = a.elements.b1.i[a.row * b.row + a.col];
                               d64_p.d = b.elements.b8.d[a.col * r.col + b.col];
                               r.m_err = ternary(int8_p.val && (d64_p.d > MAX_CHAR), ternary(int8_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int8_p.val && (d64_p.d < MIN_CHAR), ternary(int8_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b1.i[a.row * r.col + b.col] += safe_char_multiplication(int8_p.val, d64_p.d, &r.m_err);
                      }
                   break;
                   case UCHAR:
                      switch(b.type){
                         case CHAR: 
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b1.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b1.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UCHAR:
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b1.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case INT:
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b4.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b4.i[a.col * r.col + b.col] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b4.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UINT:
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b4.ui[a.col * r.col + b.col] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b4.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case LINT:
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b8.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b8.i[a.col * r.col + b.col] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b8.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case LUINT:
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b8.ui[a.col * r.col + b.col] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b8.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case FLOAT:
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b4.f[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b4.f[a.col * r.col + b.col] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b4.f[a.col * r.col + b.col], &r.m_err);
                         break;
                         case DOUBLE:
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b8.d[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b1.ui[a.row * b.row + a.col] && (b.elements.b8.d[a.col * r.col + b.col] > MAX_UCHAR), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b1.ui[a.row * r.col + b.col] += safe_uchar_multiplication(a.elements.b1.ui[a.row * b.row + a.col], b.elements.b8.d[a.col * r.col + b.col], &r.m_err);
                      }
                   break;
                   case INT:
                      switch(b.type){
                         case CHAR: 
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(a.elements.b4.i[a.row * b.row + a.col], b.elements.b1.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UCHAR:
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(a.elements.b4.i[a.row * b.row + a.col], b.elements.b1.ui[a.col * r.col + b.col], &r.m_err);
                         break;
                         case INT:
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(a.elements.b4.i[a.row * b.row + a.col], b.elements.b4.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UINT:
                               r.m_err = ternary(a.elements.b4.i[a.row * b.row + a.col] && b.elements.b4.ui[a.col * r.col + b.col] > MAX_CHAR, ternary(a.elements.b4.i[a.row * b.row + a.col] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(a.elements.b4.i[a.row * b.row + a.col], b.elements.b4.ui[a.col * r.col + b.col], &r.m_err);                    
                         break;
                         case LINT:
                               int32_p.val = a.elements.b4.i[a.row * b.row + a.col];
                               int64_p.val = b.elements.b8.i[a.col * r.col + b.col];
                               r.m_err = ternary(int32_p.val && (int64_p.val > MAX_INT), ternary(int32_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int32_p.val && (int64_p.val < MIN_INT), ternary(int32_p.parts.sign ^ int64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(int32_p.val, int64_p.val, &r.m_err);
                         break;
                         case LUINT:
                               r.m_err = ternary(a.elements.b4.i[a.row * b.row + a.col] && b.elements.b8.ui[a.col * r.col + b.col] > MAX_INT, ternary(a.elements.b4.i[a.row * b.row + a.col] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(a.elements.b4.i[a.row * b.row + a.col], b.elements.b8.ui[a.col * r.col + b.col], &r.m_err);
                         break;
                         case FLOAT:
                               int32_p.val = a.elements.b4.i[a.row * b.row + a.col];
                               f32_p.f = b.elements.b4.f[a.col * r.col + b.col];
                               r.m_err = ternary(int32_p.val && (f32_p.f > MAX_INT), ternary(int32_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int32_p.val && (f32_p.f < MIN_INT), ternary(int32_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(int32_p.val, f32_p.f, &r.m_err);
                         break;
                         case DOUBLE:
                               int32_p.val = a.elements.b4.i[a.row * b.row + a.col];
                               d64_p.d = b.elements.b8.d[a.col * r.col + b.col];
                               r.m_err = ternary(int32_p.val && (d64_p.d > MAX_INT), ternary(int32_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int32_p.val && (d64_p.d < MIN_INT), ternary(int32_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b4.i[a.row * r.col + b.col] += safe_int_multiplication(int32_p.val, d64_p.d, &r.m_err);
                      }
                   break;
                   case UINT:
                      switch(b.type){
                         case CHAR: 
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b4.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b1.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UCHAR:
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b1.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case INT:
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b4.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b4.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UINT:
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b4.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case LINT:
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b8.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b8.i[a.col * r.col + b.col] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b8.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case LUINT:
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b8.ui[a.col * r.col + b.col] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b8.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case FLOAT:
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b4.f[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b4.f[a.col * r.col + b.col] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b4.f[a.col * r.col + b.col], &r.m_err);
                         break;
                         case DOUBLE:
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b8.d[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b8.d[a.col * r.col + b.col] > MAX_UINT), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b4.ui[a.row * r.col + b.col] += safe_uint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b8.d[a.col * r.col + b.col], &r.m_err);
                      }
                   break; 
                   case LINT:
                      switch(b.type){
                         case CHAR: 
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(a.elements.b8.i[a.row * b.row + a.col], b.elements.b1.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UCHAR:
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(a.elements.b8.i[a.row * b.row + a.col], b.elements.b1.ui[a.col * r.col + b.col], &r.m_err);
                         break;
                         case INT:
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(a.elements.b8.i[a.row * b.row + a.col], b.elements.b4.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UINT:
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(a.elements.b8.i[a.row * b.row + a.col], b.elements.b4.ui[a.col * r.col + b.col], &r.m_err);                    
                         break;
                         case LINT:
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(a.elements.b8.i[a.row * b.row + a.col], b.elements.b8.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case LUINT:
                               r.m_err = ternary(a.elements.b8.i[a.row * b.row + a.col] && b.elements.b8.ui[a.col * r.col + b.col] > MAX_LINT, ternary(a.elements.b8.i[a.row * b.row + a.col] > 0, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(a.elements.b8.i[a.row * b.row + a.col], b.elements.b8.ui[a.col * r.col + b.col], &r.m_err);
                         break;
                         case FLOAT:
                               int64_p.val = a.elements.b8.i[a.row * b.row + a.col];
                               f32_p.f = b.elements.b4.f[a.col * r.col + b.col];
                               r.m_err = ternary(int64_p.val && (f32_p.f > MAX_LINT), ternary(int64_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int64_p.val && (f32_p.f < MIN_LINT), ternary(int64_p.parts.sign ^ f32_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(int64_p.val, f32_p.f, &r.m_err);
                         break;
                         case DOUBLE:
                               int64_p.val = a.elements.b8.i[a.row * b.row + a.col];
                               d64_p.d = b.elements.b8.d[a.col * r.col + b.col];
                               r.m_err = ternary(int64_p.val && (d64_p.d > MAX_LINT), ternary(int64_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.m_err = ternary(int64_p.val && (d64_p.d < MIN_LINT), ternary(int64_p.parts.sign ^ d64_p.parts.sign, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), r.m_err);
                               r.elements.b8.i[a.row * r.col + b.col] += safe_lint_multiplication(int64_p.val, d64_p.d, &r.m_err);
                      }
                   break;
                   case LUINT:
                      switch(b.type){
                         case CHAR: 
                               r.m_err = ternary(a.elements.b4.ui[a.row * b.row + a.col] && (b.elements.b4.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b8.ui[a.row * b.row + a.col], b.elements.b1.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UCHAR:
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b8.ui[a.row * b.row + a.col], b.elements.b1.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case INT:
                               r.m_err = ternary(a.elements.b8.ui[a.row * b.row + a.col] && (b.elements.b4.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b8.ui[a.row * b.row + a.col], b.elements.b4.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case UINT:
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b8.ui[a.row * b.row + a.col], b.elements.b4.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case LINT:
                               r.m_err = ternary(a.elements.b8.ui[a.row * b.row + a.col] && (b.elements.b8.i[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b8.ui[a.row * b.row + a.col], b.elements.b8.i[a.col * r.col + b.col], &r.m_err);
                         break;
                         case LUINT:
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b8.ui[a.row * b.row + a.col], b.elements.b8.ui[a.col * r.col + b.col], &r.m_err);                        
                         break;
                         case FLOAT:
                               r.m_err = ternary(a.elements.b8.ui[a.row * b.row + a.col] && (b.elements.b4.f[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b8.ui[a.row * b.row + a.col] && (b.elements.b4.f[a.col * r.col + b.col] > MAX_LUINT), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b8.ui[a.row * b.row + a.col], b.elements.b4.f[a.col * r.col + b.col], &r.m_err);
                         break;
                         case DOUBLE:
                               r.m_err = ternary(a.elements.b8.ui[a.row * b.row + a.col] && (b.elements.b8.d[a.col * r.col + b.col] < 0), NEGATIVE_OVERFLOW, r.m_err);
                               r.m_err = ternary(a.elements.b8.ui[a.row * b.row + a.col] && (b.elements.b8.d[a.col * r.col + b.col] > MAX_LUINT), POSITIVE_OVERFLOW, r.m_err);
                               r.elements.b8.ui[a.row * r.col + b.col] += safe_luint_multiplication(a.elements.b4.ui[a.row * b.row + a.col], b.elements.b8.d[a.col * r.col + b.col], &r.m_err);
                      }
                   break;
                   case FLOAT:
                      switch(b.type){
                         case CHAR: 
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b1.i[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case UCHAR:
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b1.ui[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case INT:
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b4.i[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case UINT:
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b4.ui[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case LINT:
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b8.i[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case LUINT:
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b8.ui[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case FLOAT:
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b4.f[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case DOUBLE:
                               r.elements.b4.f[a.row * r.col + b.col] += safe_float_multiplication_with_rounding((fbits){ .f = a.elements.b4.f[a.row * b.row + a.col] }, (fbits){ .f = b.elements.b8.d[a.col * r.col + b.col] }, &r.m_err);
                      }
                   break;
                   case DOUBLE:
                      switch(b.type){
                         case CHAR: 
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b1.i[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case UCHAR:
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b1.ui[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case INT:
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b4.i[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case UINT:
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b4.ui[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case LINT:
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b8.i[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case LUINT:
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b8.ui[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case FLOAT:
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b4.f[a.col * r.col + b.col] }, &r.m_err);
                         break;
                         case DOUBLE:
                               r.elements.b8.d[a.row * r.col + b.col] += safe_double_multiplication_with_rounding((dbits){ .d = a.elements.b8.d[a.row * b.row + a.col] }, (dbits){ .d = b.elements.b8.d[a.col * r.col + b.col] }, &r.m_err);
                      }
                   break;
                   default: return r;
             }          
          }
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

uint8_t int_uint_float_t(datatype type){
    switch(type){
        case CHAR:
        case INT:
        case LINT: return 0;
        case UCHAR:
        case UINT:
        case LUINT: return 1;
        case FLOAT:
        case DOUBLE: return 2;
    }
}


void* int_n_type_to_int_k_type(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* uint_n_type_to_int_k_type(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* float_n_type_to_int_k_type(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* int_n_type_to_uint_k_type(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* uint_n_type_to_uint_k_type(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* float_n_type_to_uint_k_type(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* uint_to_uiint_n_type_to_float_k_typent(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* uint_n_type_to_float_k_type(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }
void* uint_tofloat_n_type_to_float_k_type_uint(int8_t* from_ptr, void* to_ptr, uint32_t from_s, uint32_t to_s, error* err){ }


matrix_t matrix_addition(matrix_t a, matrix_t b){
    matrix_t *m_arr[3] = { &(matrix_t){ a.type, 1, 1, B8type_i_elements((int64_t[]){ 0 }), NO_ERROR }, &a, &b }, 
                r = { a.type, ternary(a.row > b.row, a.row, b.row), ternary(a.col > b.col, a.col, b.col), malloc( r.col * r.row * amount_of_type_bytes(a.type) ), NO_ERROR };
    uint32_t row = r.row, col, ai, bi;
    datapointer data_ptr = { .ptr = malloc(8) };
    while(row--){
        col = r.col;
        while(col--){
            ai = (row < a.row) & (col < a.col);
            bi = (row < b.row) & (col < b.col);
            switch(int_uint_float_t(a.type)){
                case 0:
                    switch(int_uint_float_t(b.type)){
                        case 0: int_n_type_to_int_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                        case 1: uint_n_type_to_int_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                        case 2: float_n_type_to_int_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                    }
                break;
                case 1:
                    switch(int_uint_float_t(b.type)){
                        case 0: int_n_type_to_uint_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                        case 1: uint_n_type_to_uint_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                        case 2: float_n_type_to_uint_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                    }
                break;
                case 2:
                    switch(int_uint_float_t(b.type)){
                        case 0: int_n_type_to_float_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                        case 1: uint_n_type_to_float_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                        case 2: float_n_type_to_float_k_type( m_arr[bi << 1]->elements.b1.i + (bi * (row * r.col + col)), data_ptr.ptr, amount_of_type_bytes(a.type), amount_of_type_bytes(b.type), &r.m_err ); break;
                    }
            }
            switch(a.type){
                case CHAR: r.elements.b1.i[row * r.col + col] = safe_char_addition(m_arr[ai]->elements.b1.i[ai * (row * r.col + col)], *data_ptr.i8, &r.m_err); break;
                case UCHAR: r.elements.b1.ui[row * r.col + col] = safe_uchar_addition(m_arr[ai]->elements.b1.ui[ai * (row * r.col + col)], *data_ptr.ui8, &r.m_err); break;
                case INT: r.elements.b4.i[row * r.col + col] = safe_int_addition(m_arr[ai]->elements.b4.i[ai * (row * r.col + col)], *data_ptr.i32, &r.m_err); break;
                case UINT: r.elements.b4.ui[row * r.col + col] = safe_uint_addition(m_arr[ai]->elements.b4.ui[ai * (row * r.col + col)], *data_ptr.ui32, &r.m_err); break;
                case LINT: r.elements.b8.i[row * r.col + col] = safe_lint_addition(m_arr[ai]->elements.b8.i[ai * (row * r.col + col)], *data_ptr.i64, &r.m_err); break;
                case LUINT: r.elements.b8.ui[row * r.col + col] = safe_luint_addition(m_arr[ai]->elements.b8.ui[ai * (row * r.col + col)], *data_ptr.ui64, &r.m_err); break;
                case FLOAT: r.elements.b4.f[row * r.col + col] = safe_float_addition((fbits){ .f = m_arr[ai]->elements.b4.f[ai * (row * r.col + col)] }, (fbits){ .f = *data_ptr.f32 }, &r.m_err); break;
                case DOUBLE: r.elements.b8.d[row * r.col + col] = safe_double_addition((dbits){ .d = m_arr[ai]->elements.b8.d[ai * (row * r.col + col)] }, (dbits){ .d = *data_ptr.f64 }, &r.m_err);
            }
        }
    }
    free(data_ptr.ptr);
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