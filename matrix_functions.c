#ifndef headerfile
    #include <stdio.h> 
    #include <time.h>  
    #include <stdlib.h>
    #include <math.h>  
    #include <stdint.h> 
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "additional_functions.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
    #include "new_vector_functions.c"
#endif

#define MATRIX_MAX_ELEM_BYTE_SIZE MAX_UINT32


matrix_t matrix_create(datatype type, uint32_t row, uint32_t col){
    uint32_t condition = ((row * col * amount_of_type_bytes(type)) <= MATRIX_MAX_ELEM_BYTE_SIZE);
    row &= -condition;
    void* r = malloc(sizeof(datatype) 
                + (sizeof(uint32_t) << 1) 
                + sizeof(error) 
                + (row * col * amount_of_type_bytes(type)) );

    matrix_t result;
        result.type     = (datatype*)   r;
        result.row      = (uint32_t*)  (result.type + 1);
        result.col      = (uint32_t*)  (result.row  + 1);
        result.err      = (error*)     (result.col  + 1);
        result.elements = (datapointer)(result.err  + 1);

    result.type[0] = type;
    result.row [0] = row;
    result.col [0] = col;
    result.err [0] = ternary(condition, NO_ERROR, MEMORY_LIMIT_EXCESS);
    return result;
}


matrix_t matrix_filler(matrix_t m, datapointer arr){// TODO: what if amount of elements in data is less than n
    if(!(m.type && m.row && m.col && m.err && m.elements.i32 && arr.i32)){ 
        if(m.err){ m.err[0] = NULL_POINTER; }
        return m;
    }
    uint32_t i = m.row[0] * m.col[0] * amount_of_type_bytes(m.type[0]);
    while(i--){ m.elements.i8[i] = arr.i8[i]; }
    return m;
}


matrix_t matrix_scaler_in_place(matrix_t m, datatype scl_type, fundtypeunion scale){// TODO: what if amount of elements in data is less than n
    if(m.row && m.col){ 
        if(m.err){ m.err[0] = NULL_POINTER; }
        return m; 
    }
    uint32_t cond = m.err[0], n = uint32_mult(m.row[0], m.col[0], m.err);
    m.err[0] = ternary(!cond && m.err[0], MEMORY_LIMIT_EXCESS, m.err[0]);
    vecN vec = { .type = m.type, .err = m.err, .elements = m.elements, .n = &n };
    return m;
}


matrix_t matrix_add_first_arg_t(matrix_t a, matrix_t b){// TODO: what if amount of elements in data is less than n
    if(!(a.col && a.row && a.elements.i32 && a.err && a.type && b.col && b.row && b.elements.i32 && b.err && b.type)){ 
        if(a.err){ a.err[0] = NULL_POINTER; }    
        return a; 
    }
    uint32_t cond1 = a.err[0], cond2 = b.err[0], n1 = uint32_add(a.row[0], a.col[0], a.err), n2 = uint32_mult(b.row[0], b.col[0], b.err);
    a.err[0] = ternary((!cond1 && a.err[0]) | (!cond2 && b.err[0]) | (n1 > MATRIX_MAX_ELEM_BYTE_SIZE) | (n2 > MATRIX_MAX_ELEM_BYTE_SIZE), MEMORY_LIMIT_EXCESS, a.err[0]);
    a.err[0] = ternary(n1 != n2, SIZE_DOES_NOT_MATCH, a.err[0]);
    if((a.err[0] == MEMORY_LIMIT_EXCESS) | (a.err[0] == SIZE_DOES_NOT_MATCH)){ return a; }
    vecN r_vec = vec_add_first_arg_t((vecN){ .type = a.type, .n = &n1, .err = a.err, .elements = a.elements }, (vecN){ .type = b.type, .n = &n2, .err = b.err, .elements = b.elements });
    matrix_t r = { .elements = r_vec.elements, .err = r_vec.err, .type = r_vec.type};
    uint32_t* dims = malloc(8);
    dims[0] = a.row[0];
    dims[1] = a.col[0];
    r.row = dims;
    r.col = dims + 1;
    return r;
}

