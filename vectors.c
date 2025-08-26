#ifndef headerfile      
  #include <stdio.h>
  #include <stdlib.h>
  #include "user_defined_datatypes.c"
  #include "constants.c"
  #include "logical_functions_of_decision.c"
  #include "bitwise_functions.c"
  #include "safe_arithmetic_functions.c"
#endif

//cindex[1] == 1   <==>     *(cindex + 1*sizeof(char))
// operator first of all implements from right side always , like 3[a] 2[=] 1[(1+2)];
//    char* n = (char*){'a', 'b', 'c', 0}; // "abc"
// in C    const char[] <-"abc" постоянный статический массив
// enum -- enumeration
/* если в теле цикла не вставить действие и только условие то компилятор по умолчанию оптимизирует и удаляет эту часть кода
    while (f1(arr, i++, size)){ } // this will be deleted, unless you disable compiler optimizations for loops
    while (f1(arr, i, size)){ i++; } // this will NOT be deleted
*/

int amount_of_bytes_in_this_datatype(datatype t){
  switch (t) {  
    case INT: return sizeof(int);
    case CHAR: return sizeof(char);
    case FLOAT: return sizeof(float);
    case DOUBLE: return sizeof(double);
    case UINT: return sizeof(unsigned int);
    default: return sizeof(datatype);
  }
}


int element_is_nonzero(datapointer array, int iterations, int my_datasize){// {12,7,2}, 1, 4
  int last_byte_that_need_to_check = iterations + my_datasize;
  char there_are_1s = 0;
  while(iterations < last_byte_that_need_to_check){
    there_are_1s = there_are_1s || (array.c)[iterations];
    ++iterations;
  }
  return there_are_1s;
}

vecN vector_creation(datatype type, unsigned int n, datapointer array_elements){
  int size = amount_of_bytes_in_this_datatype(type);
  int i = 0;
  if (!n){
    while (element_is_nonzero(array_elements, i, size)){ i++; } // this will NOT be deleted
    n = i;
  }
  vecN vector_result = {type, n, malloc(size * n)};
  i = n;
  while(i--) {
    switch (type){
      case INT:     vector_result.elements.i[i] = array_elements.i[i]; break;
      case CHAR:    vector_result.elements.c[i] = array_elements.c[i]; break;
      case FLOAT:   vector_result.elements.f[i] = array_elements.f[i]; break;
      case DOUBLE:  vector_result.elements.d[i] = array_elements.d[i]; break;
      case UINT:    vector_result.elements.ui[i] = array_elements.ui[i]; break;
    }
  }
  return vector_result;
}

vecN vector_creation_noswitch(datatype type, unsigned int n, datapointer arr){
  int size = amount_of_bytes_in_this_datatype(type);
  int i = 0;
  if (!n){
    while (element_is_nonzero(arr, i, size)){ i++; } // this will NOT be deleted
    n = i;
  }  
  vecN result = {type, n, malloc(size * n)};// ?
  i = 0; 
  while(i < n*size) { result.elements.c[i++] = arr.c[i]; }
  return result;
}

void* vector_creation_old(datatype type, unsigned int n, void* arr){
  int size = amount_of_bytes_in_this_datatype(type);
  int i = 0;
  if (!n){
    while (element_is_nonzero((datapointer)arr, i, size)){ i++; } // this will NOT be deleted
    n = i;
  }
  void* result = malloc(1 + sizeof(n) + (size*n));
  ((char*)result)[0] = type;
  i = 0;
  while(i < sizeof(n)){ ((char*)result)[++i] = ((char*)&n)[i]; }
  int start_of_elements = 1 + sizeof(n);
  i = 0;
  while (i < size * n){ ((char*)result)[start_of_elements++] = ((char*)arr)[i++]; }
  return result;
}

vecN vector_negation(vecN v){
  int size = amount_of_bytes_in_this_datatype(v.type);
  vecN result = {v.type, v.n, malloc(size * v.n) };
  int i = v.n;
  while(i--) {
    switch (v.type){
      case INT:     result.elements.i[i] = -v.elements.i[i]; break;
      case CHAR:    result.elements.c[i] = -v.elements.c[i]; break;
      case FLOAT:   result.elements.f[i] = -v.elements.f[i]; break;
      case DOUBLE:  result.elements.d[i] = -v.elements.d[i]; break;
      case UINT:    result.elements.ui[i] = -v.elements.ui[i]; break;
    }
  }
  return result;
}


