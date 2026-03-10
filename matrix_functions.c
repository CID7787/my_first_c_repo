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

matrix_t matrix_creation(datatype type, unsigned int row, unsigned int col, alldatapointer elements, void (*func)(matrix_t)){
    uint32_t r_size = amount_of_type_bytes(type);
    matrix_t r = {type, row, col, malloc(row * col * r_size), NO_ERROR};
    int* ptr;
    row *= col;
    while(row--){
        ptr = (int*)(elements.b1.i + row * r_size);
        switch(check_for_NULL(type, ptr)){
            case INT8:   r.elements.b1.i [row] = elements.b1.i [row]; break;
            case UINT8:  r.elements.b1.ui[row] = elements.b1.ui[row]; break;
            case INT32:    r.elements.b4.i [row] = elements.b4.i [row]; break;
            case UINT32:   r.elements.b4.ui[row] = elements.b4.ui[row]; break;
            case INT64:   r.elements.b8.i [row] = elements.b8.i [row]; break;
            case UINT64:  r.elements.b8.ui[row] = elements.b8.ui[row]; break;
            case FLOAT32:  r.   elements.b4.f [row] = elements.b4.f [row]; break;
            case FLOAT64: r.elements.b8.d [row] = elements.b8.d [row]; break;
            default: 
                ptr = (int*)func;
                if(ptr){ func(r); }
                return r;
            break;
        }
    }
    ptr = (int*)func;
    if(ptr){ func(r); }
    return r;
}

matrix_t matrix_neg(matrix_t matr){
    vecN vec_a = vec_neg((vecN){matr.type, matr.rows * matr.cols, matr.elements, matr.m_err});
    return (matrix_t){vec_a.type, matr.rows, matr.cols, vec_a.elements, vec_a.v_error};
}

matrix_t matrix_neg_in_place(matrix_t matr){
    vecN vec_a = vec_neg_in_place((vecN){matr.type, matr.rows * matr.cols, matr.elements, matr.m_err});
    return (matrix_t){vec_a.type, matr.rows, matr.cols, vec_a.elements, vec_a.v_error};
}

matrix_t matrix_addition(matrix_t a, matrix_t b){
    vecN a_vec = vec_add_of_arg1type( (vecN){ a.type, a.rows * a.cols, a.elements, a.m_err },(vecN){ b.type, b.rows * b.cols, b.elements, b.m_err } ); 
    a.elements = a_vec.elements;
    a.m_err = a_vec.v_error;
    return a;
}
