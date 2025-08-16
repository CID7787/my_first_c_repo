long unsigned int else0(unsigned int cond, int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
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
    int* c = (int*)a; 
    int* d = (int*)b;
    //    if(!(*c)){ *c = *d; }    
    *c = ternary(*c, *d, *c);
  }