#include <stdio.h>
#include <logical_functions_of_decision.c>
#include <safe_arithmetic_function.c>
#include <bitwise_functions.c>

dbits safe_double_mantissa_multiplication_without_rounding(dbits multiplicand, dbits multiplier, error* err){
    multiplicand.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplicand.parts.magn;
    multiplier.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplier.parts.magn;
    // remove useless zeros
    while (!(multiplicand.luint & 1ul)) { multiplicand.luint >>= 1; }
    while (!(  multiplier.luint & 1ul)) { multiplier.luint >>= 1; }
    // variable declaration
    unsigned int bits_after_binary_point = safe_uint_addition(how_many_bits_until_eldest_1(multiplicand.luint), how_many_bits_until_eldest_1(multiplier.luint), err); if(*err){ return multiplicand; }

    multiplier.luint = safe_luint_multiplication(multiplicand.luint, multiplier.luint, err); if(*err){ return multiplier; } //@TODO: make it so any result fits.(in the version with rounding)
    // variable declararion 
    unsigned int mant_overflow_cond = multiplier.luint > MAX_DOUBLE_MANTISSA; // I suspect this can be simpler with binary operators. Something like this: (multiplier.luint & ~MAX_DOUBLE_MANTISSA) 
    *err = else0(mant_overflow_cond, POSITIVE_OVERFLOW) | else0(!mant_overflow_cond, *err); if(*err){ return multiplicand; }
    bits_after_binary_point = safe_int_addition(how_many_bits_until_eldest_1(multiplier.luint), -bits_after_binary_point, err); if(*err){ return multiplier; }
    // moving product's first one to hidden one position
    multiplier.luint <<= safe_int_addition(AMOUNT_OF_DOUBLE_MANTISSA_BITS, -how_many_bits_until_eldest_1(multiplier.luint), err); if(*err){ return multiplier; }
    multiplier.parts.exp = bits_after_binary_point;
    return multiplier;
}

double safe_double_multiplication_without_rounding(dbits multiplicand, dbits multiplier, error* err){
    // check whether or not one of arguments equal to 0
    if(!multiplicand.d | !multiplier.d){ return 0; }
    dbits result = multiplier;
    result = safe_double_magnitude_multiplication(multiplicand, multiplier, err); if(*err){ return result.d; }
    unsigned int exponent = safe_uint_addition(multiplicand.parts.exp, multiplier.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; }
    // check whether of not exponent value bigger than MAX_DOUBLE_EXPONENT
    *err = else0(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(exponent <=  MAX_NORM_DOUBLE_EXP), *err); if(*err){ return result.d; }
    result.parts.exp = exponent;
    result.parts.sign = multiplicand.parts.sign ^ multiplier.parts.sign;
    return result.d;
}

long unsigned int how_many_0_until_youngest_1(long unsigned int a){ //32
  unsigned int counter = 0;
  while(!(a & 1ul)){ a >>= 1; counter++; }// 10101
  return counter;
}

long unsigned int old_lluint_multiplication(long unsigned int multiplicand, long unsigned int multiplier, unsigned int exponent){
    long unsigned int part1 = 0, part2 = 0;
    int n1, n2;
    unsigned int cond;
    error err = NO_ERROR;
    while(multiplicand--){
        part1 = safe_luint_addition(part1, multiplier, &err); 
        if(err) { ++part2; part1 = multiplier - (MAX_LUINT - part1) - 1; }
    }
    n1 = how_many_bits_until_eldest_1(part1);
    n2 = how_many_bits_until_eldest_1(part2);
    multiplicand = else0(part2, n2 + 1) | else0(!part2, n1 + 1);
    cond = multiplicand > SIZEOF_D_BITS;
    multiplier = (else0(cond, part2) | else0(!cond, part1)) >> (exponent - else0(cond, SIZEOF_D_BITS));
    exponent = multiplier & 0b10;
    part1 >>= n2 + else0(part2, 1);
    part2 <<= 63 - n2;
    part2 |= part1;
    n2 = how_many_bits_until_eldest_1(part2);
    part2 >>= else0(part2 > MAX_DOUBLE_MANTISSA, safe_int_addition(n2, -52, &err));
    part2 <<= else0(part2 <= MAX_DOUBLE_MANTISSA, safe_int_addition(52, -n2, &err));
    part2 ^= DOUBLE_MANTISSA_HIDDEN_ONE;
    return ((long unsigned int)exponent << 52) | part2;
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
    bin_point_shift = ternary(product2, product2 >> safe_int_addition(bin_point_shift, -64, &err), product1 >> bin_point_shift);
    bin_point_shift = !!(bin_point_shift & 0b10);
    // normalizing number and putting bin_point_shift into exponent
    result_l <<= safe_int_addition(64, -product2, &err); // 0000 0100
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

    if((multiplicand.luint & multiplier.luint) == 1) { return (dbits){ .luint = DOUBLE_MANTISSA_HIDDEN_ONE}; }
    // variable declaration

    multiplier.luint = new_lluint_multiplication(multiplicand.luint, multiplier.luint, how_many_bits_until_eldest_one(multiplicand.luint) << 1);
    return multiplier;
}   



int my_floor(float x, error* err){
    long int lintx = x;
    lintx -= else0(x < 0, 1);
    *err = ternary((lintx > MAX_INT) | (lintx < MIN_INT), ternary(lintx > MAX_INT, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), *err);
    return lintx;
}

int my_ceil(float x, error* err){
    long int lintx = x;
    lintx += else0(lintx < x, 1);
    *err = ternary((lintx > MAX_INT) | (lintx < MIN_INT), ternary(lintx > MAX_INT, POSITIVE_OVERFLOW, NEGATIVE_OVERFLOW), *err);
    return lintx;
}

  double safe_double_multiplication_with_rounding(dbits multiplicand, dbits multiplier, error* err){
        if(!multiplicand.d | !multiplier.d){ return 0; }
        dbits result = safe_double_mantissa_multiplication_with_rounding(multiplicand, multiplier, err); if(*err){ return result.d; }
        unsigned int exponent = safe_uint_addition(multiplicand.parts.exp, multiplier.parts.exp, err); if(*err){ return result.d; }
        exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
        exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; }
        // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
        *err = else0(exponent > MAX_NORM_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(exponent <= MAX_NORM_DOUBLE_EXP, *err); if(*err){ return result.d; }
        result.parts.exp = exponent;
        result.parts.sign = multiplicand.parts.sign ^ multiplier.parts.sign;
        return result.d;
    }

    int main(){
        double d1 = 3;
        double d2 = 2;
        error err = NO_ERROR;
        printf("\nthe product of multiplication %lf \t and %lf \t equals to %lf\n error: %u", d1, d2, safe_double_multiplication_with_rounding((dbits){ .d = d1}, (dbits){ .d = d2}, &err), err);
        printf("\n%u\n", err);
        return 0;
    }