matrix_t matrix_elementwise_mult_first_arg_t(matrix_t a, matrix_t b){// TODO: what if amount of elements in data is less than n
    if(!(a.col && a.row && a.elements.i32 && a.err && a.type && b.col && b.row && b.elements.i32 && b.err && b.type)){ 
        if(a.err){ a.err[0] = NULL_POINTER; }    
        return a; 
    }
    uint32_t cond1 = a.err[0], cond2 = b.err[0], n1 = uint32_add(a.row[0], a.col[0], a.err), n2 = uint32_mult(b.row[0], b.col[0], b.err);
    a.err[0] = ternary((!cond1 && a.err[0]) | (!cond2 && b.err[0]) | (n1 > MATRIX_MAX_ELEM_BYTE_SIZE) | (n2 > MATRIX_MAX_ELEM_BYTE_SIZE), MEMORY_LIMIT_EXCESS, a.err[0]);
    a.err[0] = ternary(n1 != n2, SIZE_DOES_NOT_MATCH, a.err[0]);
    if((a.err[0] == MEMORY_LIMIT_EXCESS) | (a.err[0] == SIZE_DOES_NOT_MATCH)){ return a; }
    vecN r_vec = vec_mult_first_arg_t((vecN){ .type = a.type, .n = &n1, .err = a.err, .elements = a.elements }, (vecN){ .type = b.type, .n = &n2, .err = b.err, .elements = b.elements });
    matrix_t r = { .elements = r_vec.elements, .err = r_vec.err, .type = r_vec.type};
    uint32_t* dims = malloc(8);
    dims[0] = a.row[0];
    dims[1] = a.col[0];
    r.row = dims;
    r.col = dims + 1;
    return r;
}


matrix_t matrix_mult_first_arg_t(matrix_t a, matrix_t b){// a.elements.i8[n2 * a_col + n3], b.elements.i8[n3 * b_col + n1], r.elements.i8[n2 * b_col + n1]
//error checks
    if(!(a.col && a.row && a.elements.i32 && a.err && a.type && b.col && b.row && b.elements.i32 && b.err && b.type)){ 
        if(a.err){ a.err[0] = NULL_POINTER; }    
        return a; 
    }
    uint32_t cond1 = a.err[0], cond2 = b.err[0], n1 = uint32_add(a.row[0], a.col[0], a.err), n2 = uint32_mult(b.row[0], b.col[0], b.err);
    a.err[0] = ternary((!cond1 && a.err[0]) | (!cond2 && b.err[0]) | (n1 > MATRIX_MAX_ELEM_BYTE_SIZE) | (n2 > MATRIX_MAX_ELEM_BYTE_SIZE), MEMORY_LIMIT_EXCESS, a.err[0]);
    a.err[0] = ternary(a.col[0] != b.row[0], SIZE_DOES_NOT_MATCH, a.err[0]);
    if((a.err[0] == MEMORY_LIMIT_EXCESS) | (a.err[0] == SIZE_DOES_NOT_MATCH)){ return a; }
//function body
    matrix_t r = matrix_create(a.type[0], a.row[0], b.col[0]);
    uint8_t a_size = amount_of_type_bytes(a.type[0]), b_size = amount_of_type_bytes(b.type[0]), a_type = a.type[0];
    cond1 = int_uint_float_t(a.type[0]);
    cond2 = int_uint_float_t(b.type[0]);
    uint32_t b_col = b.col[0], a_row = a.row[0], a_col = a.col[0], n3, a_ind, b_ind;
    fundtypeunion sum;
    datapointer ptr = { .vptr = malloc(8) };
    for(n1 = 0; n1 < b_col; n1++){
        for(n2 = 0; n2 < a_row; n2++){
            for(n3 = sum.ui64 = 0; n3 < a_col; n3++){
                a_ind = (n2 * a_col) + n3;
                b_ind = (n3 * b_col) + n1;
                switch(cond1){
                    case 0: 
                        switch(cond2){
                            case 0: int_n_to_int_k_type_cast  (b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); break;
                            case 1: uint_n_to_int_k_type_cast (b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); break;
                            case 2: float_n_to_int_k_type_cast(b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); 
                        }
                    break;
                    case 1: 
                        switch(cond2){
                            case 0: int_n_to_uint_k_type_cast  (b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); break;
                            case 1: uint_n_to_uint_k_type_cast (b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); break;
                            case 2: float_n_to_uint_k_type_cast(b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); 
                            printf("%f\n", ptr.f32[0]);
                        }
                    break;
                    case 2: 
                        switch(cond2){
                            case 0: int_n_to_float_k_type_cast  (b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); break;
                            case 1: uint_n_to_float_k_type_cast (b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); break;
                            case 2: float_n_to_float_k_type_cast(b.elements.i8 + (b_ind * b_size), b_size, ptr.vptr, a_size, a.elements.i8 + (a_ind * a_size), r.err); 
                        }   
                }
                switch(a_type){
                    case INT8:    sum.i8   += int8_mult  (a.elements.i8  [a_ind], ptr.i8  [0], r.err); break;
                    case INT32:   sum.i32  += int32_mult (a.elements.i32 [a_ind], ptr.i32 [0], r.err); break;
                    case INT64:   sum.i64  += int64_mult (a.elements.i64 [a_ind], ptr.i64 [0], r.err); break; 
                    case UINT8:   sum.ui8  += uint8_mult (a.elements.ui8 [a_ind], ptr.ui8 [0], r.err); break;
                    case UINT32:  sum.ui32 += uint32_mult(a.elements.ui32[a_ind], ptr.ui32[0], r.err); break;
                    case UINT64:  sum.ui64 += uint64_mult(a.elements.ui64[a_ind], ptr.ui64[0], r.err); break;
                    case FLOAT32: sum.f32  += float_mult_round ((fbits){ .f = a.elements.f32[a_ind] }, (fbits){ .f = ptr.f32[0] }, r.err); break; 
                    case FLOAT64: sum.f64  += double_mult_round((dbits){ .d = a.elements.f64[a_ind] }, (dbits){ .d = ptr.f64[0] }, r.err); 
                }        
            }
            switch(a_type){
                case INT8:    r.elements.i8  [n2 * b_col + n1] = sum.i8  ; break; 
                case INT32:   r.elements.i32 [n2 * b_col + n1] = sum.i32 ; break;
                case INT64:   r.elements.i64 [n2 * b_col + n1] = sum.i64 ; break; 
                case UINT8:   r.elements.ui8 [n2 * b_col + n1] = sum.ui8 ; break;
                case UINT32:  r.elements.ui32[n2 * b_col + n1] = sum.ui32; break;
                case UINT64:  r.elements.ui64[n2 * b_col + n1] = sum.ui64; break;
                case FLOAT32: r.elements.f32 [n2 * b_col + n1] = sum.f32 ; break; 
                case FLOAT64: r.elements.f64 [n2 * b_col + n1] = sum.f64; 
            }
        }
    }
    free(ptr.vptr);
    return r;
}

