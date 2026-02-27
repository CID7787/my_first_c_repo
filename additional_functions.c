unsigned int random_uint(unsigned int v, unsigned int u){
    v = 36969*(v & 65535) + (v >> 16);
    u = 18000*(u & 65535) + (u >> 16);
    return (v << 16) + (u & 65535);
}

unsigned int int_absolute_value(int condition){ 
    int the_original_value = condition;
    condition = *((unsigned int*)&condition) >> ((sizeof(int) << 3) - 1);// condition if last bit of arg is 1(if number is negative) 
    return (1 - (condition << 1)) * the_original_value;
}

long unsigned int lint_absolute_value(int condition){ 
    long int the_original_value = condition;
    condition = *((long unsigned int*)&condition) >> ((sizeof(long int) << 3) - 1);// condition if last bit of arg is 1(if number is negative) 
    return (1 - (condition << 1)) * the_original_value;
}

long int lint_negation(long int a){// 0011 --> 1101   
    return ~a + 1;
}

void print_int8(datapointer data)   { printf("%4d",  *data.i8);   }
void print_uint8(datapointer data)  { printf("%4u",  *data.ui8);  }
void print_int32(datapointer data)  { printf("%4d",  *data.i32);  }
void print_uint32(datapointer data) { printf("%4u",  *data.ui32); }
void print_int64(datapointer data)  { printf("%4ld", *data.i64);  }
void print_uint64(datapointer data) { printf("%4lu", *data.ui64); }
void print_float32(datapointer data){ printf("%4f",  *data.f32);  }
void print_float64(datapointer data){ printf("%4lf", *data.f64);  }

void (*print_fund_type_func_arr[8])(datapointer) = {print_int8, print_uint8, print_int32, print_uint32, print_float32, print_int64, print_uint64, print_float64}; 
char *fund_types_sting_arr[8] = {"char", "unsigned char", "int", "unsigned int", "float", "long int", "long unsigned int", "double"};
char *errors_sting_arr[16] = { "no error", "positive overflow", "negative overflow", "underflow", "division by zero", "exponentiation of zero to zero", 
                            "undefined behavior", "square root of negative number", "positive infinity", "negative infinity", "return real part",
                            "null pointer", "quite NaN", "signaling NaN", "incompatible", "attempt to get root of the number" };
