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


void vec_creation_t(void){
    long unsigned int i, a, b;
    // char a = -1;
    // unsigned int i = (unsigned int)a + 1;
    // printf("%u", i);
    // printff("%u", (unsigned int)a + 1);// why does'nt work
    // CHAR TYPE
    char char_arr[MAX_UCHAR + 1];
    for(i = 0; i <= MAX_UCHAR; ++i){ char_arr[i] = i; }
    vecN char_vec = vector_creation(CHAR, MAX_UCHAR + 1, B1type_i_elements(char_arr));
    printf("type: CHAR\nerror: %d\nelements amount: %u\n", char_vec.v_error, char_vec.n);
    for(i = 0; i <= MAX_UCHAR; ++i){
        printf("element %u: %d\n", i, char_vec.elements.b1.i[i]);
    }
    // UCHAR TYPE
    unsigned char uchar_arr[MAX_UCHAR + 1];
    for(i = 0; i <= MAX_UCHAR; ++i){ uchar_arr[i] = i; }
    vecN uchar_vec = vector_creation(UCHAR, MAX_UCHAR + 1, B1type_ui_elements(uchar_arr));
    printf("type: UCHAR\nerror: %d\nelements amount: %u\n", uchar_vec.v_error, uchar_vec.n);    
    for(i = 0; i <= MAX_UCHAR; ++i){
        printf("element %u: %u\n", i,uchar_vec.elements.b1.ui[i]);
    }
    // INT TYPE
    int int_arr[MAX_UCHAR * 2];
    for(i = 0, a = MAX_INT, b = MIN_INT; i < 2 * MAX_UCHAR; ++i, --a){  if(i > MAX_UCHAR) int_arr[i] = b++; else int_arr[i] = a; }
    vecN int_vec = vector_creation(INT, MAX_UCHAR * 2, B4type_i_elements(int_arr));
    printf("type: INT\nerror: %d\nelements amount: %u\n", int_vec.v_error, int_vec.n);
    for(i = 0; i < 2 * MAX_UCHAR; ++i){
        if(i > MAX_UCHAR)
        printf("element %u: %i\n", i, int_vec.elements.b4.i[i]);
        else
        printf("element %u: %i\n", i, int_vec.elements.b4.i[i]);
    }
    // UINT TYPE
    unsigned int uint_arr[MAX_UCHAR];
    for(i = 0, a = MAX_UINT; i < MAX_UCHAR; ++i, --a){ uint_arr[i] = a; }
    vecN uint_vec = vector_creation(UINT, MAX_UCHAR, B4type_ui_elements(uint_arr));
    printf("type: UINT\nerror: %d\nelements amount: %u\n", uint_vec.v_error, uint_vec.n);
    for(i = 0; i < MAX_UCHAR; ++i){
        printf("element %u: %u\n", i, uint_vec.elements.b4.ui[i]);
    }
    // LUINT TYPE
    long unsigned int lui[MAX_UCHAR];
    for(i = 0, a = MAX_LUINT; i < MAX_UCHAR; ++i, --a){ lui[i] = a; }
    vecN lui_vec = vector_creation(LUINT, MAX_UCHAR, B8type_ui_elements(lui));
    printf("type: LUINT\nerror: %d\nelements amount: %u\n", lui_vec.v_error, lui_vec.n);
    for(i = 0; i < MAX_UCHAR; ++i){
        printf("element %u: %lu\n", i, lui_vec.elements.b8.ui[i]);
    }
    // LINT TYPE
    long int li[MAX_UCHAR * 2];
    for(i = 0, a = MAX_LINT, b = MIN_LINT; i < 2 * MAX_UCHAR; ++i, --a){  if(i > MAX_UCHAR) li[i] = b++; else li[i] = a; }
    vecN li_vec = vector_creation(LINT, MAX_UCHAR * 2, B8type_i_elements(li));
    printf("type: LINT\nerror: %d\nelements amount: %u\n", li_vec.v_error, li_vec.n);
    for(i = 0; i < 2 * MAX_UCHAR; ++i){
        if(i > MAX_UCHAR)
        printf("element %u: %li\n", i, li_vec.elements.b8.ui[i]);
        else
        printf("element %u: %li\n", i, li_vec.elements.b8.ui[i]);
    }
}