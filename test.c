


void d_add_test(void){
    dbits a, b, result;
    error err = NO_ERROR;
    srand(899);
    unsigned int sizeof_int_in_bits = sizeof(int) << 3;
    for(int i = 0; i < 1000; ++i){
        a.luint  = ((long unsigned int)rand() << sizeof_int_in_bits) | rand();
        b.luint  = ((long unsigned int)rand() << sizeof_int_in_bits) | rand();
        result.d = safe_double_addition(a, b, &err);
        printf("%d::: %f   * \t %f = \t %f\n %064lb * %064lb = ", i, a.d, b.d , result.d, a.d, b.d);
        printf("%064lb \nerror: %u\n", result.luint, err);
        
        err = NO_ERROR;  
    }
}

void f_add_test(void){
    fbits a, b, result;
    error err = NO_ERROR;
    srand(899);
    unsigned int sizeof_int_in_bits = sizeof(int) << 3;
    for(int i = 0; i < 1000; ++i){
        a.uint  = ((long unsigned int)rand() << sizeof_int_in_bits) | rand();
        b.uint  = ((long unsigned int)rand() << sizeof_int_in_bits) | rand();
        result.f = safe_float_addition(a, b, &err);
        printf("%d::: %f   * \t %f = \t %f\n %064lb * %064lb = ", i, a.f, b.f , result.f, a.f, b.f);
        printf("%064lb \nerror: %u\n", result.luint, err);
        
        err = NO_ERROR;  
    }
}

/*
 nan   *         3.32825e-294 =          9.22337e+18
 0 00000001111 11111111000101010110010010001011000011010001110111111001100 * 10101011110111001110110101100100101000010001000 = 11111110011011100100111001000001001000000111100
*/