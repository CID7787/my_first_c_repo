long unsigned int else0(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}

long unsigned int ternary_function(unsigned char condition, long unsigned int true_value, long unsigned int false_value){
    long unsigned int array[2] = {true_value, false_value};
    return array[!condition];
}
