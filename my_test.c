// Online C compiler to run C program online
#include <stdio.h>

typedef union variable_all_types{
  double d;
  long int li;
  long unsigned int lui;
  char c;
  float f;
  int i;
  unsigned int ui;
} all;

void print_binary_with_arrays(all x, unsigned int byte_amount, unsigned char* whitespace_after_bit_indexes, unsigned int array_size){// whitespace indexes have absolute index starting from leftmost bit
    if(!whitespace_after_bit_indexes){ return; }
    if(!byte_amount){ return; }
    unsigned int i = sizeof(all);
    if(byte_amount <= i){ i = byte_amount; } else { printf("The number of bytes is larger than there are bytes in the variable \n"); }
    i <<= 3;
    while(i-- > 0){
        unsigned int index = 0;
        while(index < array_size){
            if(i == whitespace_after_bit_indexes[index++]){ printf(" "); break; }
        }
        printf("%u", (x.lui >> i) & 1);
    }
}


int main() {
    long unsigned int uint_nan = 0b01111111100000000000000000000000;
    float f_nan = *(float*)&uint_nan;
    double d_fnan = f_nan;
    uint_nan = *(double*)&d_fnan;
    printf("uint_nan value: %lu\n", uint_nan);
    printf("double representation fnan value binary: %064b", d_fnan);
    return 0;
}