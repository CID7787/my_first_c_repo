// this file contain all datatypes that was created from struct, union and enms
#ifndef headerfile
  #include <stdint.h>
#endif

typedef union variable_all_types{// TODO: all type is the same as fundtypeunion type, leave one
  double d;
  long int li;
  long unsigned int lui;
  char c;
  float f;
  int i;
  unsigned int ui;
} all;

typedef enum function_state{
  NO_PROBLEM = 0,
  ROUNDING,
  F_OVERFLOW,
  F_UNDERFLOW,
}f_error;

typedef struct long_long_unsigned_int {
  long unsigned int high, low;
} lluint;

typedef struct long_long_int{
  long int high, low;
} llint;

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
  QNAN,// TODO: need to systemize in what cases to evaluate Qnan and Snan
  SNAN,// TODO: need to systemize in what cases to evaluate Qnan and Snan
  SUBNORM,
  INCOMPATIBLE,
  ATTEMPT_TO_GET_ROOT_OF_THE_NUMBER,
  MEMORY_LIMIT_EXCESS,
  SIZE_DOES_NOT_MATCH// DESCRIPTION: this error informs about the mismatch of dimension(or amount of elements), for instance in vec_mult function first arg could have less elements than second
}error;


enum DATATYPES { // enumeration
  INT8 = 0,
  UINT8,
  INT32,
  UINT32,
  FLOAT32,
  INT64,
  UINT64,
  FLOAT64,
} typedef datatype;

union POINTERS {
  void* vptr;
  int8_t* i8;
  uint8_t* ui8;
  int16_t* i16;
  uint16_t* ui16;
  int32_t* i32;
  uint32_t* ui32;
  float* f32;
  double* f64;
  uint64_t* ui64;
  int64_t* i64;
} typedef datapointer;

union FUNDUMENTAL_TYPES_UNION{
  int8_t i8;
  uint8_t ui8;
  int32_t i32;
  uint32_t ui32;
  int64_t i64;
  uint64_t ui64;
  float f32;
  double f64;
} typedef fundtypeunion;

typedef struct vector_type{
  datatype* type;
  uint32_t* n;
  error* err;
  datapointer elements;
} vecN;

struct matrix_m_n{
  datatype* type;
  uint32_t* row;
  uint32_t* col;
  error* err;
  datapointer elements;
} typedef matrix_t;

struct int_8_bit_parts{
  int8_t mang: 7;
  int8_t sign: 1;
}typedef int8_parts;

union int_8_bit_construction{
  int8_parts parts;
  int8_t val;
}typedef int8_constr;

struct int_32_bit_parts{
  int32_t mang: 31;
  int32_t sign: 1;
}typedef int32_parts;

union int_32_bit_construction{
  int32_parts parts;
  int32_t val;
}typedef int32_constr;

struct int_64_bit_parts{
  int64_t sign: 63;
  int64_t mang: 1;
}typedef int64_parts;

union int_64_bit_construction{
  int64_parts parts;
  int64_t val;
}typedef int64_constr;
