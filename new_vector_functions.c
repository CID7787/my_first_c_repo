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

#define VECTOR_MAX_ELEM_SIZE MAX_UINT


vecN vec_create(datatype type, datapointer elements, uint64_t n){
    uint32_t condition = (n <= VECTOR_MAX_ELEM_SIZE);
    n = ((n & -condition) + !condition);

    void* r = malloc(sizeof(datatype) 
                + sizeof(uint32_t) 
                + sizeof(error) 
                + (n * amount_of_type_bytes(type)));

    vecN result;
        result.type     = (datatype*)   r;
        result.n        = (uint32_t*)  (result.type + 1);
        result.err      = (error*)     (result.n    + 1);
        result.elements = (datapointer)(result.err  + 1);

    result.type[0] = type;
    result.n   [0] = n;
    result.err [0] = ternary(condition, MEMORY_LIMIT_EXCESS, NO_ERROR);
    
    n *= amount_of_type_bytes(type); // potentially check for n exceeding bounds. 2 * 2147483647 / 8 ~= 547483647 ~= 0.5GB
    if(!elements.i32){ 
        result.err[0] = NULL_POINTER;
        while(n--){ result.elements.i8[n] = elements.i8[n]; } // TODO: what if elements points to less objects then n amount
    }

    return result;
}

vecN vec_filler(vecN vec, datapointer arr){
    if(vec.n && vec.type && vec.err && vec.elements.i32 && arr.i32){
        uint64_t i = vec.n[0];
        switch(vec.type[0]){
            case INT8:    while(i--){ vec.elements.i8  [i] = arr.i8[i]; } break;// TODO: what to do in scale have different type, and error handling
            case UINT8:   while(i--){ vec.elements.ui8 [i] = arr.ui8[i]; } break;
            case INT32:   while(i--){ vec.elements.i32 [i] = arr.i32[i]; } break;
            case UINT32:  while(i--){ vec.elements.ui32[i] = arr.ui32[i]; } break;
            case INT64:   while(i--){ vec.elements.i64 [i] = arr.i64[i]; } break;
            case UINT64:  while(i--){ vec.elements.ui64[i] = arr.ui64[i]; } break;
            case FLOAT32: 
                int8_t cond;
                fbits fv;
                while(i--){
                    fv = (fbits){ .f = arr.f32[i] }; 
                    cond = fv.parts.exp > MAX_NORM_FLOAT_EXP;
                    vec.err[0] = ternary(cond, ternary(fv.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), vec.err[0]);// check for infinity value
                    vec.err[0] = ternary(cond && fv.parts.mantissa, QNAN, vec.err[0]);// check for NaN
                    vec.elements.f32 [i] = arr.f32[i]; 
                } 
            break;
            case FLOAT64: 
                int8_t cond;
                dbits dv;
                while(i--){ 
                    dv = (dbits){ .d = arr.f64[i] };
                    cond = dv.parts.exp > MAX_NORM_DOUBLE_EXP;
                    vec.err[0] = ternary(cond, ternary(dv.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), vec.err[0]);// check for infinity value
                    vec.err[0] = ternary(cond && dv.parts.mantissa, QNAN, vec.err[0]);// check for NaN
                    vec.elements.f64 [i] = arr.f64[i]; 
                }
        }
    }
    return vec;
}

vecN vec_scalar_in_place(vecN vec, datapointer scale){
    if(scale.i32 && vec.elements.i32 && vec.err && vec.type && vec.n){
        uint32_t i = vec.n[0];
        switch(vec.type[0]){
            case INT8:    while(i--){ safe_char_multiplication (vec.elements.i8  [i], scale.i8  [0], vec.err); } break;
            case UINT8:   while(i--){ safe_uchar_multiplication(vec.elements.ui8 [i], scale.ui8 [0], vec.err); } break;
            case INT32:   while(i--){ safe_int_multiplication  (vec.elements.i32 [i], scale.i32 [0], vec.err); } break;
            case UINT32:  while(i--){ safe_uint_multiplication (vec.elements.ui32[i], scale.ui32[0], vec.err); } break;
            case INT64:   while(i--){ safe_lint_multiplication (vec.elements.i64 [i], scale.i64 [0], vec.err); } break;
            case UINT64:  while(i--){ safe_luint_multiplication(vec.elements.ui64[i], scale.ui64[0], vec.err); } break;
            case FLOAT32: while(i--){ safe_float_multiplication_with_rounding((fbits){ .f = vec.elements.f32 [i] }, (fbits){ .f = scale.f32[0] }, vec.err); } break;
            case FLOAT64: while(i--){ safe_double_multiplication_with_rounding((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.f64[0] }, vec.err); }
        }
    }
    return vec;
}

