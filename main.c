#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <math.h>
#include <stdint.h>
#define headerfile 1

#include "user_defined_datatypes.c"
#include "constants.c"
#include "additional_functions.c"
#include "logical_functions_of_decision.c"
#include "bitwise_functions.c"
#include "safe_arithmetic_functions.c"
#include "new_vector_functions.c"
// #include "test.c"   

void vector_multiplication_test(void){
  unsigned int f_pos_inf_ui = 0b01111111100000000000000000000000, f_neg_inf_ui = 0b11111111100000000000000000000000, ui_f_pos_nan = 0b01111111110000100110000010000000, ui_f_neg_nan = 0b11111111110000100110000010000000, ui_max_fl = 0b01111111011111111111111111111111, ui_min_fl = 0b00000000100000000000000000000000;
    long unsigned int d_pos_inf_lui = 0b0111111111110000000000000000000000000000000000000000000000000000, d_neg_inf_lui = 0b1111111111110000000000000000000000000000000000000000000000000000, lui_d_pos_nan = 0b0111111111111000000000000010001000001100000011100000000000000000, lui_d_neg_nan = 0b1111111111111000000000000010001000001100000011100000000000000000, lui_max_d = 0b0111111111101111111111111111111111111111111111111111111111111111, lui_min_d = 0b0000000000010000000000000000000000000000000000000000000000000000;
    float f_pos_inf = *(float*)&f_pos_inf_ui, f_neg_inf = *(float*)&f_neg_inf_ui, f_pos_nan = *(float*)&ui_f_pos_nan, f_neg_nan = *(float*)&ui_f_neg_nan, f_max = *(float*)&ui_max_fl, f_min = *(float*)&ui_min_fl;
    double d_pos_inf = *(double*)&d_pos_inf_lui, d_neg_inf = *(double*)&d_neg_inf_lui, d_pos_nan = *(double*)&lui_d_pos_nan, d_neg_nan = *(double*)&lui_d_neg_nan, d_max = *(double*)&lui_max_d, d_min = *(double*)&lui_min_d;
    // vectors declaration
    vecN vec_int, vec_uint, vec_char, vec_uchar, vec_lint, vec_luint, vec_float, vec_double;
    vec_char = vector_creation(CHAR, 5, B1type_i_elements((char[]){0x80, 0x7f, -128, 0, -32}));
    vec_uchar = vector_creation(UCHAR, 5, B1type_ui_elements((unsigned char[]){0xff, 162, 0x7f, 23, 0}));
    vec_int = vector_creation(INT, 5, B4type_i_elements((int[]){0x80000000, 0x7fffffff, 23536, -23523, 0}));  
    vec_uint = vector_creation(UINT, 5, B4type_ui_elements((unsigned int[]){0xffffffff, 0x7fffffff, 612465136, 2572, 0}));
    vec_lint = vector_creation(LINT, 5, B8type_i_elements((long int[]){0x8000000000000000, 0x7fffffffffffffff, -2572725, -235236232, 0}));
    vec_luint = vector_creation(LUINT, 5, B8type_ui_elements((long unsigned int[]){0xffffffffffffffff, 0x7fffffffffffffff,14376325762, 24362436, 0}));
    vec_float = vector_creation(FLOAT, 11, B4type_f_elements((float[]){234.235, 235233.1, -2343232.222, -23362, 0, f_pos_inf, f_neg_inf, f_pos_nan, f_neg_nan, f_max, f_min}));
    vec_double = vector_creation(DOUBLE, 11, B8type_d_elements((double[]){23432.363464, 23425.23634, -987897.888, -239929, 0, d_pos_inf, d_neg_inf, d_pos_nan, d_neg_nan, d_max, d_min}));
    // show vectors's values
    int v = 0;
    while(v < vec_char.n){
      printf("\nvec_char.elements.b1.i[%d] = %d\n", v, vec_char.elements.b1.i[v]); 
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_uchar.elements.b1.ui[%d] = %u\n", v, vec_uchar.elements.b1.ui[v]);
      v++;  
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_int.elements.b4.i[%d] = %d\n", v, vec_int.elements.b4.i[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_uint.elements.b4.ui[%d] = %u\n", v, vec_uint.elements.b4.ui[v]);
      v++;
    }
    v = 0;
    while(v < vec_float.n){
      printf("\nvec_float.elements.b4.f[%d] = %f\n", v, vec_float.elements.b4.f[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_lint.elements.b8.i[%d] = %li\n", v, vec_lint.elements.b8.i[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_luint.elements.b8.ui[%d] = %lu\n", v, vec_luint.elements.b8.ui[v]);
      v++;
    }
    v = 0;
    while(v < vec_double.n){
      printf("\nvec_double.elements.b8.d[%d] = %f\n", v, vec_double.elements.b8.d[v]);
      v++;
    }
    vec_char = vector_multiplication(vec_char, vec_char);
    vec_uchar = vector_multiplication(vec_uchar, vec_uchar);
    vec_int = vector_multiplication(vec_int, vec_int);
    vec_uint = vector_multiplication(vec_uint, vec_uint);
    vec_lint = vector_multiplication(vec_lint, vec_lint);
    vec_luint = vector_multiplication(vec_luint, vec_luint);
    vec_float = vector_multiplication(vec_float, vec_float);
    vec_double = vector_multiplication(vec_double, vec_double);
    v = 0;
    
    while(v < vec_char.n){
      printf("\nvec_char.elements.b1.i[%d] = %d\n", v, vec_char.elements.b1.i[v]); 
      v++;
    }
    printf("error: %d\n\n\n", vec_char.v_error);
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_uchar.elements.b1.ui[%d] = %u\n", v, vec_uchar.elements.b1.ui[v]);
      v++;  
    }
    printf("error: %d\n\n\n", vec_uchar.v_error);
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_int.elements.b4.i[%d] = %d\n", v, vec_int.elements.b4.i[v]);
      v++;
    }
    printf("error: %d\n\n\n", vec_int.v_error);
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_uint.elements.b4.ui[%d] = %u\n", v, vec_uint.elements.b4.ui[v]);
      v++;
    }
    printf("error: %d\n\n\n", vec_uint.v_error);
    v = 0;
    while(v < vec_float.n){
      printf("\nvec_float.elements.b4.f[%d] = %f\n", v, vec_float.elements.b4.f[v]);
      v++;
    }
    printf("error: %d\n\n\n", vec_float.v_error);
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_lint.elements.b8.i[%d] = %li\n", v, vec_lint.elements.b8.i[v]);
      v++;
    }
    printf("error: %d\n\n\n", vec_lint.v_error);
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_luint.elements.b8.ui[%d] = %lu\n", v, vec_luint.elements.b8.ui[v]);
      v++;
    }
    printf("error: %d\n\n\n", vec_luint.v_error);
    v = 0;
    while(v < vec_double.n){
      printf("\nvec_double.elements.b8.d[%d] = %f\n", v, vec_double.elements.b8.d[v]);
      v++;
    }
    printf("error: %d\n\n\n", vec_double.v_error);
}


void small_test(void){
  long unsigned int lui_min_d = 0b0000000000010000000000000000000000000000000000000000000000000000;
  double d_min = *(double*)&lui_min_d;
  error err = NO_ERROR;
  printf("\nprod = %f\n", safe_double_multiplication_with_rounding((dbits){ .d = d_min}, (dbits){ .d = d_min}, &err));
}


char main(){ 
  small_test();
  return 0;    
}


/*
int v = 0;
while(v < a.n){
    printf("\na.elements.b4.f[%d] = %f\n", v, a.elements.b4.f[v]);
    v++;
}
v = 0;
while(v < a.n){
    printf("\nb.elements.b4.f[%d] = %f\n", v, b.elements.b4.f[v]);
    v++;
}
*/