#ifndef headerfile
    #include "user_defined_datatypes.c"
    #include "logical_functions_of_decision.c"
#endif
unsigned int random_uint(unsigned int v, unsigned int u){
    v = 36969*(v & 65535) + (v >> 16);
    u = 18000*(u & 65535) + (u >> 16);
    return (v << 16) + (u & 65535);
}

uint32_t int32_abs(int32_t condition){ 
    int32_t the_original_value = condition;
    condition = *((uint32_t*)&condition) >> 31;// condition if last bit of arg is 1(if number is negative) 
    return (1 - (condition << 1)) * the_original_value;
}

uint64_t int64_abs(int64_t condition){ 
    int64_t the_original_value = condition;
    condition = *((int64_t*)&condition) >> 63;// condition if last bit of arg is 1(if number is negative) 
    return (1 - (condition << 1)) * the_original_value;
}

long int int64_neg(long int a){// 0011 --> 1101   
    return ~a + 1;
}

// FUNCTION: double_absolute_value(double)

double double_abs(dbits value){
    value.bits.sign = 0;
    return value.d;
}

void print_int8   (datapointer data){ printf("%4d",  *data.i8);   }
void print_uint8  (datapointer data){ printf("%4u",  *data.ui8);  }
void print_int32  (datapointer data){ printf("%4d",  *data.i32);  }
void print_uint32 (datapointer data){ printf("%4u",  *data.ui32); }
void print_int64  (datapointer data){ printf("%4ld", *data.i64);  }
void print_uint64 (datapointer data){ printf("%4lu", *data.ui64); }
void print_float32(datapointer data){ printf("%4f",  *data.f32);  }
void print_float64(datapointer data){ printf("%4lf", *data.f64);  }

void (*print_fund_type_func_arr[8])(datapointer) = {print_int8, print_uint8, print_int32, print_uint32, print_float32, print_int64, print_uint64, print_float64}; 
char *fund_types_sting_arr[8] = {"char", "unsigned char", "int", "unsigned int", "float", "long int", "long unsigned int", "double"};
char *errors_sting_arr[18] = { "no error", "positive overflow", "negative overflow", "underflow", "division by zero", "exponentiation of zero to zero", 
                            "undefined behavior", "square root of negative number", "positive infinity", "negative infinity", "return real part",
                            "null pointer", "quite NaN", "signaling NaN", "subnormal number", "incompatible", "attempt to get root of the number", "memory limit excess" };


unsigned char amount_of_type_bytes(datatype t){
    switch(t) {
        case INT8:     return sizeof(int8_t);   break;
        case UINT8:    return sizeof(int8_t);   break;
        case INT32:    return sizeof(int32_t);  break;
        case UINT32:   return sizeof(int32_t);  break;
        case FLOAT32:  return sizeof(float);    break;
        case INT64:    return sizeof(int64_t);  break;
        case UINT64:   return sizeof(int64_t);  break;
        case FLOAT64:  return sizeof(double);   break;
        default:       return sizeof(datatype); 
    }
}

unsigned char is_integer(datatype type){ 
    return !((type == FLOAT32) | (type == FLOAT64));
}

unsigned char is_unsigned(datatype type){
    return (type == UINT8) | (type == UINT32) | (type == UINT64);
}

uint8_t int_uint_float_t(datatype type){
    return ternary(is_integer(type), ternary(is_unsigned(type), 1, 0), 2); // function returns 0 when type is integer, 1 for unsigned int and 2 for float type
}

void print_vector(vecN a){
    unsigned int i = 0;
    printf("Vector type: %s, elements amount: %u, vector error: %s\nvector elements:\n", fund_types_sting_arr[a.type[0]], a.n[0], errors_sting_arr[a.err[0]]);
    while(i < a.n[0]){
        print_fund_type_func_arr[a.type[0]]((datapointer){ .i8 = a.elements.i8 + i * amount_of_type_bytes(a.type[0]) } );
        printf(", ");
        i++;
    }    
}

// void print_matrix(matrix_t a){
//     unsigned int i = 0;
//     printf("Matrix type: %s, size: %ux%u, matrix error: %s\nmatrix elements:\n", fund_types_sting_arr[a.type], a.rows, a.cols, errors_sting_arr[a.m_err]);
//     while(i < a.rows * a.cols){
//         if(!(i % a.cols)){ printf("\n"); }
//         print_fund_type_func_arr[a.type]((datapointer){ .i8 = a.elements.i8 + ((i / a.cols) * a.cols + (i % a.cols)) * amount_of_type_bytes(a.type) } );
//         printf(" ");
//         i++;
//     }    
// }