vecN vec_neg(vecN vec){
    vecN r = { .type = 0, .n = 0, .err = 0, .elements = 0};
    if(vec.type  && vec.elements.i32 && vec.err && vec.n){
        uint32_t i = vec.n[0];
        vec.type = malloc(sizeof(datatype) + sizeof(uint32_t) + sizeof(error) 
        + ( ( 1 + ((vec.type[0] == UINT8) | (vec.type[0] == UINT32)) ) * vec.n[0] * amount_of_type_bytes(vec.type[0])) );// ( 1 + ((vec.type[0] == UINT8) | (vec.type[0] == UINT32)) ) this part will double allocated memory or will multiply it to 1 depending on it's type
        r.n        = (uint32_t*)  (r.type + 1);
        r.err      = (error*)     (r.n    + 1);
        r.elements = (datapointer)(r.err  + 1);
        switch(r.type[0]){
            case INT8: 
                while(i--){ r.elements.i8[i] = -vec.elements.i8[i]; }   
                break;
            case UINT8:
                r.type[0] = UINT32;   
                while(i--){ r.elements.i32[i] = -vec.elements.ui8[i]; }   
                break;
            case INT32:   
                while(i--){ r.elements.i32[i] = -vec.elements.i32[i]; }   
                break;
            case UINT32:  
                r.type[0] = UINT64;   
                while(i--){ r.elements.i64[i] = -vec.elements.ui32[i]; }   
                break;            
            case INT64:  
                while(i--){ r.elements.i64[i] = -vec.elements.i64[i]; }   
                break;
            case UINT64:
                uint64_t max_negatiable_uint64 = MAX_INT + 1;
                r.type[0] = UINT64;
                while(i--){ 
                    r.err[0] = ternary(vec.elements.ui64[i] > max_negatiable_uint64, NEGATIVE_OVERFLOW, r.err[0]);
                    r.elements.i64[i] = -vec.elements.ui64[i]; 
                }   
                break;
            case FLOAT32: 
                while(i--){ vec.elements.f32[i] = -vec.elements.f32[i]; } 
                break;
            case FLOAT64: 
                while(i--){ vec.elements.f64[i] = -vec.elements.f64[i]; }
        }
    }
    return r;
}

vecN vec_neg_in_place(vecN vec){
    if(vec.elements.i32 && vec.err && vec.type && vec.n){
        uint32_t i = vec.n[0];
        switch (vec.type[0]){
            case INT8:    
                while(i--){ vec.elements.i8[i] = -vec.elements.i8[i]; } 
                break;
            case UINT8:   
                uint8_t max_negatiable_uint8 = MAX_CHAR + 1;
                vec.type[0] = INT8;
                while(i--){ 
                    vec.err[0] = ternary(vec.elements.ui8 > max_negatiable_uint8, NEGATIVE_OVERFLOW, vec.err[0]);
                    vec.elements.i8[i] = -vec.elements.ui8[i]; 
                } 
                break;
            case INT32:   
                while(i--){ vec.elements.i32[i] = -vec.elements.i32[i]; } 
                break;
            case UINT32:  
                uint32_t max_negatiable_uint32 = MAX_INT + 1;
                vec.type[0] = INT32;
                while(i--){
                    vec.err[0] = ternary(vec.elements.ui32 > max_negatiable_uint32, NEGATIVE_OVERFLOW, vec.err[0]);
                    vec.elements.i32[i] = -vec.elements.ui32[i]; 
                } 
                break;
            case INT64:   
                while(i--){ vec.elements.i64[i] = -vec.elements.i64[i]; } 
                break;
            case UINT64:
                uint64_t max_negatiable_uint64 = MAX_LINT + 1;
                vec.type[0] = INT64;
                while(i--){ 
                    vec.err[0] = ternary(vec.elements.ui64 > max_negatiable_uint64, NEGATIVE_OVERFLOW, vec.err[0]);                    
                    vec.elements.i64[i] = -vec.elements.ui64[i];
                } 
                break;
            case FLOAT32: 
                while(i--){ vec.elements.f32[i] = -vec.elements.f32[i]; } 
                break;
            case FLOAT64: 
                while(i--){ vec.elements.f64[i] = -vec.elements.f64[i]; }
        }
    }
    return vec;
}



/*sample
error {
    const char* function_of_origin: ...;
    error_type errT: ...;
    const int* buffer: ...;
    unsigned int buffer_length: 10000;
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