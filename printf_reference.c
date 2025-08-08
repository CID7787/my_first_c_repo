// printf() needs <stdio.h>
void printf_reference(){
  // This is a comment
  /*
      This is a multiline comment
      Look, this comment is a multiline
  */
  // data types of variables:
  
  "This is text, sometime referred to as a text string, or string for short.";
  printf("The printf() function is a part of <stdio.h> library, and it allows you to print text like this.\n");
  printf("Newline: \n");
  printf("\nTabulation: \t");
  printf("\nBackslash: \\");
  printf("\nDouble quotation mark: \"");

  printf("\nCharacter: %c", '1');
  printf("\nCharacter: %c", 48);
  printf("\nInteger: %d", -1);
  printf("\nUnsigned integer: %u", -1);
  printf("\nLong integer: %ld", -1l);
  printf("\nLong unsigned integer: %lu", -1lu);
  printf("\nFloating-point Number: %f", -1.25f);
  printf("\nFloating-point Number: %f", -10.25);
  printf("\nScientific notation: %f", -10.25);
  printf("\nHex float notation: %a", -10.25);
  printf("\nBinary representation (unsigned): %b", 5);
  printf("\nOctal representation (unsigned): %o", 5);
  printf("\nHexadecimal representation (unsigned): %x", 255);
  printf("\nString: %s", "This is a parameter string");
  char a = 0;
  printf("\nPointer address: %p", &a); // this prints the address of the variable
  printf("\nWhen you need to print a percent symbol, do this: %%", '%');

  printf("\nprintf() can accept multiple arguments: %c %d %e", 'a', -25, -12.3);
  printf("\nprintf() just tells you how to print the data, it doesn't force data types. You can print signed integer as unsigned integer: %u", -30);

  // See more printf format specifiers here: https://cplusplus.com/reference/cstdio/printf/
}

void arithmetic_types_reference(){
  int integer = 10; // a whole number with a sign (sign is the first bit), typically represented with two's complement (дополнительный код)
  unsigned int unsigned_integer = 10u; // a whole number without a sign, represented with 0s complement (прямой код)
  long int long_integer = 10l; // same as int, but has 2x the memory
  long unsigned int long_unsigned_integer = 10ul; // same as unsigned int, but has 2x the memory
  long long int long_long_integer = 10; // same as int, but has even more memory. Part of the standard since C99
  short int short_integer = 10; // same as int, but has half the memory

  char character = '1'; // a char is an integer in 1 byte, it's value is interpreted from the ASCII table: https://www.asciitable.com/
  char c = 48; // because char is an integer, you can directly assign it integer values (as long as they fit in 1 byte), and it will work just fine
  c = c + 1; // because char is an integer, you can perform arithmetic operations with it

  float floating_point_number = 10.0f;
  double double_precision_floating_point_number = 10.0;

  // learn more about arithmetic types here: https://en.cppreference.com/w/c/language/arithmetic_types.html
}


void pointer_types_reference(){
  int a = 0;
  int* pointer_to_address_of_a = &a;
  int static_integer_array[4] = {1, -2, 30, 4};
  int static_integer_array_with_undefined_size[] = {1, 2, 3, 4, 5, 21};
  int *pointer_to_a_memory = (int*)static_integer_array_with_undefined_size;// pointer decay
  


  // All pointer types have the same length, which is the length of the
  void* pointer_to_a_memory_with_unknown_type = (void*) pointer_to_address_of_a;

}
