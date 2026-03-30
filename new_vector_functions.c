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


vecN vec_create(datatype type, uint64_t n){
    uint32_t condition = (n <= VECTOR_MAX_ELEM_SIZE);
    n &= -condition;

    void* r = malloc(sizeof(datatype) 
                + sizeof(uint32_t) 
                + sizeof(error) 
                + (n * amount_of_type_bytes(type)) );

    vecN result;
        result.type     = (datatype*)   r;
        result.n        = (uint32_t*)  (result.type + 1);
        result.err      = (error*)     (result.n    + 1);
        result.elements = (datapointer)(result.err  + 1);

    result.type[0] = type;
    result.n   [0] = n;
    result.err [0] = ternary(condition, NO_ERROR, MEMORY_LIMIT_EXCESS);

    return result;
}


error check_float_type_elem_for_err(datapointer data, datatype type, uint32_t n, error* err){ // TODO: what if amount of elements in data is less than n 
    if(!(data.i32 && err)){ return NULL_POINTER; }
    int8_t cond;
    switch(type){
        case FLOAT32:
            fbits fb;
            while(n--){
                fb = (fbits){ .f = data.f32[n] };
                cond = fb.parts.exp > MAX_NORM_FLOAT_EXP;
                *err = ternary(cond, ternary(fb.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
                *err = ternary(cond && fb.parts.mantissa, QNAN, *err);// check for NaN
            }
            break;
        case FLOAT64:
        dbits db;
        while(n--){
            db = (dbits){ .d = data.f64[n] };
            cond = db.parts.exp > MAX_NORM_DOUBLE_EXP;
            *err = ternary(cond, ternary(db.parts.sign, NEGATIVE_INFINITY, POSITIVE_INFINITY), *err);// check for infinity value
            *err = ternary(cond && db.parts.mantissa, QNAN, *err);// check for NaN
        }
    }
    return *err;
}


vecN vec_filler(vecN vec, datapointer arr){// TODO: what if amount of elements in data is less than n
    if(!(vec.n && vec.type && vec.err && vec.elements.i32 && arr.i32)){ 
        error er = NULL_POINTER;
        vec.err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)&er);
        return vec; 
    }
    uint64_t i = vec.n[0] * amount_of_type_bytes(vec.type[0]);
    while(i--){
        vec.elements.i8[i] = arr.i8[i];
    }
    return vec;
}


