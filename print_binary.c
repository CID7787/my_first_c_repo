#ifndef headerfile
    #include "user_defined_datatypes.c"
#endif

// Example usage:
const all IEEE754_FLOAT_MASK = { .li =   0b10000001000000000000000000000000};
const all IEEE754_DOUBLE_MASK = { .lui = 0b100000000001000000000000000000000000000000000000000000000000000};
const all LONG_INT_SIGN_MASK = { .lui =  0b100000000000000000000000000000000000000000000000000000000000000};

// print_binary_actual_best_version_with_one_variable((all){ .d = 10.25}, (all){.lui = 0}, IEEE754_DOUBLE_MASK);

void print_binary_actual_best_version_with_one_variable(all x, all stop_where_bit_is_1, all whitespace_mask){
    unsigned int i = sizeof(all) << 3;
    while(i-- > 0){
        if ((stop_where_bit_is_1.lui >> i) & 1) { return; }
        if ((whitespace_mask.lui >> i) & 1) { printf(" "); }
        printf("%u", (x.lui >> i) & 1);
    }
}

void print_binary_my_first_best_version_with_one_variable(all x, unsigned int byte_amount, all mask){
    if(!byte_amount){ return; }
    unsigned int i = sizeof(all);
    if(byte_amount <= i){ i = byte_amount; } else { printf("The number of bytes is larger than there are bytes in the variable \n"); }
    i <<= 3;
    while(i-- > 0){
        if ((mask.lui >> i) & 1) { printf(" "); }
        printf("%u", (x.lui >> i) & 1);
    }
}

void print_binary_with_arrays(all x, unsigned int byte_amount, unsigned char* whitespace_after_bit_indexes, unsigned int array_size){// whitespace indexes have absolute index starting from leftmost bit
    if(!whitespace_after_bit_indexes){ return; }
    if(!byte_amount){ return; }
    unsigned int i = sizeof(all);
    if(byte_amount <= i){ i = byte_amount; } else { printf("The number of bytes is larger than there are bytes in the variable \n"); }
    i <<= 3;
    while(i-- > 0){
        unsigned int index = 0;
        while(index < array_size){
            if(i == whitespace_after_bit_indexes[index++]){ printf(" "); break; }
        }
        printf("%u", (x.lui >> i) & 1);
    }
}
void print_binary_first_amirako_version(all x){// x = 0
    int i = 64;
    while(i-- > 0){
        if((i == 62) | (i == 51)){ printf(" "); }
        printf("%u", (x.lui >> i) & 1);
    }
}

void print_binary_new_version1(all x, unsigned int byte_amount, unsigned int *spaces){// TODO: rewrite it as function that accepts array that contain information about at what postion to put spaces
    if(!spaces){ return; }
    unsigned int i = sizeof(all);
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
        printf("%u", (x.lui >> i) & 1);
    }
}

// void print_binary_my_version(all x, dtype type){
//     long unsigned int mask = ~((~0ul) >> 1);
//     unsigned int sizeoftype = sizeof(all) << 3;
//     unsigned int sizeof_double_sign_exponent_bit_sum = 12;// aim: (conveniently)exhibit correct amount of bit in this datatype(argument)
//     for(int i = 0; i < sizeofall; ++i){
//         if((i == 1) | (i == sizeof_double_sign_exponent_bit_sum)){ printf(" "); }
//         printf("%u", !!(x.lui & mask));
//         mask >>= 1;
//     }
// }