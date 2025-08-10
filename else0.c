long unsigned int else0(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}