datapointer elem_type_cast(datapointer elem, datatype from, datatype to, uint32_t n, error* err){// TODO: there is an option to convert negative int type to positve unsigned the same way as C does
    // TODO: what if amount of elements in data is less than n
    if(!(elem.i32 && err)){
        error er = NULL_POINTER;
        err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)err, (long unsigned int)(long unsigned int*)err, (long unsigned int)(long unsigned int*)&er);        
        return elem; 
    }
    datapointer r = (datapointer){ .vptr = malloc(amount_of_type_bytes(to) * n) };
    switch(from){
            case INT8:
                switch(to){
                    case INT8:    while(n--){ r.i8 [n] = elem.i8[n]; } break;
                    case INT32:   while(n--){ r.i32[n] = elem.i8[n]; } break;
                    case INT64:   while(n--){ r.i64[n] = elem.i8[n]; } break;
                    case FLOAT32: while(n--){ r.f32[n] = elem.i8[n]; } break;
                    case FLOAT64: while(n--){ r.f64[n] = elem.i8[n]; } break;
                    case UINT8:
                        while(n--){
                            *err = ternary(elem.i8[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui8[n] = elem.i8[n]; 
                        }
                        break;
                    case UINT32: 
                        while(n--){
                            *err = ternary(elem.i8[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui32[n] = elem.i8[n]; 
                        }
                        break;
                    case UINT64: 
                        while(n--){
                            *err = ternary(elem.i8[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui64[n] = elem.i8[n]; 
                        }
                        break;
                }
                break;
            case UINT8:
                switch(to){
                    case INT8: 
                        while(n--){
                            *err = ternary(elem.ui8[n] > MAX_CHAR, POSITIVE_OVERFLOW, *err);
                            r.i8[n] = elem.ui8[n];
                        } 
                        break;
                    case UINT8:   while(n--){ r.ui8[n]  = elem.ui8[n]; } break;
                    case INT32:   while(n--){ r.i32[n]  = elem.ui8[n]; } break;
                    case UINT32:  while(n--){ r.ui32[n] = elem.ui8[n]; } break;
                    case INT64:   while(n--){ r.i64[n]  = elem.i8 [n]; } break;
                    case UINT64:  while(n--){ r.i64[n]  = elem.ui8[n]; } break;
                    case FLOAT32: while(n--){ r.f32[n]  = elem.ui8[n]; } break;
                    case FLOAT64: while(n--){ r.f64[n]  = elem.ui8[n]; } 
                }
                break;
            case INT32:
                switch(to){
                    case INT32:   while(n--){ r.i32[n] = elem.i32[n]; } break;
                    case INT64:   while(n--){ r.i64[n] = elem.i32[n]; } break;
                    case FLOAT32: while(n--){ r.f32[n] = elem.i32[n]; } break;
                    case FLOAT64: while(n--){ r.f64[n] = elem.i32[n]; } break; 
                    case INT8:    
                        while(n--){
                            *err = ternary(elem.i32[n] > MAX_CHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.i32[n] < MIN_CHAR, NEGATIVE_OVERFLOW, *err);
                            r.i8[n] = elem.i32[n];
                        }
                        break;
                    case UINT8:   
                        while(n--){ 
                            *err = ternary(elem.i32[n] > MAX_UCHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.i32[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui8[n] = elem.i32[n];
                        } 
                        break;
                        case UINT32:  
                        while(n--){
                            *err = ternary(elem.i32[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui32[n] = elem.i32[n];
                        } 
                        break;
                    case UINT64:  
                        while(n--){ 
                            *err = ternary(elem.i32[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui64[n] = elem.i32[n];
                        } 
                        break;
                }
                break;
            case UINT32:
                switch(to){
                    case UINT32:  while(n--){ r.ui32[n] = elem.ui32[n]; } break;
                    case INT64:   while(n--){ r.i64[n]  = elem.ui32[n]; } break;
                    case UINT64:  while(n--){ r.i64[n]  = elem.ui32[n]; } break;
                    case FLOAT32: while(n--){ r.f32[n]  = elem.ui32[n]; } break;
                    case FLOAT64: while(n--){ r.f64[n]  = elem.ui32[n]; } break; 
                    case INT8: 
                        while(n--){
                            *err = ternary(elem.ui32[n] > MAX_CHAR, POSITIVE_OVERFLOW, *err);
                            r.i8[n] = elem.ui32[n];
                        } 
                        break;
                    case UINT8:   
                        while(n--){ 
                            *err = ternary(elem.ui32[n] > MAX_UCHAR, POSITIVE_OVERFLOW, *err);
                            r.ui8[n]  = elem.ui32[n]; 
                        }
                        break;
                    case INT32:   
                        while(n--){
                            *err = ternary(elem.ui32[n] > MAX_INT, POSITIVE_OVERFLOW, *err);
                            r.i32[n] = elem.ui32[n];
                        }
                        break;
                }
                break;
            case INT64:
                switch(to){
                    case INT64:   while(n--){ r.i64[n] = elem.i64[n]; } break;
                    case FLOAT32: while(n--){ r.f32[n] = elem.i64[n]; } break;
                    case FLOAT64: while(n--){ r.f64[n] = elem.i64[n]; } break; 
                    case INT8:    
                        while(n--){
                            *err = ternary(elem.i64[n] > MAX_CHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.i64[n] < MIN_CHAR, NEGATIVE_OVERFLOW, *err);
                            r.i8[n] = elem.i64[n];
                        }
                        break;
                    case UINT8:   
                        while(n--){ 
                            *err = ternary(elem.i64[n] > MAX_UCHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.i64[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui8[n] = elem.i64[n];
                        } 
                        break;
                    case INT32:   
                        while(n--){ 
                            *err = ternary(elem.i64[n] > MAX_INT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.i64[n] < MIN_INT, NEGATIVE_OVERFLOW, *err);                        
                            r.i32[n] = elem.i64[n]; 
                        } 
                        break;
                    case UINT32:  
                        while(n--){
                            *err = ternary(elem.i64[n] > MAX_UINT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.i64[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui32[n] = elem.i64[n];
                        } 
                        break;
                    case UINT64:  
                        while(n--){ 
                            *err = ternary(elem.i64[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui64[n] = elem.i64[n];
                        } 
                        break;
                }
                break;
            case UINT64:
                switch(to){
                    case UINT64:  while(n--){ r.i64[n]  = elem.ui64[n]; } break;
                    case FLOAT32: while(n--){ r.f32[n]  = elem.ui64[n]; } break;
                    case FLOAT64: while(n--){ r.f64[n]  = elem.ui64[n]; } break; 
                    case INT8: 
                        while(n--){
                            *err = ternary(elem.ui64[n] > MAX_CHAR, POSITIVE_OVERFLOW, *err);
                            r.i8[n] = elem.ui64[n];
                        } 
                        break;
                    case UINT8:   
                        while(n--){ 
                            *err = ternary(elem.ui64[n] > MAX_UCHAR, POSITIVE_OVERFLOW, *err);
                            r.ui8[n]  = elem.ui64[n]; 
                        }
                        break;
                    case INT32:   
                        while(n--){
                            *err = ternary(elem.ui32[n] > MAX_INT, POSITIVE_OVERFLOW, *err);
                            r.i32[n] = elem.ui64[n];
                        }
                        break;
                    case UINT32:  
                        while(n--){ 
                            *err = ternary(elem.ui64[n] > MAX_UINT, POSITIVE_OVERFLOW, *err);
                            r.ui32[n] = elem.ui64[n]; 
                        } 
                        break;
                    case INT64:   
                        while(n--){ 
                            *err = ternary(elem.ui64[n] > MAX_LINT, POSITIVE_OVERFLOW, *err);
                            r.i64[n]  = elem.ui64[n]; 
                        } 
                        break;
                }
                break;
            case FLOAT32:
                switch(to){
                    case INT8:    
                        while(n--){
                            *err = ternary(elem.f32[n] > MAX_CHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f32[n] < MIN_CHAR, NEGATIVE_OVERFLOW, *err);
                            r.i8[n] = elem.f32[n];
                        }
                    break;
                    case UINT8:   
                        while(n--){
                            *err = ternary(elem.f32[n] > MAX_UCHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f32[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui8[n] = elem.f32[n];
                        }
                    break;
                    case INT32:   
                        while(n--){
                            *err = ternary(elem.f32[n] > MAX_INT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f32[n] < MIN_INT, NEGATIVE_OVERFLOW, *err);
                            r.i32[n] = elem.f32[n];
                        }
                    break;
                    case UINT32:  
                        while(n--){
                            *err = ternary(elem.f32[n] > MAX_UINT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f32[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui32[n] = elem.f32[n];
                        }
                    break;
                    case INT64:   
                        while(n--){
                            *err = ternary(elem.f32[n] > MAX_LINT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f32[n] < MIN_LINT, NEGATIVE_OVERFLOW, *err);
                            r.i64[n] = elem.f32[n];
                        }
                    break;
                    case UINT64:  
                        while(n--){
                            *err = ternary(elem.f32[n] > MAX_LUINT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f32[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui64[n] = elem.f32[n];
                        }
                    break;
                    case FLOAT32: while(n--){ r.f32 [n] = elem.f32[n]; } break;
                    case FLOAT64: while(n--){ r.f64[n] = elem.f32[n];}
                }
                break;
            case FLOAT64:
                switch(to){
                    case INT8:    
                        while(n--){
                            *err = ternary(elem.f64[n] > MAX_CHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f64[n] < MIN_CHAR, NEGATIVE_OVERFLOW, *err);
                            r.i8[n] = elem.f64[n];
                        }
                    break;
                    case UINT8:   
                        while(n--){
                            *err = ternary(elem.f64[n] > MAX_UCHAR, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f64[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui8[n] = elem.f64[n];
                        }
                    break;
                    case INT32:   
                        while(n--){
                            *err = ternary(elem.f64[n] > MAX_INT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f64[n] < MIN_INT, NEGATIVE_OVERFLOW, *err);
                            r.i32[n] = elem.f64[n];
                        }
                    break;
                    case UINT32:  
                        while(n--){
                            *err = ternary(elem.f64[n] > MAX_UINT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f64[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui32[n] = elem.f64[n];
                        }
                    break;
                    case INT64:   
                        while(n--){
                            *err = ternary(elem.f64[n] > MAX_LINT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f64[n] < MIN_LINT, NEGATIVE_OVERFLOW, *err);
                            r.i64[n] = elem.f64[n];
                        }
                    break;
                    case UINT64:  
                        while(n--){
                            *err = ternary(elem.f64[n] > MAX_LUINT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f64[n] < 0, NEGATIVE_OVERFLOW, *err);
                            r.ui64[n] = elem.f64[n];
                        }
                    break;
                    case FLOAT32: 
                        while(n--){
                            *err = ternary(elem.f64[n] > MAX_FLOAT, POSITIVE_OVERFLOW, *err);
                            *err = ternary(elem.f64[n] < NEG_MAX_FLOAT, NEGATIVE_OVERFLOW, *err);
                            r.f32[n] = elem.f64[n];
                        }
                    break;
                    case FLOAT64: while(n--){ r.f64[n] = elem.f64[n];}
                }
        }
    return r;
}


vecN vec_scaler_in_place(vecN vec, datatype scale_type, fundtypeunion scale){// TODO: what if amount of elements in data is less than n
    if(!(vec.elements.i32 && vec.err && vec.type && vec.n)){// TODO: how to inform about NULL 
        error er = NULL_POINTER;
        vec.err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)&er);
        return vec; 
    } 
    uint32_t i = vec.n[0];
    switch(vec.type[0]){
        case INT8:    
            switch(scale_type){
                case INT8:    
                    while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.i8, vec.err); } 
                    break; 
                case UINT8:   
                    vec.err[0] = ternary(scale.ui8 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]); 
                    while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.ui8, vec.err); } 
                    break;
                case INT32:   
                    vec.err[0] = ternary(scale.i32 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.i32 < MIN_CHAR, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.i32, vec.err); } 
                    break;
                case UINT32:  
                    vec.err[0] = ternary(scale.ui32 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.ui32, vec.err); } 
                    break;
                case INT64:   
                    vec.err[0] = ternary(scale.i64 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.i64 < MIN_CHAR, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.i64, vec.err); } 
                    break;
                case UINT64:
                    vec.err[0] = ternary(scale.ui64 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i8[i] = int8_mult(vec.elements.i8[i], scale.ui64, vec.err); } 
                    break;
                case FLOAT32: 
                    vec.err[0] = ternary(scale.f32 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f32 < MIN_CHAR, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i8[i] = float_mult_round((fbits){ .f = vec.elements.i8[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                    break;
                case FLOAT64:
                    vec.err[0] = ternary(scale.f64 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f64 < MIN_CHAR, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i8[i] = float_mult_round((fbits){ .f = vec.elements.i8[i] } , (fbits){ .f = scale.f64 }, vec.err); } 
                    break;
            }
            break;
        case UINT8:    
            switch(scale_type){
                case INT8:    
                    vec.err[0] = ternary(scale.i8 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.i8, vec.err); } 
                    break; 
                case UINT8:   
                    while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.ui8, vec.err); } 
                    break;
                case INT32:   
                    vec.err[0] = ternary(scale.i32 > MAX_UCHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.i32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.i32, vec.err); } 
                    break;
                case UINT32:  
                    vec.err[0] = ternary(scale.ui32 > MAX_UCHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.ui32, vec.err); } 
                    break;
                case INT64:   
                    vec.err[0] = ternary(scale.i64 > MAX_UCHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.i64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.i64, vec.err); } 
                    break;
                case UINT64:
                    vec.err[0] = ternary(scale.ui64 > MAX_UCHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui8[i] = uint8_mult(vec.elements.ui8[i], scale.ui64, vec.err); } 
                    break;
                case FLOAT32: 
                    vec.err[0] = ternary(scale.f32 > MAX_UCHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui8[i] = float_mult_round((fbits){ .f = vec.elements.ui8[i] }, (fbits){ . f= scale.f32 }, vec.err); }  
                    break;
                case FLOAT64:
                    vec.err[0] = ternary(scale.f64 > MAX_UCHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui8[i] = float_mult_round((fbits){ .f = vec.elements.ui8[i] }, (fbits){ . f= scale.f64 }, vec.err); }
                    break;
            }
            break;
        case INT32: 
            switch(scale_type){
                case INT8:    
                    while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.i8, vec.err); } 
                    break; 
                case UINT8:   
                    while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.ui32, vec.err); } 
                    break;
                case INT32:   
                    while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.i32, vec.err); } 
                    break;
                case UINT32:  
                    vec.err[0] = ternary(scale.ui32 > MAX_CHAR, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.ui32, vec.err); } 
                    break;
                case INT64:   
                    vec.err[0] = ternary(scale.i64 > MAX_INT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.i64 < MIN_INT, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.i64, vec.err); } 
                    break;
                case UINT64:
                    vec.err[0] = ternary(scale.ui64 > MAX_INT, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i32[i] = int32_mult(vec.elements.i32[i], scale.ui64, vec.err); } 
                    break;
                case FLOAT32: 
                    vec.err[0] = ternary(scale.f32 > MAX_INT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f32 < MIN_INT, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i32[i] = float_mult_round((fbits){ .f = vec.elements.i32[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                    break;
                case FLOAT64:
                    vec.err[0] = ternary(scale.f64 > MAX_INT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f64 < MIN_INT, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i32[i] = float_mult_round((fbits){ .f = vec.elements.i32[i] }, (fbits){ . f= scale.f64 }, vec.err); } 
                    break;
            }
            break;
        case UINT32: 
            switch(scale_type){
                case INT8:    
                    vec.err[0] = ternary(scale.i8 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.i8, vec.err); } 
                    break; 
                case UINT8:   
                    while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.ui8, vec.err); } 
                    break;
                case INT32:   
                    vec.err[0] = ternary(scale.i32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.i32, vec.err); } 
                    break;
                case UINT32:  
                    while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.ui32, vec.err); } 
                    break;
                case INT64:   
                    vec.err[0] = ternary(scale.i64 > MAX_UINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.i64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.i64, vec.err); } 
                    break;
                case UINT64:
                    vec.err[0] = ternary(scale.ui64 > MAX_UINT, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui32[i] = uint32_mult(vec.elements.ui32[i], scale.ui64, vec.err); } 
                    break;
                case FLOAT32: 
                    vec.err[0] = ternary(scale.f32 > MAX_UINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui32[i] = float_mult_round((fbits){ .f = vec.elements.ui32[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                    break;
                case FLOAT64:
                    vec.err[0] = ternary(scale.f64 > MAX_UINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui32[i] = float_mult_round((fbits){ .f = vec.elements.ui32[i] }, (fbits){ . f= scale.f64 }, vec.err); } 
                    break;
            }
        case INT64:   
            switch(scale_type){
                case INT8:    
                    while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.i8, vec.err); } 
                    break; 
                case UINT8:   
                    while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.ui32, vec.err); } 
                    break;
                case INT32:   
                    while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.i32, vec.err); } 
                    break;
                case UINT32:  
                    while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.ui32, vec.err); } 
                    break;
                case INT64:   
                    while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.i64, vec.err); } 
                    break;
                case UINT64:
                    vec.err[0] = ternary(scale.ui64 > MAX_LINT, POSITIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i64[i] = int64_mult(vec.elements.i64[i], scale.ui64, vec.err); } 
                    break;
                case FLOAT32: 
                    vec.err[0] = ternary(scale.f32 > MAX_LINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f32 < MIN_LINT, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i64[i] = float_mult_round((fbits){ .f = vec.elements.i64[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                    break;
                case FLOAT64:
                    vec.err[0] = ternary(scale.f64 > MAX_LINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f64 < MIN_LINT, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.i64[i] = float_mult_round((fbits){ .f = vec.elements.i64[i] }, (fbits){ . f= scale.f64 }, vec.err); }
                    break;
            }
        case UINT64:
            switch(scale_type){
                case INT8:    
                    vec.err[0] = ternary(scale.i8 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.i8, vec.err); } 
                    break; 
                case UINT8:   
                    while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.ui8, vec.err); } 
                    break;
                case INT32:   
                    vec.err[0] = ternary(scale.i32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.i32, vec.err); } 
                    break;
                case UINT32:  
                    while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.ui32, vec.err); } 
                    break;
                case INT64:   
                    vec.err[0] = ternary(scale.i64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.i64, vec.err); } 
                    break;
                case UINT64:
                    while(i--){ vec.elements.ui64[i] = uint64_mult(vec.elements.ui64[i], scale.ui64, vec.err); } 
                    break;
                case FLOAT32: 
                    vec.err[0] = ternary(scale.f32 > MAX_LUINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f32 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui64[i] = float_mult_round((fbits){ .f = vec.elements.ui64[i] }, (fbits){ . f= scale.f32 }, vec.err); } 
                    break;
                case FLOAT64:
                    vec.err[0] = ternary(scale.f64 > MAX_LUINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f64 < 0, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.ui64[i] = float_mult_round((fbits){ .f = vec.elements.ui64[i] }, (fbits){ . f= scale.f64 }, vec.err); }
                    break;
            }
        case FLOAT32:
            switch(scale_type){
                case INT8:    while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.i8   }, vec.err); } break; 
                case UINT8:   while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.ui32 }, vec.err); } break;
                case INT32:   while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.i32  }, vec.err); } break;
                case UINT32:  while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.ui32 }, vec.err); } break;
                case INT64:   while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.i64  }, vec.err); } break;
                case UINT64:  while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.ui64 }, vec.err); } break;
                case FLOAT32: while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] }, (fbits){ .f = scale.f32  }, vec.err); } break;
                case FLOAT64:
                    vec.err[0] = ternary(scale.f64 > MAX_LINT, POSITIVE_OVERFLOW, vec.err[0]);
                    vec.err[0] = ternary(scale.f64 < MIN_LINT, NEGATIVE_OVERFLOW, vec.err[0]);
                    while(i--){ vec.elements.f32[i] = float_mult_round((fbits){ .f = vec.elements.f32[i] } , (fbits){ .f = scale.f64 }, vec.err); } 
                    break;
            }
        case FLOAT64:
        switch(scale_type){
            case INT8:    while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.i8   }, vec.err); } break; 
            case UINT8:   while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.ui32 }, vec.err); } break;
            case INT32:   while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.i32  }, vec.err); } break;
            case UINT32:  while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.ui32 }, vec.err); } break;
            case INT64:   while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.i64  }, vec.err); } break;
            case UINT64:  while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.ui64 }, vec.err); } break;
            case FLOAT32: while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.f32  }, vec.err); } break;
            case FLOAT64: while(i--){ vec.elements.f64[i] = double_mult_round((dbits){ .d = vec.elements.f64[i] }, (dbits){ .d = scale.f64  }, vec.err); } break;
        }
    }
    return vec;
}


