unsigned int logicalNOT(unsigned int a){
  return 1 >> a;
}

/**
includes: ,
dependencies: ,
run: 11
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) == (*(long int*)&i) % 2) {printf("\nERROR! %u != %u (i: %u)", $0(i), (*(unsigned int*)&i) % 2, i); };
  }
}
description: This function checks whether the last bit is 0

*/
unsigned int last_bit_is_0_with_shift(unsigned int a){
    a = a << 31;
    a = a >> 31;
    // 0.1f; 0.1; 0xb; 0b1;
    return !a;
}
unsigned int last_bit_is_0(unsigned int a){
    return !(a & 1);
}

  /**
includes: ,
dependencies: ,
run: 11
test: {
  for (@1 i = 0; i < sizeof(@1)*8; ++i) {
    if($0(i) != (*(long int*)&i) % 2) {printf("\nERROR! %u != %u (i: %u)", $0(i), (*(unsigned int*)&i) % 2, i); };
  }
}
description: This function checks whether the last bit is 1

*/
unsigned int last_bit_is_1(unsigned int a){
    return a & 1u; // should return 1 if last bit is 1, 0 if last bit is 0
}


unsigned int ith_bit_is_1(unsigned int a, unsigned int i){
    return a & (1u << i);// 1u is 1 of type unsigned int
}


unsigned int byte2bit(unsigned int n){
    return n << 3; // n * 8 = n * 2^3 = n << 3
}

  
unsigned int is_first_bit_same(unsigned int a, unsigned int b){
  return (~(a ^ b)) & 1u;
}

unsigned int is_first_bit_different(unsigned int a, unsigned int b){
  return (a ^ b) & 1u;
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

unsigned int lshift(unsigned int a, unsigned int b){
    //  a << b         if (b == 0) { return a; }        if (b == 32) { return 0; }
    unsigned int n = how_many_0_until_youngest_1(sizeof(b) << 3); // 5
    unsigned int mask = ~(((~0u) >> n) << n); // 00000000...00011111          a << 0 == a    a << 32 == 0
    unsigned int newshiftvalue = b & mask;  //     0000000...00100000       if (b == bitsize) { newshiftvalue = 0; } else { newshiftvalue = b % bitsize; }  
    // printf("\t(n = %u, b & mask = %u)", n, newshiftvalue);
    // printfint("mask:", &mask, 'd');
    // printfint("b & mask", &newshiftvalue, 'd');
    if ((~mask) & b) { return 0; }    // if (b >= bitsize) { return 0; }
    return a << newshiftvalue; // a << 0 ; => a
}


unsigned int is_ith_bit_same(unsigned int a, unsigned int b, unsigned int i){//2
  return (~((a ^ b) >> i)) & 1u; 
}

unsigned int is_ith_bit_different(unsigned int a, unsigned int b, unsigned int i){//2
  return ((a ^ b) >> i) & 1u; 
}


unsigned int compare(unsigned int a, unsigned int b){ 
//   unsigned int h = 0;
//   while(compare_ith_bit(a, b, h)){ ++h; }
//   unsigned int c;
//   c = (compare_ith_bit(h, byte2bit(sizeof(a)), how_many_0_until_youngest_1_until_youngest_1(byte2bit(sizeof(h))) ));
  return !(a ^ b);
}

unsigned int condition(unsigned int a){
  return !(!a);
}


unsigned int set_ith_bit_to_1(unsigned int a, unsigned int i){ 
  return a | (1u << i);
}


unsigned int set_last_bit_to_1(unsigned int x){ // 2
  return x | 1u;
}


char eldest_bit_is_1(unsigned int a){
  return a >> (byte2bit(sizeof(a)) - 1);
}


int division_of_negative_numbers_by_2(int a){// -6(1010)  
  int max_positive_integer = (~0) >> 1;
  int mask = ~max_positive_integer;
  return mask | ((max_positive_integer - (max_positive_integer - (mask ^ a) + 1) >> 1) + 1);
}


unsigned int convert_to_sign_and_magnitude(int a){
  int max_positive_integer = (~0) >> 1;
  int mask = ~max_positive_integer;
  return (max_positive_integer - (mask ^ a) + 1);
}


int convert_from_sign_and_magnitude(unsigned int a){

  int max_positive_integer = (~0) >> 1; // 0111111....    10000000000...
  int mask = ~max_positive_integer;
  a = a & max_positive_integer;
  return (mask | (max_positive_integer - a) + 1);
}

void swap_in_place_xor(int* a, int* b){// through xor operator
  if((long int)a & (long int)b){ return; }// TODO: THINK OVER TO WHAT TYPE NEED CAST
  (*a) = (*a) ^ (*b);// xor of couple numbers like this 0101 and 0110 equal to 0011
  (*b) = (*a) ^ (*b);// then xor of result with any of these two numbers will result to another number, ex_1:0011 ^ 0101 = 0110
  (*a) = (*a) ^ (*b);// ex_2: 0011 ^ 0110 = 0101
}