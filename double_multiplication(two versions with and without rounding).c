#include <stdio.h>
#include <.gitignore>/es
#include <./safe_arithmetic_function.c>
#include <./bitwise_functions.c>

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
    *err = else0(exponent > MAX_DOUBLE_EXP, POSITIVE_OVERFLOW) | else0(!(exponent > MAX_DOUBLE_EXP), *err); if(*err){ return result.d; }
    result.parts.exp = exponent;
    result.parts.sign = multiplicand.parts.sign ^ multiplier.parts.sign;
    return result.d;
}

long unsigned int special_luint_multiplication(long unsigned int multiplicand, long unsigned int multiplier, unsigned int bits_after_bin_p, dbits* exponent_storer){
    long unsigned int part1, part2;
    part1 = 0;
    part2 = 0;
    error err = NO_ERROR;
    while(multiplicand--){
        part1 = safe_luint_addition(part1, multiplier, &err);
        if(*err){ part1 = multiplier - (MAX_LUINT - part1) - 1; ++part2; }//  max_value = 35; multiplier = 15; part1 = 30;  
    }
    unsigned int product_bits_after_bin_p = how_many_bits_until_eldest_1(part1) + how_many_bits(part2);
    (*exponent_storer).parts.exp = product_bits_after_bin_p - bits_after_bin_p;
    unsigned int n = how_many_bits_until_eldest_1(part2);
    part2 <<= 63 - n;
    part1 >>= n;
    part2 |= ++part1;
    n = how_many_bits_until_eldest_1(part2);
    if(part2 > MAX_DOUBLE_MANTISSA){ part2 = n - 53; }
    else{ part2 = 53 - n; }
    return part2;
}

long unsigned int lluint_multiplication(long unsigned int multiplicand, long unsigned int multiplier, unsigned int bits_after_bin_p, dbits* exponent_storer){
    long unsigned int part1 = 0, part2 = 0;
    error err = NO_ERROR;
    while(multiplicand--){
        part1 = safe_luint_addition(part1, multiplier, &err);
        if(err){ part1 = multiplier - (MAX_LUINT - part1) - 1; ++part2; }
    }
    unsigned int n = how_many_bits_until_eldest_1(part2);
    unsigned int product_bits_after_bin_p = how_many_bits_until_eldest_1(part1) + n;
    (*exponent_storer).parts.exp = product_bits_after_bin_p - bits_after_bin_p;
/*

    1.10101010101  2^(exp - bias)
    1.10101010101  2^(exp - bias)

    Subnormal:
    2.5 == 5 / 2 = 101b / 2 == 101b * 2^(-1)
    2.5 * 2.5 = 101b * 101b * 2^(-1) * 2(-1) == 11001b * 2^(-2) = 25 * 1/4 = 6.25 = 2.5^2


    101b > 001b
    101b == 001.01b * 2^(2) == 1.01b * 2^(2)

    Normal:
    2.5 = 1.01b * 2^(-1) * 2^2 == (1.01b * 2^2) * 2^(-1) == ((1b + 0.01b) * 2^2) * 2^(-1) == (1 * 2^2 + 0.01b * 2^2) * 2^(-1) == (4 + 0.01b * 2^2) * 2^(-1) = (5) * 2^(-1) 
    2.5 * 2.5 = (1.01b * 2^(-1) * 2^2) * (1.01b * 2^(-1) * 2^2) = 1.1001b * 2^(-2) * 2^4 =  
    

    (1).  110000000000000000000000000
    (1).  110000000000000000000000000

    111000000000000000000000000000000
    *
    111000000000000000000000000000000
    =

1)  0     0       1
   *0    *1      *1
   =0    =0      =1  
2)  01        11     11
   *01       *10    *11
   =01      =110  =1001
3)  100     111         111        111          111
   *110    *100        *101       *110         *111
  11000   11100      100011     101010       110001




    0.5 -> 0.25 -> 0.125 -> 0.0625 -> 0.03125 -> 0.015625
      000000001100100000000000000000000000000000000000000000000000000000
    >>                                11001000000000000000000000000000|00000000000000000000000000

*/
    part2 <<= 63 - n;// 1111 1000
    part1 >>= ++n;//    0000 1110
    part2 |= part1;
    n = how_many_bits_until_eldest_1(part2);
    if(part2 > MAX_DOUBLE_MANTISSA){ part2 = n - 53; }
    else{ part2 = 53 - n; }
    // part2 = (part2 > MAX_DOUBLE_MANTISSA) ? (n - 53) : (53 - n);
    // part2 = (part2 > MAX_DOUBLE_MANTISSA) ? -(53 - n) : (53 - n);
    // part2 = (part2 > MAX_DOUBLE_MANTISSA) ? -1 : 1;
    // part2 *= 53 - n;
    return part2;
}