vecN vec_neg(vecN vec){// TODO: what if amount of elements in data is less than n
    vecN r;
    if(!(vec.type  && vec.elements.i32 && vec.err && vec.n)){ 
        error er = NULL_POINTER;
        vec.err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)&er);
        return vec; 
    }
    uint32_t i = vec.n[0];
    r = vec_create( ternary(UINT8 == vec.type[0], INT32, ternary(UINT32 == vec.type[0], INT64, vec.type[0])), vec.n[0]);
    switch(r.type[0]){
        case INT8:    while(i--){ r.elements.i8 [i] = -vec.elements.i8 [i]; } break;
        case INT32:   while(i--){ r.elements.i32[i] = -vec.elements.i32[i]; } break;
        case INT64:   while(i--){ r.elements.i64[i] = -vec.elements.i64[i]; } break;
        case FLOAT32: while(i--){ r.elements.f32[i] = -vec.elements.f32[i]; } break;
        case FLOAT64: while(i--){ r.elements.f64[i] = -vec.elements.f64[i]; } break;
        case UINT8:
            r.type[0] = UINT32;   
            while(i--){ r.elements.i32[i] = -vec.elements.ui8[i]; }// C standard says the unary - promotes unsigned integer types 
            break;
        case UINT32:  
            r.type[0] = UINT64; 
            while(i--){ r.elements.i64[i] = -vec.elements.ui32[i]; } 
            break;            
        case UINT64:
            uint64_t max_negateable_uint64 = MAX_INT + 1;
            r.type[0] = UINT64;
            while(i--){ 
                r.err[0] = ternary(vec.elements.ui64[i] > max_negateable_uint64, NEGATIVE_OVERFLOW, r.err[0]);
                r.elements.i64[i] = -vec.elements.ui64[i]; 
            }   
            break;
    }
    return r;
}


