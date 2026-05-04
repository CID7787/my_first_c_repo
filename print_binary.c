#ifndef headerfile
    #include "user_defined_datatypes.c"
#endif

// Example usage:
const fundt_un IEEE754_FLOAT_MASK = { .i64 =   0b10000001000000000000000000000000};
const fundt_un IEEE754_DOUBLE_MASK = { .ui64 = 0b100000000001000000000000000000000000000000000000000000000000000};
const fundt_un LONG_INT_SIGN_MASK = { .ui64 =  0b100000000000000000000000000000000000000000000000000000000000000};

// print_binary_actual_best_version_with_one_variable((fundt_un){ .d = 10.25}, (fundt_un){.ui64 = 0}, IEEE754_DOUBLE_MASK);

void print_binary_actual_best_version_with_one_variable(fundt_un x, fundt_un stop_where_bit_is_1, fundt_un whitespace_mask){
    unsigned int i = sizeof(fundt_un) << 3;
    while(i-- > 0){
        if ((stop_where_bit_is_1.ui64 >> i) & 1) { return; }
        if ((whitespace_mask.ui64 >> i) & 1) { printf(" "); }
        printf("%u", (x.ui64 >> i) & 1);
    }
}

void print_binary_my_first_best_version_with_one_variable(fundt_un x, unsigned int byte_amount, fundt_un mask){
    if(!byte_amount){ return; }
    unsigned int i = sizeof(fundt_un);
    if(byte_amount <= i){ i = byte_amount; } else { printf("The number of bytes is larger than there are bytes in the variable \n"); }
    i <<= 3;
    while(i-- > 0){
        if ((mask.ui64 >> i) & 1) { printf(" "); }
        printf("%u", (x.ui64 >> i) & 1);
    }
}

void print_binary_with_arrays(fundt_un x, unsigned int byte_amount, unsigned char* whitespace_after_bit_indexes, unsigned int array_size){// whitespace indexes have absolute index starting from leftmost bit
    if(!whitespace_after_bit_indexes){ return; }
    if(!byte_amount){ return; }
    unsigned int i = sizeof(fundt_un);
    if(byte_amount <= i){ i = byte_amount; } else { printf("The number of bytes is larger than there are bytes in the variable \n"); }
    i <<= 3;
    while(i-- > 0){
        unsigned int index = 0;
        while(index < array_size){
            if(i == whitespace_after_bit_indexes[index++]){ printf(" "); break; }
        }
        printf("%u", (x.ui64 >> i) & 1);
    }
}
void print_binary_first_amirako_version(fundt_un x){// x = 0
    int i = 64;
    while(i-- > 0){
        if((i == 62) | (i == 51)){ printf(" "); }
        printf("%u", (x.ui64 >> i) & 1);
    }
}

void print_binary_new_version1(fundt_un x, unsigned int byte_amount, unsigned int *spaces){// TODO: rewrite it as function that accepts array that contain information about at what postion to put spaces
    if(!spaces){ return; }
    unsigned int i = sizeof(fundt_un);
    if(byte_amount == 0){ return; }
    if(byte_amount <= i){ i = byte_amount; } else { printf("The number of bytes is larger than there are bytes in the variable \n"); }
    i <<= 3; // bytes -> bits; (i << 3) is the same as  (i * (2**3)), or the same as (i * 8)
    unsigned int sign_bit_index = (i - 1) - 1;
    unsigned int exponent_end_index = 0;
    if(i == (sizeof(double) << 3)){ exponent_end_index = (i - 1) - 1 - 11; }
    else{ 
            if(i == (sizeof(float) << 3)){ exponent_end_index = (i - 1) - 1 - 7; } 
    }
    unsigned int custom_point = 16;
    unsigned int custom_point_from_the_end = (i - 1) - 5;
    while(i-- > 0){
        if((i == sign_bit_index) | (i == exponent_end_index) | (i == custom_point) | (i == custom_point_from_the_end)){
            printf(" ");
        }
        printf("%u", (x.ui64 >> i) & 1);
    }
}

// void print_binary_my_version(fundt_un x, dtype type){
//     long unsigned int mask = ~((~0ul) >> 1);
//     unsigned int sizeoftype = sizeof(fundt_un) << 3;
//     unsigned int sizeof_double_sign_exponent_bit_sum = 12;// aim: (conveniently)exhibit correct amount of bit in this datatype(argument)
//     for(int i = 0; i < sizeofall; ++i){
//         if((i == 1) | (i == sizeof_double_sign_exponent_bit_sum)){ printf(" "); }
//         printf("%u", !!(x.ui64 & mask));
//         mask >>= 1;
//     }
// }