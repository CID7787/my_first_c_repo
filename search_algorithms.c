#ifndef headerfile
  #include "user_defined_datatypes.c"
#endif
unsigned int binary_search_array_element_index(int *array, unsigned int size, int value){ //10
  if(!array){ return 0; }
  if (value > array[size - 1]){return -1;}
  
  unsigned int f = 0, start = 0;
  if (value < array[start]){return -2;}
  
  unsigned int center = start + integer_division_by_2(size - start); //  center = 0 + (2 - 0) / 2 = 0 + 2/2 = 1
  while (1) {
    if(array[center] > value) {
       size = center; // size = 1
       center = start + integer_division_by_2(size - start); // 0 + (1 - 0) / 2 == 0
    }
    if (array[center] < value) {
      start = center; //  start = 0
      center = start + integer_division_by_2(size - start); // 0
    }
    if (array[center] == value) { break; }
    if ((start + 1) >= size) {
      if (array[start] == value) { return start; }
      if (array[size] == value) { return size; } // TODO: potential error, but don't know which one
      return -1;
    }
    ++f;
    if (f > size) { return -1; }
  }
 return center;
}

char greater_than(void* a, int offsetA, void* b, int offsetB, char type){
  if ('i' == type){ return ( ((int*)a)[offsetA] > ((int*)b)[offsetB] ); }
  if ('f' == type){ return ( ((float*)a)[offsetA] > ((float*)b)[offsetB] ); }
  if ('u' == type){ return ( ((unsigned int*)a)[offsetA] > ((unsigned int*)b)[offsetB] );}
  if ('d' == type){ return ( ((double*)a)[offsetA] > ((double*)b)[offsetB]);}
  if ('c' == type){ return ( ((char*)a)[offsetA] > ((char*)b)[offsetB] );}
  return 2;
}
char less_than(void* a, int offsetA, void* b, int offsetB, char type){
  if ('i' == type){ return ( ((int*)a)[offsetA] < ((int*)b)[offsetB] ); }
  if ('f' == type){ return ( ((float*)a)[offsetA] < ((float*)b)[offsetB] ); }
  if ('u' == type){ return ( ((unsigned int*)a)[offsetA] < ((unsigned int*)b)[offsetB] ); }
  if ('d' == type){ return ( ((double*)a)[offsetA] < ((double*)b)[offsetB]); }
  if ('c' == type){ return ( ((char*)a)[offsetA] < ((char*)b)[offsetB] ); }
  return 2;
}
char equals(void* a, int offsetA, void* b, int offsetB, char type){
  if ('i' == type){ return ( ((int*)a)[offsetA] == ((int*)b)[offsetB] ); }
  if ('f' == type){ return ( ((float*)a)[offsetA] == ((float*)b)[offsetB] ); }
  if ('u' == type){ return ( ((unsigned int*)a)[offsetA] == ((unsigned int*)b)[offsetB] );}
  if ('d' == type){ return ( ((double*)a)[offsetA] == ((double*)b)[offsetB] );}
  if ('c' == type){ return ( ((char*)a)[offsetA] == ((char*)b)[offsetB] );}
  return 2;
}

int array_binary_search(void* array, void* x, unsigned int size, char type){
    int min = 0;
    int middle = (size - 1) / 2;
    while(1){
      if (greater_than(x, 0, array, middle, type)) {min = middle;}
      if (less_than(x, 0, array, middle, type)) {size = middle;}
      middle = min + ((size + min) / 2);
      if (equals(x, 0, array, middle, type))  {return middle;}
    }
  return -1;
}


double newton_method(double (*function)(double, error*), double (*derivative)(double, error*), double x0, double precision, error* err){
  if(!err) { return x0; }
  double x = x0;
  while( ((*function)(x, err) <= precision) | ((*function)(x, err) >= precision) ){
      if(*err != NO_ERROR){ return x; }
      double dx0 = (*derivative)(x0, err); if(*err != NO_ERROR) { return x; }
      double f0 = (*function)(x0, err); if (*err) { return x; }
      x = x0 - safe_double_division(f0, dx0, err); // OVERFLOW, UNDERFLOW, DIVISION BY ZERO
      if(*err != NO_ERROR){ return x; }
  }
  return x;
}