vecN vec_neg_in_place(vecN vec){// TODO: what if amount of elements in data is less than n
    if(!(vec.elements.i32 && vec.err && vec.type && vec.n)){ 
        error er = NULL_POINTER;
        vec.err = (error*)(long unsigned int*)ternary((long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)vec.err, (long unsigned int)(long unsigned int*)&er);
        return vec; 
    }
    uint32_t i = vec.n[0];
    switch (vec.type[0]){
        case INT8:    while(i--){ vec.elements.i8 [i] = -vec.elements.i8 [i]; } break;
        case INT32:   while(i--){ vec.elements.i32[i] = -vec.elements.i32[i]; } break;
        case INT64:   while(i--){ vec.elements.i64[i] = -vec.elements.i64[i]; } break;
        case FLOAT32: while(i--){ vec.elements.f32[i] = -vec.elements.f32[i]; } break;
        case FLOAT64: while(i--){ vec.elements.f64[i] = -vec.elements.f64[i]; } break;
        case UINT8:   
            uint8_t max_negateable_uint8 = MAX_CHAR + 1;
            vec.type[0] = INT8;
            while(i--){ 
                vec.err[0] = ternary(vec.elements.ui8[i] > max_negateable_uint8, NEGATIVE_OVERFLOW, vec.err[0]);
                vec.elements.i8[i] = -vec.elements.ui8[i]; 
            } 
            break;
        case UINT32:  
            uint32_t max_negateable_uint32 = MAX_INT + 1;
            vec.type[0] = INT32;
            while(i--){
                vec.err[0] = ternary(vec.elements.ui32[i] > max_negateable_uint32, NEGATIVE_OVERFLOW, vec.err[0]);
                vec.elements.i32[i] = -vec.elements.ui32[i]; 
            } 
            break;
        case UINT64:
            uint64_t max_negateable_uint64 = MAX_LINT + 1;
            vec.type[0] = INT64;
            while(i--){ 
                vec.err[0] = ternary(vec.elements.ui64[i] > max_negateable_uint64, NEGATIVE_OVERFLOW, vec.err[0]);                    
                vec.elements.i64[i] = -vec.elements.ui64[i];
            } 
            break;
        }
    return vec;
}


