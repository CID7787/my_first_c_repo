long unsigned int else0_array(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}

int else0(int cond, int x){
    return x & (~0 + !cond);
}



long unsigned int ternary(unsigned char condition, long unsigned int true_value, long unsigned int false_value){
    long unsigned int array[2] = {true_value, false_value};
    return array[!condition];
}

int trueXfalse0(unsigned int cond, int x){
    int any = (!cond) << (sizeof(int) + 1);
    return x >> any;// if(cond){return x;} else{ return 0;}
}
  
void ifnot(void* a, void* b){
  int* array[2] = {(int*)a, (int*)b};
  *(array[0]) = array[!(*(array[0]))];
}