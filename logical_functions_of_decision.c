long unsigned int else0_array(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}

long unsigned int else0(long unsigned int condition, long unsigned int x){
    return x & (~0ul + !condition);
    // return -!!condition & x;
}


long unsigned int ternary(unsigned char condition, long unsigned int true_value, long unsigned int false_value){
    long unsigned int array[2] = {true_value, false_value};
    return array[!condition];
}