vecN vec_add_first_arg_t(vecN a, vecN b){
    if(!(a.type && a.n && a.err && a.elements.i32 && b.type && b.n && b.err && b.elements.i32)){ return a; }
    vecN r = vec_create(a.type[0], ternary(a.n[0] > b.n[0], a.n[0], b.n[0])),
         *arr[3] = { &(vecN){ .elements.i64 = (int64_t[]){0} } };
    fundtypeunion val;
    uint32_t i = r.n[0], ai, bi;
    while(i--){
        switch(b.type[0]){
            case INT8:
            case UINT8:
            case INT32:
            case UINT32:
            case INT64:
            case UINT64:
            case FLOAT32:
            case FLOAT64:
        }
    }
    return r;
}

#define FUNC_ARR_ELEM(a, b, c) ((a) * 6) + ((b) * 2) + (c) // this macro counts which element(type_convertion_function) of type_conv_func_arr will be called, for principle of it's work please reference to string number 741
//  a * 6 stand for a arg type's category(int, uint or float), b * 2 stand for which category does b arg type belongs to, and c determines whether to use little endian function of big endian fuction 

void int_n_to_int_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void int_n_to_int_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}

void uint_n_to_int_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void uint_n_to_int_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}

void float_n_to_int_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void float_n_to_int_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}


