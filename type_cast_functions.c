#ifndef headerfile
    #include <stdint.h>
    #include "constants.c"
    #include "user_defined_datatypes.c"
    #include "logical_functions_of_decision.c"
    #include "bitwise_functions.c"
#endif

void int_n_to_int_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    uint8_t i = 0, neg_cond = (from_ptr[from_s - 1] >> 7) & 1, val, zero_cond = 0;// condition if converting number is negative 
    if(from_s <= to_s){
        for( ; i < from_s; i++){ to_ptr[i] = from_ptr[i]; }
        for( ; neg_cond & (from_s < to_s) & (i < to_s); i++){ to_ptr[i] = 0xff; }// 0xff == 11111111 == -1
        // explanation to upper string: if number is negative traling 1 will be  added to the head part of the data type to which it is being converted to 
    }
    else{
        for(zero_cond = 0; i < to_s; i++){ 
            to_ptr[i] = from_ptr[i]; 
            zero_cond |= sec_arg[i];
        }
        for(i = to_s, val = 0; i < from_s; i++){ // TODO: 11111111 11111111 11111111 01100110, this case have no error but value will be store incorrect
            val |= ((from_ptr[i] ^ 0xff) & -neg_cond) | (from_ptr[i] & -(!neg_cond)); 
            zero_cond |= sec_arg[i];
        }
        *err = ternary(val && zero_cond, ternary(neg_cond & (to_ptr[to_s - 1] >> 7), NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
    }
}

void uint_n_to_int_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    uint8_t i = 0, val = 0; 
    if(from_s > to_s){
        for( ; i < to_s; i++){ to_ptr[i] = from_ptr[i]; val |= sec_arg[i]; }
        for(to_s = 0; i < from_s; i++){ to_s |= from_ptr[i]; val |= sec_arg[i]; }
        *err = ternary(to_s && val, POSITIVE_OVERFLOW, *err);
    }
    else{
        val = from_s ^ to_s;// from_s ^ to_s == from_s != to_s
        for( ; i < from_s; i++){ to_ptr[i] = from_ptr[i]; }
        for( ; (i < to_s) && val; i++){ to_ptr[i] = 0; }
    }
}

void int_n_to_uint_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    uint8_t i = 0, val = 0;
    if(from_s > to_s){
        for( ; i < to_s; i++){ to_ptr[i] = from_ptr[i]; val |= sec_arg[i]; }
        for(to_s = 0; i < from_s; i++){ to_s |= from_ptr[i]; val |= sec_arg[i]; }
        i = (from_ptr[i - 1] >> 7) & 1;
        *err = ternary(to_s && val, ternary(i, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
    }
    else{
        val = from_s ^ to_s; // from_s ^ to_s == from_s != to_s
        for( ; i < from_s; i++){ to_ptr[i] = from_ptr[i]; }
        *err = ternary((from_ptr[i] >> 7) & 1, NEGATIVE_OVERFLOW, *err); 
        for( ; i < to_s; i++){ to_ptr[i] = 0; }
    }
}

void uint_n_to_uint_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    uint8_t i = 0, val = 0; 
    if(from_s > to_s){
        for( ; i < to_s; i++){ to_ptr[i] = from_ptr[i]; val |= sec_arg[i]; }
        for(to_s = 0; i < from_s; i++){ to_s |= from_ptr[i]; val |= sec_arg[i]; }
        *err = ternary(to_s && val, POSITIVE_OVERFLOW, *err);
    }
    else{
        for( ; i < from_s; i++){ to_ptr[i] = from_ptr[i]; }
        for( ; i < to_s; i++){ to_ptr[i] = 0; }
    }
}

