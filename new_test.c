
#include <stdio.h>
#include <time.h>



const long unsigned int MAX_LUINT = ~0ul;
const unsigned int MAX_UINT = ~0u;
const int MAX_INT = 0x7fffffff;
const int MIN_INT = ~MAX_INT;
const long unsigned int MAX_DOUBLE_MANTISSA = 0x001fffffffffffff;
const unsigned int MAX_NORM_DOUBLE_EXP = 2046;
const long unsigned int DOUBLE_MANTISSA_HIDDEN_ONE = 0x0010000000000000;
const unsigned int DOUBLE_EXP_BIAS = 0x000003ff;
const unsigned char AMOUNT_OF_DOUBLE_MANTISSA_BITS = 52;
const unsigned int SIZEOF_D_BITS = sizeof(double) << 3;


typedef enum error_code{
    NO_ERROR = 0,
    UNDEFINED_BEHAVIOR,
    POSITIVE_OVERFLOW,
    NEGATIVE_OVERFLOW,
    UNDERFLOW,
    DIVISION_BY_ZERO,
    ZERO_TO_ZERO// DESCRIPTION: ZERO TO THE POWER ZERO
}error;

typedef struct double_bitfields_sign_exponent_mantissa{
    long unsigned int magn: 52;
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

long unsigned int ternary(unsigned char condition, long unsigned int true_value, long unsigned int false_value){
    long unsigned int array[2] = {true_value, false_value};
    return array[!condition];
}

long unsigned int how_many_0_until_youngest_1(long unsigned int a){ //32
    unsigned int counter = 0;
    while(!(a & 1ul)){ a >>= 1; counter++; }// 10101
    return counter;
}

long unsigned int how_many_bits_until_eldest_1(long unsigned int a){
    unsigned int counter = 0;
    while(a > 1){ ++counter; a >>= 1; } // 10101
    return counter;
}

long unsigned int else0(unsigned int cond, long unsigned int x){
    long unsigned int array[2] = {x, 0};
    return array[!cond];
}
int safe_int_addition(int addend1, int addend2, error* err){
    unsigned int pos_overflow_cond = (addend1 > 0) && (addend2 > ((long int)MAX_INT - (long int)addend1));
    unsigned int neg_overflow_cond =  (addend1 < 0) && (addend2 < ((long int)MIN_INT - (long int)addend1));
    *err = else0(pos_overflow_cond, POSITIVE_OVERFLOW) | else0(!pos_overflow_cond, *err);
    *err = else0(neg_overflow_cond, NEGATIVE_OVERFLOW) | else0(!neg_overflow_cond, *err);
    return addend1 + ( addend2 & (else0(!pos_overflow_cond, ~0) & else0(!neg_overflow_cond, ~0)) );
}
unsigned int safe_uint_addition(unsigned int arg1, unsigned int arg2, error* err){
    unsigned int cond = (arg1 > 0) && (arg2 > (MAX_UINT - arg1));
    *err = else0(cond, POSITIVE_OVERFLOW) | else0(!(*err), *err);
    return arg1 + (arg2 & else0(!cond, ~0));
}

long unsigned int safe_luint_addition(long unsigned int addend1, long unsigned int addend2, error* err){
    unsigned int cond = addend2 > (MAX_LUINT - addend1);
    *err = else0(cond, POSITIVE_OVERFLOW) | else0(!cond, *err);
    return addend1 + (addend2 & else0(!cond, ~0ul));
}

long unsigned int safe_luint_multiplication(long unsigned int multiplier, long unsigned int multiplicand, error* err){
    long unsigned int product = 0;
    while(multiplicand-- > 0){
        if(*err){ return product; }
        product = safe_luint_addition(product, multiplier, err); 
    }
    return product;
}


long unsigned int new_lluint_multiplication(long unsigned int multiplicand, long unsigned int multiplier, unsigned int bin_point_shift){
    long unsigned int result_l, result_r, product1, product2, multiplicand_l, multiplicand_r, multiplier_l, multiplier_r, mask_r = 0x7ffffff;
    error err = NO_ERROR;
    // getting left and right parts of multiplicand and multiplier
    multiplicand_l = multiplicand >> 27;
    multiplier_l = multiplier >> 27;
    multiplicand_r = multiplicand & mask_r;
    multiplier_r = multiplier & mask_r;
    // computing products
    product1 = safe_luint_multiplication(multiplicand_l, multiplier_l, &err);// << (27 + 27)
    product2 = safe_luint_multiplication(multiplicand_r, multiplier_l, &err);// << 27
    result_r = safe_luint_multiplication(multiplicand_l, multiplier_r, &err);// << 27
    product2 = safe_luint_addition(product2, result_r, &err);// << 27
    result_r = safe_luint_multiplication(multiplicand_r, multiplier_r, &err);
    // summing up products
    product2 = safe_luint_addition(product2, result_r & (~0ul << 27), &err);
    product1 = safe_luint_addition(product1, product2 & (~0ul << 27), &err);
    result_r = product1 << 54 | product2 << 27 | result_r;
    result_l = product1 >> 10;
    // getting exponent
    product1 = how_many_bits_until_eldest_1(result_r);
    product2 = how_many_bits_until_eldest_1(result_l);
    product1 = ternary(result_l, product1 + 1, product1);
    product2 = ternary(result_l, product2 + 1, product2);
    bin_point_shift = ternary(product2, result_l >> safe_int_addition(bin_point_shift, -64, &err), result_r >> bin_point_shift);
    bin_point_shift = !!(bin_point_shift & 0b10);
    // normalizing number and putting bin_point_shift into exponent
    result_l <<= safe_int_addition(64, -product2, &err);// 0000 0100
    result_r >>= product2;
    result_l |= result_r;
    product1 = how_many_bits_until_eldest_1(result_l);
    result_l = ternary(result_l < DOUBLE_MANTISSA_HIDDEN_ONE, result_l << safe_int_addition(52, -product1, &err), result_l >> safe_int_addition(product1, -52, &err));
    result_l = (DOUBLE_MANTISSA_HIDDEN_ONE - 1) & result_l;
    result_l = ternary(bin_point_shift, result_l | DOUBLE_MANTISSA_HIDDEN_ONE, result_l);
    return result_l;
}

dbits safe_double_mantissa_multiplication_with_rounding(dbits multiplicand, dbits multiplier, error* err){
    multiplicand.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplicand.parts.magn;
    multiplier.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplier.parts.magn;
    // remove useless zeros
    if(how_many_0_until_youngest_1(multiplicand.luint) < how_many_0_until_youngest_1(multiplier.luint)) { while(!(multiplicand.luint & 1ul)) { multiplicand.luint >>= 1; multiplier.luint >>= 1; }}
    else{ while(!(multiplier.luint & 1ul)) { multiplicand.luint >>= 1; multiplier.luint >>= 1; } }
    if((multiplicand.luint & multiplier.luint) == 1) { return (dbits){ .luint = 0}; }
    // variable declaration
    multiplier.luint = new_lluint_multiplication(multiplicand.luint, multiplier.luint, how_many_bits_until_eldest_1(multiplicand.luint) << 1);
    return multiplier;
}
double safe_double_multiplication_with_rounding(dbits multiplicand, dbits multiplier, error* err){
    if(!multiplicand.d | !multiplier.d){ return 0; }

    clock_t start_t = clock();
    dbits result = safe_double_mantissa_multiplication_with_rounding(multiplicand, multiplier, err); if(*err){ return result.d; }
    // printf("\nmantissa compute time = %lg sec\n", (long double)(clock() - start_t));
    clock_t mantissa_time = clock() - start_t;
    start_t = clock();
    unsigned int exponent = safe_uint_addition(multiplicand.parts.exp, multiplier.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; }
    // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT

    *err = else0(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(exponent <= MAX_NORM_DOUBLE_EXP, *err); if(*err){ return result.d; }
    result.parts.exp = exponent;
    result.parts.sign = multiplicand.parts.sign ^ multiplier.parts.sign;
    printf("function iner time:%lu", (clock() - start_t) + mantissa_time);
    return result.d;
}

int main(void)
{
    error err = NO_ERROR;
    clock_t start_t = clock();
    safe_double_multiplication_with_rounding((dbits){ .d = 2.6}, (dbits){ .d = 2.5}, &err);
    printf("\n time per second:  %lu sec \n", (clock() - start_t));
    return 0;
}