void int_n_to_uint_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void int_n_to_uint_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}

void uint_n_to_uint_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void uint_n_to_uint_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}

void float_n_to_uint_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void float_n_to_uint_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}


void int_n_to_float_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void int_n_to_float_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}

void uint_n_to_float_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void uint_n_to_float_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}

void float_n_to_float_k_len_big_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}
void float_n_to_float_k_len_lit_end(void* from, uint32_t from_s, void* to, uint32_t to_size, error* err){}

void (*type_conv_func_arr[18])(void*, uint32_t, void*, uint32_t, error*) = 
        { int_n_to_int_k_len_big_end, int_n_to_int_k_len_lit_end, uint_n_to_int_k_len_big_end, uint_n_to_int_k_len_lit_end , float_n_to_int_k_len_big_end, float_n_to_int_k_len_lit_end,
          int_n_to_uint_k_len_big_end, int_n_to_uint_k_len_lit_end, uint_n_to_uint_k_len_big_end, uint_n_to_uint_k_len_lit_end, float_n_to_uint_k_len_big_end, float_n_to_uint_k_len_lit_end,
          int_n_to_float_k_len_big_end, int_n_to_float_k_len_lit_end, uint_n_to_float_k_len_big_end, uint_n_to_float_k_len_lit_end, float_n_to_float_k_len_big_end, float_n_to_float_k_len_lit_end };


