// this file contain all datatypes that was created from struct, union and enms
typedef union variable_all_types{
  double d;
  long int li;
  long unsigned int lui;
  char c;
  float f;
  int i;
  unsigned int ui;
} all;

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


typedef struct long_long_int {
  long unsigned int high;
    long unsigned int low;
} lluint;



typedef struct double_bitfields_sign_exponent_mantissa{
  long unsigned int mantissa: 52;
  long unsigned int exp: 11;
  long unsigned int sign: 1;
} dieee754;

typedef struct double_bitfields_sign_positive{
  long unsigned int positive: 63;
  long unsigned int sign: 1;
} dluint;

typedef union double_bits{
  double d;
  long unsigned int luint;
  dieee754 parts; 
  dluint bits;
} dbits;

typedef struct float_bitfields_sign_exponent_mantissa{
  unsigned int mantissa: 23;
  unsigned int exp: 8;
  unsigned int sign: 1;
} fieee754;

typedef struct float_bitfields_sign_positive{
  unsigned int positive: 31;
  unsigned int sign: 1;
} fuint;

typedef union float_bits{
  float f;
  unsigned int uint;
  fieee754 parts; 
  fuint bits;
} fbits;

typedef enum error_code{
  NO_ERROR = 0,
  POSITIVE_OVERFLOW,
  NEGATIVE_OVERFLOW,
  UNDERFLOW,
  DIVISION_BY_ZERO,
  ZERO_TO_ZERO,// DESCRIPTION: ZERO TO THE POWER ZERO
  UNDEFINED_BEHAVIOR,
  SQUARE_ROOT_OF_NEGATIVE_NUMBER,
  POSITIVE_INFINITY,
  NEGATIVE_INFINITY,
  RETURN_REAL_PART,
  NULL_POINTER,
  QNAN,
  SNAN,
  INCOMPATIBLE,
  ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER
}error;

typedef union one_byte_all_types{
  int8_t* i;
  uint8_t* ui;
}B1type;

typedef union four_byte_all_types{
  int32_t* i;
  uint32_t* ui;
  float* f;
}B4type;

typedef union eight_byte_all_types{
  int64_t* i;
  uint64_t* ui;
  double* d;
}B8type;


union POINTERS_TO_ALL_DATA_TYPES {
  B1type b1;
  B4type b4;
  B8type b8;
} typedef alldatapointer;

enum DATATYPES { // enumeration
  CHAR = 0,
  UCHAR,
  INT,
  UINT,
  FLOAT,
  LINT,
  LUINT,
  DOUBLE,
} typedef datatype;

union POINTERS {
  char* c;
  unsigned char* uc;
  int* i;
  unsigned int* ui;
  float* f;
  double* d;
  long unsigned int* lui;
  long int* li;
} typedef datapointer;
  
struct vector_n {
  datatype type;
  unsigned int n; // this "n" is amount of elements
  alldatapointer elements;
  error v_error;
  //  type       n               elements
  //  datatype   unsigned int    int* i
  //                             char* c
  //                             float* f
  //                             unsigned int* u
  //                             double *d
} typedef vecN;