void float_n_to_int_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    int32_t i = (from_s == 8) * 3, exp, zero_cond = from_s << 3, neg_cond;// from_s == 8 condition to check if variable type is type double 
    int64_t mant = 0, mask = 0;
    exp = (from_ptr[from_s - 1] & 0x7f) << 1;
    exp <<= i;
    exp |= from_ptr[from_s - 2] >> (7 - i);
    for(i = from_s - 1; i--; ){ mant <<= 8; mant |= from_ptr[i]; }
    i = from_s == sizeof(double);// from_s == 8 condition to check if type variavle is type double
    mant &= ternary(i, MAX_DOUBLE_MANTISSA, MAX_FLOAT_MANTISSA);
    mant |= ternary(i, DOUBLE_MANTISSA_IMPLICIT_ONE, FLOAT_MANTISSA_IMPLICIT_ONE);
    mant = else0(exp, mant);// if exp == 0, it amplies that mant must be 0 too, not mantissa implicit one number 
// error check
    *err = ternary(exp > ternary(i, MAX_NORM_DOUBLE_EXP, MAX_NORM_FLOAT_EXP), SNAN, *err);
    exp -= ternary(i, DOUBLE_EXP_BIAS, FLOAT_EXP_BIAS);
    *err = ternary((exp < 0) && mant, UNDERFLOW, *err);