int is_little_end(){
    int32_t i = 0x01020304;
    return *(int8_t*)&i == 4; 
}


vecN vec_add_first_arg_t(vecN a, vecN b){// TODO: what if amount of elements in data is less than n
    if(!(a.type && a.n && a.err && a.elements.i32 && b.type && b.n && b.err && b.elements.i32)){ return a; }
    vecN r = vec_create(a.type[0], ternary(a.n[0] > b.n[0], a.n[0], b.n[0])),
    *arr[3] = { &(vecN){ .elements.i64 = (int64_t[]){0} }, &a, &b };
    uint32_t i = r.n, ai, bi;
    uint8_t end = is_little_end(), a_size = amount_of_type_bytes(a.type[0]), b_size = amount_of_type_bytes(b.type[0]), a_type_cat = int_uint_float_t(a.type[0]), b_type_cat = int_uint_float_t(b.type[0]);
    datapointer ptr;
    while(i--){
        ai = i < a.n[0];
        bi = i < b.n[0];
        type_conv_func_arr[ FUNC_ARR_ELEM( a_type_cat, b_type_cat, end ) ] (arr[bi << 1]->elements.i8 + (bi * i * b_size), b_size, ptr.vptr, a_size, r.err);
        switch(a.type[0]){
            case INT8:    r.elements.i8  [i] = int8_add  (arr[ai]->elements.i8  [ai * i], ptr.i8  [0], r.err); break; 
            case UINT8:   r.elements.ui8 [i] = uint8_add (arr[ai]->elements.ui8 [ai * i], ptr.ui8 [0], r.err); break;
            case INT32:   r.elements.i32 [i] = int32_add (arr[ai]->elements.i32 [ai * i], ptr.i32 [0], r.err); break;
            case UINT32:  r.elements.ui32[i] = uint32_add(arr[ai]->elements.ui32[ai * i], ptr.ui32[0], r.err); break;
            case INT64:   r.elements.i64 [i] = int64_add (arr[ai]->elements.i64 [ai * i], ptr.i64 [0], r.err); break; 
            case UINT64:  r.elements.ui64[i] = uint64_add(arr[ai]->elements.ui64[ai * i], ptr.ui64[0], r.err); break;
            case FLOAT32: r.elements.f32 [i] = float_add((fbits){ .f = arr[ai]->elements.f32[ai * i] }, (fbits){ .f = ptr.f32[0] }, r.err); break; 
            case FLOAT64: r.elements.f64 [i] = float_add((dbits){ .d = arr[ai]->elements.f64[ai * i] }, (dbits){ .d = ptr.f64[0] }, r.err); break; 
        }
    } 
    return r;
}

