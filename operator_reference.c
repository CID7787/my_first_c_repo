void operator_reference(){
  int a, b;
  a = 1; // assignment operator
  
  a++; // post increment, returns the next number, executes after all other operations
  a--; // post decrement, returns the previous number, executes after all other operations
  ++a; // pre increment, returns the next number
  --a; // pre decrement, returns the previous number
//1++; // increment doesn't work on literals!



  a = a + 1;
  a += 1; // this is the same as a = a + 1
  a = a - 1;
  a -= 1; // this is the same as a = a - 1

  a = a * 2;
  a *= 2;
  a = a / 2; // this is integer division. It will return a whole part and disregard the remainder: 9 / 2 => 1
  a /= 2;
  a = a % 2; // this is remainder operator, also known as modulo operator, or "mod" for short. It will return a remainder: 9 % 2 => 4
  
  a == 1;
  a = (a == 1);
  a != 1;
  a = (a != 1);

  a > 1;
  a < 1;
  a >= 1;
  a <= 1;

  a = a << 1;
  a <<= 1;
  a = a >> 1;
  a >>= 1;

  a = a & 1;
  a &= 1;
  a = a | 1;
  a |= 1;
  a = a ^ 1;
  a ^= 1;

  a = ~a;

  a = a && 0;
  a = a || printf("False");
  a = a || 1;
  a = a && printf("True");

  a = a ? 1 : 0; // ternary operator
  a = a ? printf("true") : printf("false");

  a = sizeof(a); // sizeof() is an operator!!!

  a = (int)1.0;

  //TODO: finish this

  // order of operators: https://en.cppreference.com/w/c/language/operator_precedence.html
}