long unsigned int how_many_0_until_youngest_1(long unsigned int a){ //32
  unsigned int counter = 0;
  while(!(a & 1ul)){ a >>= 1; counter++; }// 10101
  return counter;
}

long unsigned int lluint_multiplication(long unsigned int multiplicand, long unsigned int multiplier, unsigned int exponent){
    long unsigned int part1 = 0, part2 = 0;
    int n1, n2;
    unsigned int cond;
    error err = NO_ERROR;
    while(multiplicand--){
        part1 = safe_luint_addition(part1, multiplier, &err); 
        if(err) { ++part2; part1 = multiplier - (MAX_LUINT - part1) - 1; }
    }
    n1 = how_many_bits_until_eldest_(part1);
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

dbits safe_double_mantissa_multiplication_with_rounding(dbits multiplicand, dbits multiplier, error* err){
    multiplicand.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplicand.parts.magn;
    multiplier.luint = DOUBLE_MANTISSA_HIDDEN_ONE | multiplier.parts.magn;
    // remove useless zeros
    if(how_many_0_until_youngest_1(multiplicand.luint) < how_many_0_until_youngest_1(multiplier.luint)) { while(!(multiplicand.luint & 1ul)) { multiplicand.luint >>= 1; multiplier.luint >>= 1; }}
    else{ while(!(multiplier.luint & 1ul)) { multiplicand.luint >>= 1; multiplier.luint >>= 1; } }

    if((multiplicand.luint & multiplier.luint) == 1) { return (dbits){ .luint = DOUBLE_MANTISSA_HIDDEN_ONE}; }
    // variable declaration

    multiplier.luint = lluint_multiplication(multiplicand.luint, multiplier.luint, how_many_bits_until_eldest_one(multiplicand.luint) << 1);
    return multiplier;
}   

double safe_double_multiplication_with_rounding(dbits multiplicand, dbits multiplier, error* err){
    if(!multiplicand.d | !multiplier.d){ return 0; }
    dbits result = safe_double_magnitude_multiplication_with_rounding(multiplicand, multiplier, err); if(*err){ return result.d; }
    unsigned int exponent = safe_uint_addition(multiplicand.parts.exp, multiplier.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_uint_addition(exponent, result.parts.exp, err); if(*err){ return result.d; }
    exponent = safe_int_addition(exponent, -DOUBLE_EXP_BIAS, err); if(*err){ return result.d; } 
    // check whether or not exponent value bigger than MAX_DOUBLE_EXPONENT
    *err = else0(exponent > MAX_DOUBLE_EXP, POSITIVE_OVERF LOW) | else0(exponent <= MAX_DOUBLE_EXP, *err); if(*err){ return result.d; }
    result.parts.exp = exponent;
    result.parts.sign = multiplicand.parts.sign ^ multiplier.parts.sign;
    return result.d;
}

int main(){
    dbits d1, d2;
    d1.luint = 0x7fefffffffffffff;
    d2.luint = 0xffefffffffffffff;
    error err = NO_ERROR;
        printf("\nthe product of multiplication %lf and %lf is equal to %lf\n error code: %u\n\n", d1.d, d2.d, safe_double_multiplication(d1, d2, &err), err);
        printf("\n\n%u", err);
    return 0;
}