// error check
    *err = ternary(exp > zero_cond, ternary(from_ptr[from_s - 1] >> 7, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
    exp = ((int64_t)ternary(i, AMOUNT_OF_DOUBLE_MANTISSA_BITS, AMOUNT_OF_FLOAT_MANTISSA_BITS) - exp) % zero_cond;
    for(i = 0; i < exp; i++){ mask <<= 1; mask |= 1; }
    *err = ternary((exp > 0) && (mant & mask), UNDERFLOW, *err);
    mant = ternary(exp < 0, mant << -exp, mant >> exp);
// error check
    neg_cond = from_ptr[from_s - 1] >> 7;
    *err = ternary(mant < 0, ternary(neg_cond, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
    mant = ternary(neg_cond, int64_neg(mant), mant);
    neg_cond = (mant < 0) & (*err != POSITIVE_OVERFLOW);
    for(i = 0; i < to_s; i++){ 
        to_ptr[i] = (mant >> (8 * i)) & 0xff; 
        zero_cond |= sec_arg[i];
    }
    for(mask = 0; i < 8; i++){ 
        exp = (mant >> (i * 8)) & 0xff;
        mask |= ((exp ^ 0xff) & -neg_cond) | (exp & -(!neg_cond));
        zero_cond |= sec_arg[i];
    }
    *err = ternary(mask && zero_cond, ternary(neg_cond & (to_ptr[to_s - 1] >> 7), NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
}

void float_n_to_uint_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    int32_t i, exp, cond = (from_s == 8) * 3;// from_s == 8 condition to check if type variavle is type double 
    uint64_t mant = 0;
    exp = (from_ptr[from_s - 1] & 0x7f) << 1;
    exp <<= cond;
    exp |= from_ptr[from_s - 2] >> (7 - cond);
    for(i = from_s - 1; i--; ){ mant <<= 8; mant |= from_ptr[i]; }
    mant &= ternary(cond, MAX_DOUBLE_MANTISSA, MAX_FLOAT_MANTISSA);
    mant |= ternary(cond, DOUBLE_MANTISSA_IMPLICIT_ONE, FLOAT_MANTISSA_IMPLICIT_ONE);
// error check
    *err = ternary(exp > ternary(cond, MAX_NORM_DOUBLE_EXP, MAX_NORM_FLOAT_EXP), SNAN, *err);
    mant = else0(exp, mant);// if exp == 0, it amplies that mant must be 0 too, not mantissa implicit one number 
    exp -= ternary(cond, DOUBLE_EXP_BIAS, FLOAT_EXP_BIAS);
    *err = ternary((exp < 0) && mant, UNDERFLOW, *err);
    cond = from_s << 3;
// error check
    *err = ternary(from_ptr[from_s - 1] >> 7, NEGATIVE_OVERFLOW, *err);
    *err = ternary(exp > cond, POSITIVE_OVERFLOW, *err);
    exp = ((int64_t)ternary(cond, AMOUNT_OF_DOUBLE_MANTISSA_BITS, AMOUNT_OF_FLOAT_MANTISSA_BITS) - exp) % cond;
    for(i = cond = 0; i < exp; i++){ cond <<= 1; cond |= 1; }
    *err = ternary((exp > 0) && (mant & cond), UNDERFLOW, *err);
    mant = ternary(exp < 0, mant << -exp, mant >> exp);
    for(i = cond = exp = 0; i < to_s; i++){
        to_ptr[i] = (mant >> (8 * i)) & 0xff; 
        cond |= sec_arg[i];
    }
    for(; i < 8; i++){ 
        exp |= (mant >> (8 * i)) & 0xff;
        cond |= sec_arg[0]; 
    }
    *err = ternary(exp && cond, POSITIVE_OVERFLOW, *err);
}

void int_n_to_float_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    int32_t i, exp, neg_cond = from_ptr[from_s - 1] >> 7, is_float32 = to_s == sizeof(float), cond;
    int64_t val = 0;
    for(cond = 0, i = 8; i-- > from_s; ){// this loop fills last bit of val with 1 if negative variable contains negative value
      val <<= 8;
      val |= -neg_cond;
      cond |= sec_arg[i];
    }
    for(cond =  0, ++i; i--; ){ 
      val <<= 8; 
      val |= from_ptr[i]; 
      cond |= sec_arg[i];
    }
    *err = ternary((val == MIN_INT64) && cond, UNDERFLOW, *err);
    val = ternary(neg_cond, int64_neg(val), val);
    exp = how_many_bits_until_eldest_1(val);
    i = ternary(is_float32, AMOUNT_OF_FLOAT_MANTISSA_BITS, AMOUNT_OF_DOUBLE_MANTISSA_BITS);// this is the amount of bits in mantissa of this type
    *err = ternary(cond && (exp > i) , UNDERFLOW, *err);
    i -= exp;
    val = ternary(i > 0, val << i, val >> -i);
    exp += ternary(is_float32, FLOAT_EXP_BIAS, DOUBLE_EXP_BIAS) & -(!!val);
    val &= ternary(is_float32, MAX_FLOAT_MANTISSA, MAX_DOUBLE_MANTISSA);
    cond = ternary(is_float32, 3, 7); // 3 and 7 isn't just magical numbers, they identify the number of bytes in cast to variable type for storing the mantissa part
    for(i = 0; i < cond; i++){ to_ptr[i] = (val >> (i * 8)) & 0xff; }// evaluting mantissa
    to_ptr[to_s - 2] = (to_ptr[to_s - 2] & ternary(is_float32, 0x7f, 0xf)) | ((exp << ternary(is_float32, 7, 4)) & 0xff); 
    to_ptr[to_s - 1] = ( exp >> (4 >> (is_float32 << 1))) | (from_ptr[from_s - 1] & 0x80);
}

void uint_n_to_float_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){// TODO: check validality from_s and to_s
  if(!(from_ptr && to_ptr && sec_arg && err)){
      if(err){ *err = NULL_POINTER; }
      return;
  }
  int32_t i, cond, exp, is_float32 = to_s == sizeof(float);
  uint64_t val = 0;
  for(i = from_s, cond = 0; i--; ){
    val <<= 8;// 8 == one byte bit size
    val |= from_ptr[i];
    cond |= sec_arg[i];
  }
  exp = how_many_bits_until_eldest_1(val);
  i = ternary(is_float32, AMOUNT_OF_FLOAT_MANTISSA_BITS, AMOUNT_OF_DOUBLE_MANTISSA_BITS);
// error check
  *err = ternary(cond && (exp > i), UNDERFLOW, *err);
  i -= exp;
  val = ternary(i > 0, val << i, val >> -i);
  exp += ternary(is_float32, FLOAT_EXP_BIAS, DOUBLE_EXP_BIAS) & -(!!val);
  val &= ternary(is_float32, MAX_FLOAT_MANTISSA, MAX_DOUBLE_MANTISSA);
  cond = ternary(is_float32, 3, 7); // 3 and 7 isn't just magical numbers, they identify the number of bytes in cast to variable type for storing the mantissa part
  for(i = 0; i < cond; i++){ to_ptr[i] = (val >> (i * 8)) & 0xff; }// evaluting mantissa
  to_ptr[to_s - 2] = (to_ptr[to_s - 2] & ternary(is_float32, 0x7f, 0xf)) | ((exp << ternary(is_float32, 7, 4)) & 0xff); 
  to_ptr[to_s - 1] = exp >> (4 >> (is_float32 << 1));
}

void float_n_to_float_k_type_cast(uint8_t* from_ptr, uint8_t from_s, uint8_t* to_ptr, uint8_t to_s, uint8_t* sec_arg, error* err){
    if(!(from_ptr && to_ptr && sec_arg && err)){
        if(err){ *err = NULL_POINTER; }
        return;
    }
    int8_t i, zero_cond = 0;
    if(from_s == to_s){ for(i = 0; i < from_s; i++){ to_ptr[i] = from_ptr[i]; } return; }
    fbits f;
    dbits d;
    if(to_s > from_s){
      for(i = 4; i--; ){ 
        f.uint <<= 8; 
        f.uint |= from_ptr[i]; 
        zero_cond |= sec_arg[i] | sec_arg[i + 4];
      }
      *err = ternary(f.parts.exp > MAX_NORM_FLOAT_EXP, SNAN, *err);
      *err = ternary(!(f.parts.exp) && f.parts.mantissa && zero_cond, UNDERFLOW, *err);
      d.parts.mantissa = f.parts.mantissa;
      d.parts.mantissa <<= 29; // 29 == (AMOUNT_OF_DOUBLE_MANTISSA_BITS - AMOUNT_OF_FLOAT_MANTISSA_BITS)
      d.parts.exp = (f.parts.exp - FLOAT_EXP_BIAS + DOUBLE_EXP_BIAS) & -(!!f.parts.exp);
      d.parts.sign = f.parts.sign;
      for(i = 0; i < 8; i++){ to_ptr[i] = (d.luint >> (i * 8)) & 0xff; }
    }
    else{
      for(i = 8; i--; ){ 
        d.luint <<= 8; 
        d.luint |= from_ptr[i];
        zero_cond |= sec_arg[i % 4];
      }
      *err = ternary(d.parts.exp > MAX_NORM_DOUBLE_EXP, SNAN, *err);
      *err = ternary(!(d.parts.exp) && d.parts.mantissa && zero_cond, UNDERFLOW, *err);
      i = zero_cond && (how_many_0_until_youngest_1(d.parts.mantissa) < 29);// 29 == amount_of_double_mantissa_bits - amount_of_float_mantissa_bits
      *err = ternary(i, UNDERFLOW, *err);
      f.parts.mantissa = d.parts.mantissa >> 29;// 29 == amount_of_double_mantissa_bits - amount_of_float_mantissa_bits
      d.parts.exp -= (DOUBLE_EXP_BIAS - FLOAT_EXP_BIAS) & -(!!(d.parts.exp));
      *err = ternary((d.parts.exp > MAX_NORM_FLOAT_EXP) && zero_cond, ternary(d.parts.sign, NEGATIVE_OVERFLOW, POSITIVE_OVERFLOW), *err);
      *err = ternary((d.parts.exp < 0) && zero_cond, UNDERFLOW, *err);
      f.parts.exp = (uint32_t)d.parts.exp % (MAX_NORM_FLOAT_EXP + 1);
      f.parts.sign = d.parts.sign;
      for(i = 0; i < 4; i++){ to_ptr[i] = (f.uint >> (i * 8)) & 0xff; }
    } 
  }