matrix_t matrix_exp_first_arg_t(matrix_t a, matrix_t b){// TODO: what if amount of elements in data is less than n
    if(!(a.col && a.row && a.elements.i32 && a.err && a.type && b.col && b.row && b.elements.i32 && b.err && b.type)){ 
        if(a.err){ a.err[0] = NULL_POINTER; }    
        return a; 
    }
    uint32_t cond1 = a.err[0], cond2 = b.err[0], n1 = uint32_add(a.row[0], a.col[0], a.err), n2 = uint32_mult(b.row[0], b.col[0], b.err);
    a.err[0] = ternary((!cond1 && a.err[0]) | (!cond2 && b.err[0]) | (n1 > MATRIX_MAX_ELEM_BYTE_SIZE) | (n2 > MATRIX_MAX_ELEM_BYTE_SIZE), MEMORY_LIMIT_EXCESS, a.err[0]);
    a.err[0] = ternary(n1 != n2, SIZE_DOES_NOT_MATCH, a.err[0]);
    if((a.err[0] == MEMORY_LIMIT_EXCESS) | (a.err[0] == SIZE_DOES_NOT_MATCH)){ return a; }
    vecN r_vec = vec_exp_first_arg_t((vecN){ .type = a.type, .n = &n1, .err = a.err, .elements = a.elements }, (vecN){ .type = b.type, .n = &n2, .err = b.err, .elements = b.elements });
    matrix_t r = { .elements = r_vec.elements, .err = r_vec.err, .type = r_vec.type};
    uint32_t* dims = malloc(8);
    dims[0] = a.row[0];
    dims[1] = a.col[0];
    r.row = dims;
    r.col = dims + 1;
    return r;
}


/*
What if memory is smaller than N? Then, we access it out of bounds. That's a problem. What do we do?
A) Do not touch, make sure logic is airtight
B) Write checks
    1) every time, within your program, you re-write checks
    2.1) you write checks 1 time, put them in a set of structs and functions
    2.2) just use it everywhere

Memory safety is a big, unsolved problem. Different languages have different approaches:
    Rust - Borrow Checker
    JS, Python, ... - automatic memory management
    Swift, Zig - compiler checks
    Fil-C  - C library

*/
/*
Errors:
    Terminating errors
        some bit is always 1
    Non-terminating errors
        that same bit is always 0
    if (error >> 31) { return; } // terminating error
    else {} // non-terminating error


*/