void vector_negation_in_place_old(void* v){
  int size = amount_of_bytes_in_this_datatype( ( (char*)v )[0]);
  unsigned int n = ((unsigned int*)(((char*)v)[1]))[0];//  ? - NOT RIGHT , FIX!s 
  int i = 1 + sizeof(n); 
  int i2 = i + n;
  while(i < i2) {
    switch( (datatype)(((char*)v)[0]) ){
      case INT:
        ( (int*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i] = -(( (int*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i]);
        break;
      case CHAR:
        // (((char*)v)[1 + sizeof(unsigned int)])[i] = -(( (char*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i]);
        break;
      case FLOAT:
        ( (float*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i] = -(( (float*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i]);
        break;
      case DOUBLE:
        ( (double*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i] = -(( (double*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i]);
        break;
      case UINT:
        ( (unsigned int*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i] = -(( (unsigned int*)( ((char*)v)[1 + sizeof(unsigned int)]) )[i]);
        break;
    }
  }
  return;
}


void vector_negation_in_place(vecN v) {
  unsigned int i = v.n;
  while(i--) {
    switch (v.type){
      case INT:     v.elements.i[i] = -v.elements.i[i]; break;
      case CHAR:    v.elements.c[i] = -v.elements.c[i]; break;
      case FLOAT:   v.elements.f[i] = -v.elements.f[i]; break;
      case DOUBLE:  v.elements.d[i] = -v.elements.d[i]; break;
      case UINT:    v.elements.ui[i] = -v.elements.ui[i]; break;
    }
  }
 return;
}

vecN vector_addition(vecN v1, vecN v2){
  int size = amount_of_bytes_in_this_datatype(v1.type);
  vecN v = {v1.type, v1.n, malloc(size * v1.n)};
  unsigned int i = v1.n;

  char flag = 0;
  unsigned int instr = (~0U >> 1) ^ (1 << 23);
  float float_max = *((float*)&instr);
  instr = (~0ul >> 1) ^ (1ul << 52);
  double double_max = *( (double*)&instr ); 

  while(i--) {
    
    switch (v1.type){
      case INT:
        v.elements.i[i] = v1.elements.i[i] + v2.elements.i[i];
        v.v_error = (v.elements.i[i] < 0) & (v1.elements.i[i] >= 0);
        v.v_error = 1 && ( MIN_INT & (v1.elements.i[i] ^ v2.elements.i[i]) );
        break;
      case CHAR:    
        v.elements.c[i] = v1.elements.c[i] + v2.elements.c[i]; 
        v.v_error = (v.elements.c[i] < 0) & (v1.elements.c[i] >= 0);
        v.v_error = (v.v_error + !v.v_error) && ( MIN_INT & (v1.elements.c[i] ^ v2.elements.c[i]) );                
        
        break;
      case FLOAT:   
        v1.elements.f[i] = (v1.elements.f[i] < 0) * (-v1.elements.f[i]) + (!(v1.elements.f[i] < 0)) * v1.elements.f[i];// here if evaluation of variable was negative number, then I will set it to positive, otherwise it will be the same  
        v2.elements.f[i] = (v2.elements.f[i] < 0) * (-v2.elements.f[i]) + (!(v2.elements.f[i] < 0)) * v2.elements.f[i];// here if evaluation of variable was negative number, then I will set it to positive, otherwise it will be the same          
        flag = ( (v1.elements.f[i] < 0) && (v2.elements.f[i] < 0) );
        v.v_error = ( (v2.elements.f[i] >= 0) && ( v1.elements.f[i] > (float_max - v2.elements.f[i]) ) ) * POSITIVE_OVERFLOW;// the result of this string must be either 0(it means ALL_GOOD) or 1(it means we have OVERFLOW) 
        
        v.elements.i[i] = v1.elements.i[i] + v2.elements.i[i]; 
        
        v.elements.f[i] = flag * (-v.elements.f[i]) + (!flag) * v.elements.f[i];  
        flag = 0; 
        break;
      case DOUBLE:  
        v1.elements.d[i] = (v1.elements.d[i] < 0) * (-v1.elements.d[i]) + (!(v1.elements.d[i] < 0)) * v1.elements.d[i];// here if evaluation of variable was negative number, then I will set it to positive, otherwise it will be the same  
        v2.elements.d[i] = (v2.elements.d[i] < 0) * (-v2.elements.d[i]) + (!(v2.elements.d[i] < 0)) * v2.elements.d[i];// here if evaluation of variable was negative number, then I will set it to positive, otherwise it will be the same          
        flag = ( (v1.elements.d[i] < 0) && (v2.elements.d[i] < 0) );
        v.v_error = ( (v2.elements.d[i] >= 0) && ( v1.elements.d[i] > (double_max - v2.elements.d[i]) ) ) * POSITIVE_OVERFLOW;// the result of this string must be either 0(it means ALL_GOOD) or 1(it means we have OVERFLOW) 
        
        v.elements.i[i] = v1.elements.d[i] + v2.elements.d[i]; 
        
        v.elements.i[i] = flag * (-v.elements.d[i]) + (!flag) * v.elements.d[i];  
        flag = 0; 
        break;
      case UINT:
        v.v_error = ( (v2.elements.ui[i] >= 0) && ( v1.elements.ui[i] > (MAX_UINT - v2.elements.ui[i]) ) ) * POSITIVE_OVERFLOW;// the result of this string must be either 0(it means ALL_GOOD) or 1(it means we have OVERFLOW) 
        v.elements.ui[i] = v1.elements.ui[i] + v2.elements.ui[i]; 
        break;
    }
  }
  return v;
}

void vector_addition_in_place(vecN v1, vecN v2){
  unsigned int i = v1.n;
  while(i--) {
    switch (v1.type){
      case INT:     v1.elements.i[i] = v1.elements.i[i] + v2.elements.i[i]; break;
      case CHAR:    v1.elements.c[i] = v1.elements.c[i] + v2.elements.c[i]; break;
      case FLOAT:   v1.elements.f[i] = v1.elements.f[i] + v2.elements.f[i]; break;
      case DOUBLE:  v1.elements.d[i] = v1.elements.d[i] + v2.elements.d[i]; break;
      case UINT:    v1.elements.ui[i] = v1.elements.ui[i] + v2.elements.ui[i]; break;
    }
  }
  return ;
}
  
vecN vector_multiplication(vecN v1, vecN v2){
  int size = amount_of_bytes_in_this_datatype(v1.type);
  vecN v = {v1.type, v1.n, malloc(size * v1.n)};
  unsigned int i = v1.n;
  while(i--) {
    switch (v1.type){
      case INT:     v.elements.i[i]  =  v1.elements.i[i] * v2.elements.i[i]; break;
      case CHAR:    v.elements.c[i]  =  v1.elements.c[i] * v2.elements.c[i]; break;
      case FLOAT:   v.elements.f[i]  =  v1.elements.f[i] * v2.elements.f[i]; break;
      case DOUBLE:  v.elements.d[i]  =  v1.elements.d[i] * v2.elements.d[i]; break;
      case UINT:    v.elements.ui[i] = v1.elements.ui[i] * v2.elements.ui[i]; break;
    }
  }
  return v;
}

void vector_multiplication_in_place(vecN v1, vecN v2){
  unsigned int i = v1.n;
  while(i--) {
    switch (v1.type){
      case INT:     v1.elements.i[i] = v1.elements.i[i] * v2.elements.i[i]; break;
      case CHAR:    v1.elements.c[i] = v1.elements.c[i] * v2.elements.c[i]; break;
      case FLOAT:   v1.elements.f[i] = v1.elements.f[i] * v2.elements.f[i]; break;
      case DOUBLE:  v1.elements.d[i] = v1.elements.d[i] * v2.elements.d[i]; break;
      case UINT:    v1.elements.ui[i] = v1.elements.ui[i] * v2.elements.ui[i]; break;    
    }
  }
  return ; 
}

int exp_complement_for_int(int base, int exponent, error* v_error){ //To_Do
  int sign = ((exponent << 1 ) >> 1) ^ exponent; // 0
  if(!exponent){
    *v_error = (!base) * UNDEFINED_BEHAVIOR;
    return 1; }
  int result = base;
  exponent = !sign * exponent + !!sign * (-exponent); // condition * result + condition * result + condition * result
  while(--exponent){
    result = result * base;
  }
  result = !!sign / result + !sign * result;
  return result;
}
  
unsigned exp_complement_for_uint(unsigned int base, unsigned int exponent){// 2,5
  if(!exponent){return 1;}
  unsigned int result = base;
  while(--exponent){
    result = result * base;
  }
  return result;
}

int convert_from_offsetB_to_twosCml(int arg){
  int var = 1 << 31;
  return -(var | arg);
}

double n_power_exponentiation(double var, unsigned int exp){
  double result = 1.0;
  while(exp--){
    result = (double)result * var; 
  }
  return result;
}

unsigned int check_if_number_is_even(unsigned int arg){
  return !(arg & 1);
}

double root(double base,int power){
  long  int var = 1L << 63 ;
  long  int* ptr = (long int*)&base;
  long int sign = (*ptr) & var;// this variable contain sign of my base
  base = (double)( (*ptr) & (~var) );// here I set the most sign bit of my base to 0
  double min = -base;
  double max = base;
  double precision = 0.01;
  double result = min + ( (min + max) / 2 );
  char flag = 1;
  while(flag){
     if(n_power_exponentiation(result, power) < base){ min = result; }
     if(n_power_exponentiation(result, power) > base){ max = result; }
    result = min + ( (min + max) / 2 );
     if ( ( n_power_exponentiation(result, power) < ( base + precision ) )
       && ( n_power_exponentiation(result, power) > (base - precision) ) ){ flag = 0; }
  }
  ptr = (long int*)&result;
  result = (double)( (*ptr) & (~var) );// here i I set the most signed bit of result to 0
  result = (double)((*ptr) | sign);
  return result;
}

double base_to_the_power_of_exponent_fractional_part(double base, double unsigned_initial_exp_fraction){  double result = 1.0;
  int exponent_exponent = ( ( (int*)(&unsigned_initial_exp_fraction) )[1] ) >> 20;
  long unsigned int mask = ((~0Ul) >> 52) << 52;
  long unsigned int exponent_mantis =( ( (long unsigned int*)(&unsigned_initial_exp_fraction) )[0] ) & mask;
  while(exponent_mantis--){
    result = result * base;
  }
  exponent_exponent = convert_from_offsetB_to_twosCmp(exponent_exponent);
  result = root(result, exponent_exponent);
  return result;
}
/*
when you give to this function base that must be powered to floating point less than 1 exponent and if it is even,
  then with considering next cases you will do next steps:
if base is a negative number, this number will first be turned to positive
  then when the root of this base will be found and there we will definitely get 2 results: 1 positive and 1 negative, 
  and as answer we will get negative because we can't use or represent imaginary numbers in computer memory 
  therefore we will use this special case for cases
if we wanna get negative result for even negative exponent,
or obviously if base was positive we will get positive result (to get more information about it start reading from -249- string) 
*/
double exp_complement_for_double(double base, double exponent){
  long  int exp_significant = exponent;
  double exp_fraction = exponent - exp_significant;
  double base_powered_to_exp_fractional_power = base_to_the_power_of_exponent_fractional_part(base, exp_fraction);
  if(exp_significant < 0){ base_powered_to_exp_fractional_power = 1 / base_powered_to_exp_fractional_power; }
  long double result = 1.0;
  while(exp_significant){
    result = result * base;
     if (exp_significant > 0){--exp_significant;}
      else{++exp_significant; result = 1 / result;}
  }
  return (result * base_powered_to_exp_fractional_power);
}
    
vecN vector_exponentiation_real(vecN v1, vecN v2){ // here are some rules with double and floating point exponentiation, that you should follow(to get more information about it go to -299- string)
  int size = amount_of_bytes_in_this_datatype(v1.type);
  vecN v = (vecN){v1.type, v1.n, malloc(size * v1.n)}; // ?
  unsigned int i = v1.n;
  while(i--) {
    switch (v1.type){
      case INT:
        // v.elements.i[i] = exp_complement_for_int(v1.elements.i[i], v2.elements.i[i]);
        break;
      case CHAR:
        // v.elements.c[i] = (char)exp_complement_for_int(v1.elements.c[i], v2.elements.c[i]);
        break;
      case FLOAT:
        v.elements.f[i] = (float)exp_complement_for_double((double)v1.elements.d[i], (double)v2.elements.d[i]);
        break;
      case DOUBLE:
        v.elements.d[i] = exp_complement_for_double(v1.elements.d[i], v2.elements.d[i]);
        break;
      case UINT:
        v.elements.ui[i] = exp_complement_for_uint(v1.elements.ui[i], v2.elements.ui[i]);
        break;
    }
  }  
  return v; 
}
/* the reason why next function won't work with float or double type variable 
is that exponentiation to real number power is function inversed to exponentiation - root(radical)
and function will return  1.1
*/
vecN vector_exponentiation_int(vecN v1, vecN v2){ // here is one case, when function gives double or float type vector type it will give you mistake(1), if you wanna know more about it forward to string № 371
  if ((v2.type == DOUBLE) || (v2.type == FLOAT)) {
    vecN have_v_error;
    have_v_error.v_error = ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER;
    return have_v_error; // @TODO: fix with proper v_error types inside a struct
  }
  
  int size = amount_of_bytes_in_this_datatype(v1.type);
  vecN v = (vecN){
    .type = v1.type,
    .n = v1.n,
    .elements = malloc(size * v1.n),
    .v_error = NO_ERROR
  }; 
  unsigned int i = v1.n;
  while(i--) {
    switch (v1.type){
      // @TODO: what if we have different data types?
      case INT:
        v.elements.i[i] = exp_complement_for_int(v1.elements.i[i], v2.elements.i[i], &v.v_error);
        break;
      case CHAR:
        v.elements.c[i] = (char)exp_complement_for_int(v1.elements.c[i], v2.elements.c[i], &v.v_error); // @TODO: what do we do on overflow?
        break;
      case UINT:
        v.elements.ui[i] = exp_complement_for_uint(v1.elements.ui[i], v2.elements.ui[i]);
        break;
    }
  }
  return v; 
}

vecN vectors_multiplication_via_dot_product(vecN v1, vecN v2){
  if(v1.type != v2.type){ return v1; }
  vecN var = vector_multiplication(v1, v2);
  // free(var.elements);
  // var.elements = malloc(...);
  // long unsigned int var1 = 0;
  // long unsigned int *ptr = &var1;
  vecN result;
  result.type = v1.type;
  while(v1.n --){
    switch(var.type){
      case INT:   { result.elements.i[0]  = safe_int_addition(result.elements.i[0] , result.elements.i[v1.n] , &result.v_error); } break;
      case CHAR:  { result.elements.c[0]  = safe_char_addition(result.elements.c[0] , result.elements.c[v1.n] , &result.v_error); } break;
      case FLOAT: { result.elements.f[0]  = safe_float_addition((fbits){ .f = result.elements.f[0] } , (fbits){ .f = result.elements.f[v1.n] } , &result.v_error); } break;
      case DOUBLE:{ result.elements.d[0]  = safe_double_addition((dbits){ .d = result.elements.d[0] }, (dbits){ .d = result.elements.d[v1.n] }, &result.v_error); } break;
      case UINT:  { result.elements.ui[0] = safe_uint_addition(result.elements.ui[0], result.elements.ui[v1.n], &result.v_error); } break;
    }
  }
 return result; 
}


/*

[
  [1, 2],
  [4, 5],
  [7, 8],
]

{
  vector_creation(type, 0, {1, 2}),
  vector_creation(type, 0, {4, 5}),
  vector_creation(type, 0, {7, 8}),
}

M = [vR0, vR1, vR2];
{
  vector_creation(type, 0, {1, 4, 7}),
  vector_creation(type, 0, {2, 5, 8}),
}

M = [vC0, vC1];

take_element_from_matrix(M, 0, 1);

struct ListElement {
  int value;
  ListElement* nextElement;
  ListElement* prevElement;
}

[ListElement, ListElement, ...]


    |
   /\
  | |
 /\ c
a b
*/

// https://excalidraw.com/#room=129ea2d94a216f3a200a,_UR0qR8RmMbQttVFzzumAw3