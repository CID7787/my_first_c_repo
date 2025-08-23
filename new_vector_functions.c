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
      default: return sizeof(datatype);
    }
}

vecN vector_creation(datatype type, unsigned int n, datapointer elements){
    vecN r = {type, n, malloc(n * amount_of_type_bytes(type))};
    while(n--){
        switch(type){
            case CHAR:   r.elements.c[n]  = elements.c[n]; break;
            case UCHAR:  r.elements.uc[n] = elements.uc[n]; break;
            case INT:    r.elements.i[n]  = elements.i[n]; break;
            case UINT:   r.elements.ui[n] = elements.ui[n]; break;
            case FLOAT:  r.elements.f[n]  = elements.f[n]; break;
            case LINT:   r.elements.li[n] = elements.li[n]; break;
            case LUINT:  r.elements.lui[n]= elements.lui[n]; break;
            case DOUBLE: r.elements.d[n]  = elements.d[n]; break;
        }
    }
    return r;
}   

vecN vector_negation(vecN a){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), a.error};
    while(a.n--){
        switch(r.type){
            case CHAR: 
                r.error = ternary(a.elements.c[a.n] == MIN_CHAR, POSITIVE_OVERFLOW, r.error); 
                r.elements.c[a.n] = -a.elements.c[a.n]; 
            break;
            case UCHAR: 
                r.elements.uc[a.n] = a.elements.uc[a.n]; 
            break;
            case INT: 
                r.error = ternary(a.elements.i[a.n] == MIN_INT, POSITIVE_OVERFLOW, r.error); 
                r.elements.i[a.n] =  -a.elements.i[a.n]; 
            break;
            case UINT: 
                r.elements.ui[a.n] = a.elements.ui[a.n]; 
            break;
            case FLOAT: 
                r.elements.f[a.n] = -a.elements.f[a.n]; 
            break;
            case LINT: 
                r.error = ternary(a.elements.li[a.n] == MIN_LINT, POSITIVE_OVERFLOW, r.error); 
                r.elements.li[a.n] = -a.elements.li[a.n]; 
            break;
            case DOUBLE: 
                r.elements.d[a.n] = -a.elements.d[a.n]; 
            break;
        }
    }
    return r;
}

vecN vector_addition(vecN a, vecN b){
    vecN r = {a.type, a.n, malloc(a.n * amount_of_type_bytes(a.type)), a.error};
    unsigned int uint_float_representations = 0x7e7fffff;
	float MAX_FLOAT = *( (float*)(&uint_float_representations) );
    uint_float_representations |= 1u << 31;
    float MIN_FLOAT = *( (float*)(&uint_float_representations) );
    while(a.n && a.n--){
        switch(a.type){
            case CHAR: // DONE
                r.error = ternary((a.elements.c[a.n] < 0) & (a.elements.c[a.n] < (int)MIN_CHAR - (int)b.elements.c[a.n]), V_NEGATIVE_OVERFLOW, r.error);// check whether or not have negative overflow
                r.error = ternary((a.elements.c[a.n] > 0) & (a.elements.c[a.n] > (int)MAX_CHAR - (int)b.elements.c[a.n]), V_POSITIVE_OVERFLOW, r.error);// check whether or not have positive overflow
                r.elements.c[a.n] = a.elements.c[a.n] + b.elements.c[a.n];
            break;
            case UCHAR:// DONE
                r.error = ternary(a.elements.uc[a.n] > (unsigned int)MIN_CHAR - (unsigned int)b.elements.uc[a.n], V_POSITIVE_OVERFLOW, r.error);// check whether or not have positive overflow
                r.elements.uc[a.n] = a.elements.uc[a.n] + b.elements.uc[a.n]; 
            break;
            case INT: // DONE
                r.error = ternary((a.elements.i[a.n] < 0) & (a.elements.i[a.n] < (long int)MIN_INT - (long int)b.elements.i[a.n]), V_NEGATIVE_OVERFLOW, r.error);// check whether or not have negative overflow
                r.error = ternary((a.elements.i[a.n] > 0) & (a.elements.i[a.n] > (long int)MAX_INT - (long int)b.elements.i[a.n]), V_POSITIVE_OVERFLOW, r.error);// check whether or not have positive overflow
                r.elements.i[a.n] = a.elements.i[a.n] + b.elements.i[a.n]; 
            break;
            case UINT:// DONE
                r.error = ternary(a.elements.ui[a.n] > (MAX_UINT - b.elements.ui[a.n]), V_POSITIVE_OVERFLOW, r.error);// check whether or not have positive overflow
                r.elements.ui[a.n] = a.elements.ui[a.n] + b.elements.ui[a.n]; 
            break;
            case FLOAT: // ALMOST DONE
                r.error = ternary((a.elements.f[a.n] < 0) & (a.elements.f[a.n] < (double)MIN_FLOAT - (double)b.elements.f[a.n]), V_NEGATIVE_OVERFLOW, r.error);// check whether or not have negative overflow
                r.error = ternary((a.elements.f[a.n] > 0) & (a.elements.f[a.n] > (double)MAX_FLOAT - (double)b.elements.f[a.n]), V_POSITIVE_OVERFLOW, r.error);// check whether or not have positive overflow
                r.error ;
                r.elements.f[a.n] = a.elements.f[a.n] + b.elements.f[a.n];
            break;
            case LINT: // TODO

            break;
            case LUINT:// DONE
                r.error = ternary(a.elements.lui[a.n] > (MAX_LUINT - b.elements.lui[a.n]), V_POSITIVE_OVERFLOW, r.error);
                r.elements.lui[a.n] = a.elements.lui[a.n] + b.elements.lui[a.n];
            break;            
            case DOUBLE:// TODO
            break;
            default: 
                r.elements.i[a.n] = a.elements.i[a.n]; 
            break;
        }
    }

    return r;
}

