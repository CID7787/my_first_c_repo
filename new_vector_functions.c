#ifndef headerfile
    #include <stdio.h>
    #include <stdlib.h>
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
    #include "safe_arithmetic_functions.c"
#endif

unsigned int amount_of_type_bytes(datatype t){
    switch(t) {  
      case CHAR: return sizeof(char); break;
      case UCHAR: return sizeof(unsigned char); break;
      case INT: return sizeof(int); break;
      case UINT: return sizeof(unsigned int); break;
      case FLOAT: return sizeof(float); break;
      case LINT: return sizeof(long int); break;
      case LUINT: return sizeof(long unsigned int); break;
      case DOUBLE: return sizeof(double); break;
      case FBITS: return sizeof(fbits); break;
      case DBITS: return sizeof(dbits); break;
      default: return sizeof(datatype);
    }
}

vecN vector_creation(datatype type, unsigned int n, datapointer elements){
    vecN r = {type, n, malloc(n * amount_of_type_bytes(type))};
    while(n--){
        switch(type){
            case CHAR:  r.elements.c[n]  = elements.c[n]; break;
            case UCHAR: r.elements.uc[n] = elements.uc[n]; break;
            case INT:   r.elements.i[n]  = elements.i[n]; break;
            case UINT:  r.elements.u[n]  = elements.u[n]; break;
            case FLOAT: r.elements.f[n]  = elements.f[n]; break;
            case LINT:  r.elements.li[n] = elements.li[n]; break;
            case LUINT: r.elements.lui[n]= elements.lui[n]; break;
            case DOUBLE:r.elements.d[n]  = elements.d[n]; break;
            case FBITS: r.elements.fb[n] = elements.fb[n]; break;
            case DBITS: r.elements.db[n] = elements.db[n]; break;
        }
    }
    return r;
}

vecN vector_negation(vecN a){
    unsigned int iter = a.n;
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), a.error};
    while(iter--){
        switch(r.type){
            case CHAR: r.error = ternary(a.elements.c[iter] == MIN_CHAR, POSITIVE_OVERFLOW, a.error); r.elements.c[iter] = -a.elements.c[iter]; break;
            case INT: r.error = ternary(a.elements.i[iter] == MIN_INT, POSITIVE_OVERFLOW, a.error); r.elements.i[iter] =  -a.elements.i[iter]; break;
            case FLOAT: r.elements.f[iter] = -a.elements.f[iter]; break;
            case LINT: r.error = ternary(a.elements.li[iter] == MIN_LINT, POSITIVE_OVERFLOW, a.error); r.elements.li[iter] = -a.elements.li[iter]; break;
            case DOUBLE: r.elements.d[iter] = -a.elements.d[iter]; break;
            case FBITS: (r.elements.fb[iter]).f = -(a.elements.fb[iter]).f; break;
            case DBITS: (r.elements.db[iter]).d = -(a.elements.db[iter]).d; break;
        }
    }
    return r;
}



int main(){
    unsigned int array[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
    vecN vec1 = vector_creation(UINT, sizeof(array) / sizeof(unsigned int), (datapointer)array);
    printf("%u", *(vec1.elements.u + 3));
    return 0;
}