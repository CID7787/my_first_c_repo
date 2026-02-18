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