#ifndef headerfile
    #include <stdint.h>
    #include <stdlib.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
    #include "new_vector_functions.c"
#endif

void vector_creation_test(void){
    // variable declaration 
    unsigned int f_pos_inf_ui = 0b01111111100000000000000000000000, f_neg_inf_ui = 0b01111111100000000000000000000000, f_pos_nan_w_h_m_b = 0b01111111110000100110000010000000, f_neg_nan_w_h_m_b = 0b11111111110000100110000010000000, f_pos_nan_w_o_m_b = 0b01111111100000100110000010000000, f_neg_nan_w_o_m_b = 0b11111111100000100110000010000000;
    long unsigned int d_pos_inf_lui = 0b0111111111110000000000000000000000000000000000000000000000000000, d_neg_inf_lui = 0b1111111111110000000000000000000000000000000000000000000000000000, d_pos_nan_w_h_m_b = 0b0111111111111000000000000010001000001100000011100000000000000000, d_neg_nan_w_h_m_b = 0b1111111111111000000000000010001000001100000011100000000000000000, d_pos_nan_w_o_m_b = 0b0111111111110000000000000010001000001100000011100000000000000000, d_neg_nan_w_o_m_b = 0b1111111111110000000000000010001000001100000011100000000000000000;
    float f_pos_inf = f_pos_inf_ui, f_neg_inf = f_neg_inf_ui, f_pos_nan_with_high_man_bit = f_pos_nan_w_h_m_b, f_neg_nan_with_high_man_bit = f_neg_nan_w_h_m_b, f_pos_nan_w_o_high_man_bit = f_pos_nan_w_o_m_b, f_neg_nan_w_o_high_man_bit = f_neg_nan_w_o_m_b;
    double d_pos_inf = d_pos_inf_lui, d_neg_inf = d_neg_inf_lui, d_pos_nan_with_high_man_bit = d_pos_nan_w_h_m_b, d_neg_nan_with_high_man_bit = d_neg_nan_w_h_m_b, d_pos_nan_w_o_high_man_bit = d_pos_nan_w_o_m_b, d_neg_nan_w_o_high_man_bit = d_neg_nan_w_o_m_b;
    // vectors declaration
    vecN vec_int, vec_uint, vec_char, vec_uchar, vec_lint, vec_luint, vec_float, vec_double;
    vec_char = vector_creation(CHAR, 5, B1type_i_elements((char[]){2, 35, -324, 0, -32}));
    vec_uchar = vector_creation(UCHAR, 5, B1type_ui_elements((unsigned char[]){2,652,62,23, 0}));
    vec_int = vector_creation(INT, 5, B4type_i_elements((int[]){251235, 346143, -23536, -23523, 0}));
    vec_uint = vector_creation(UINT, 5, B4type_ui_elements((unsigned int[]){1,23523, 612465136, 2572, 0}));
    vec_lint = vector_creation(LINT, 5, B8type_i_elements((long int[]){234614, 7357357, -2572725, -235236232, 0}));
    vec_luint = vector_creation(LUINT, 5, B8type_ui_elements((long unsigned int[]){235236, 2346263,14376325762, 24362436, 0}));
    vec_float = vector_creation(FLOAT, 11, B4type_f_elements((float[]){234.235, 235233.1, -2343232.222, -23362, 0, }));
    vec_double = vector_creation(DOUBLE, 11, B8type_d_elements((double[]){23432.363464, 23425.23634, -987897.888, -239929, 0}));
    // show vectors's values
    int v = 0;
    while(v < vec_char.n){
      printf("\nvec_char.elements.b1.f[%d] = %d\n", v, vec_char.elements.b1.i[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_uchar.elements.b1.f[%d] = %u\n", v, vec_uchar.elements.b1.i[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_int.elements.b4.f[%d] = %d\n", v, vec_int.elements.b4.i[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_uint.elements.b4.f[%d] = %u\n", v, vec_uint.elements.b4.ui[v]);
      v++;
    }
    v = 0;
    while(v < vec_float.n){
      printf("\nvec_float.elements.b4.f[%d] = %f\n", v, vec_float.elements.b4.f[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_lint.elements.b4.f[%d] = %li\n", v, vec_lint.elements.b8.i[v]);
      v++;
    }
    v = 0;
    while(v < vec_char.n){
      printf("\nvec_luint.elements.b4.f[%d] = %lu\n", v, vec_luint.elements.b8.ui[v]);
      v++;
    }
    v = 0;
    while(v < vec_double.n){
      printf("\nvec_double.elements.b4.f[%d] = %f\n", v, vec_double.elements.b8.d[v]);
      v++;
    }
    // function call
    // vecN d = vector_creation(a.type, a.n, a.elements);
}