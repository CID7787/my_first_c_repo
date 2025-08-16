// this file contain all datatypes that was created from struct, union and enms

typedef enum error_code{
    NO_ERROR = 0,
    UNDEFINED_BEHAVIOR,
    POSITIVE_OVERFLOW,
    NEGATIVE_OVERFLOW,
    UNDERFLOW,
    DIVISION_BY_ZERO,
    ZERO_TO_ZERO// DESCRIPTION: ZERO TO THE POWER ZERO
}error;

typedef struct long_long_int {
    long unsigned int high;
    long unsigned int low;
} lluint;

typedef struct double_bitfields_sign_exponent_mantissa{
    long unsigned int mantissa: 52;
    long unsigned int exp: 11;
    long unsigned int sign: 1;
} ieee754;
    
typedef struct double_bitfields_sign_positive{
    long unsigned int positive: 63;
    long unsigned int sign: 1;
} dluint;

typedef union double_bits{
    double d;
    long unsigned int luint;
    ieee754 parts; 
    dluint bits;
} dbits;

union union_name1{
  double d;
  long unsigned int luint;
} v1;
union union_name2{
  double d;
  struct struct_name{
      long unsigned int luint1: 63;
      long unsigned int luint2:  1; 
  } luint;
} union_var;

 enum vectorErrors {
   ALL_GOOD = 0,
   V_OVERFLOW,
   V_UNDERFLOW,
   SQUARE_ROOT_OF_NEGATIVE_NUMBER,
   RETURN_INFINITY,
   ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER,
   RETURN_REAL_PART,
   V_UNDEFINED_BEHAVIOR,
 } typedef vERR;

enum DATATYPES { // enumeration
  INT = 0,
  CHAR,
  FLOAT,
  DOUBLE,
  UINT,
} typedef datatype;

union POINTERS {
  int* i;
  char* c;
  float* f;
  unsigned int* u;
  double* d;
} typedef datapointer;
  
struct vector_n {
  datatype type;
  unsigned int n; // this "n" is amount of elements
  datapointer elements;
  vERR error;
  //  type       n               elements
  //  datatype   unsigned int    int* i
  //                             char* c
  //                             float* f
  //                             unsigned int* u
  //                             double *d
} typedef vecN;
