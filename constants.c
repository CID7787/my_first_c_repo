
const double PI = 3.141592653589793; // TODO
const double PI2 = PI * 2;

typedef enum all_unsigned_and_signed_constants{
	MAX_LUINT = ~0ul,
	MAX_UINT = ~0u,
	MAX_INT = 0x7fffffff,
	MIN_INT = ~MAX_INT,
	MIN_CHAR = 0xff,
	MIN_LINT = 0xffffffffffffffff,
	MAX_FLOAT_MANTISSA = 0x3fffff,
	MAX_DOUBLE_MANTISSA = 0x001fffffffffffff,
	MAX_NORM_FLOAT_EXP = 126, 
	MAX_NORM_DOUBLE_EXP = 2046,
	DOUBLE_MANTISSA_IMPLICIT_ONE = 0x0010000000000000,
	DOUBLE_EXP_BIAS = 0x000003ff,
	AMOUNT_OF_DOUBLE_MANTISSA_BITS = 52,
} constants;