vecN vec_mult_first_arg_t(vecN a, vecN b){// TODO: what if amount of elements in data is less than n
    if(!(a.type && a.n && a.err && a.elements.i32 && b.type && b.n && b.err && b.elements.i32)){ return a; }
    if(a.n[0] ^ b.n[0]){ 
        error er = malloc(sizeof(error));
        return (vecN){ .err = &er }; 
    }
    vecN r = vec_create(a.type[0], a.n[0]);
    uint32_t i = r.n;
    uint8_t end = is_little_end(), a_size = amount_of_type_bytes(a.type[0]), b_size = amount_of_type_bytes(b.type[0]), a_type_cat = int_uint_float_t(a.type[0]), b_type_cat = int_uint_float_t(b.type[0]);
    datapointer ptr;
    while(i--){
        type_conv_func_arr[ FUNC_ARR_ELEM( a_type_cat, b_type_cat, end ) ] (b.elements.i8 + (i * b_size), b_size, ptr.vptr, a_size, r.err);
        switch(a.type[0]){
            case INT8:    r.elements.i8  [i] = int8_mult  (a.elements.i8  [i], ptr.i8  [0], r.err); break; 
            case UINT8:   r.elements.ui8 [i] = uint8_mult (a.elements.ui8 [i], ptr.ui8 [0], r.err); break;
            case INT32:   r.elements.i32 [i] = int32_mult (a.elements.i32 [i], ptr.i32 [0], r.err); break;
            case UINT32:  r.elements.ui32[i] = uint32_mult(a.elements.ui32[i], ptr.ui32[0], r.err); break;
            case INT64:   r.elements.i64 [i] = int64_mult (a.elements.i64 [i], ptr.i64 [0], r.err); break; 
            case UINT64:  r.elements.ui64[i] = uint64_mult(a.elements.ui64[i], ptr.ui64[0], r.err); break;
            case FLOAT32: r.elements.f32 [i] = float_mult ((fbits){ .f = a.elements.f32[i] }, (fbits){ .f = ptr.f32[0] }, r.err); break; 
            case FLOAT64: r.elements.f64 [i] = float_mult ((dbits){ .d = a.elements.f64[i] }, (dbits){ .d = ptr.f64[0] }, r.err); break; 
        }
    } 
    return r;
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