/*   links:
https://stackoverflow.com/questions/1782080/what-is-eof-in-the-c-programming-language
https://en.cppreference.com/w/cpp/io/c/std_streams
https://en.cppreference.com/w/cpp/io/c/FILE.html
https://en.cppreference.com/w/c/io/gets
https://en.cppreference.com/w/c/io/fgets
https://www.geeksforgeeks.org/c/fgets-function-in-c/
https://www.geeksforgeeks.org/c/eof-and-feof-in-c/
https://stackoverflow.com/questions/28655061/reading-binary-pe-files-in-c
https://stackoverflow.com/questions/54162152/what-precisely-does-the-g-printf-specifier-mean
https://yandex.com/search/?text=C+file+stream&lr=202
https://www.geeksforgeeks.org/c/read-a-file-line-by-line-in-c/
https://yandex.com/search/?text=type+of+constants+in+c&lr=202
https://en.cppreference.com/w/c/language/statements.html
https://en.cppreference.com/w/c/io/fscanf.html
https://yandex.com/search/?text=c+fgets&lr=202

*/

/* observe the principle of this recursion
#define lluint long long unsigned int
lluint comb(int n, int k){
    if(k > n){ return 0; }
    if(k < 1){ return 1; }
    return (lluint)comb(n - 1, k - 1) + comb(n - 1, k);
}
*/

// very important: memory layout, virtual machine, c99 compiler either on comp or online(source that will be available no matter where or through what use it), how to compile and execute program in Windows

/*
void* my_if(int cond, void* (*true_cond_func)(), void* (*false_cond_func)()){
    cond = !(!cond); // 1; 0
    void* t = true_cond_func; // convert function pointer to data pointer
    void* f = false_cond_func;
    int big = cond << (sizeof(int) << 1); // 0/0010000....
    int r = (~0) >> big; // 00000000... | 11111111......
    void* result = (void*)((r & (long long int)t) | (r & (long long int)f)); // leave only one data pointer according to IF/OR logic, based on cond value
    void* (*resulting_function)() = (void* (*)())result; // convert back to the function pointer
    resulting_function(); // call a function
}
*/


#include <stdio.h>
#define uint unsigned int
#define llui long long unsigned int

/*1143
Description

向标准输出上打印一些用ASCII字符组成的图形。

-----------------------------------------------------------------------------

编写一个函数用于输出ASCII图形。

用C语言实现：append.c中函数原型为

int print_graphic(int n, char c);

用C++实现：append.cc中函数原型为

int printGraphic(int n, char c);

功能：输出n层有字符c组成的图形。

函数的调用格式见“Append Code”。

Input

输入为一个整数n和一个字符c，0<n<100。

Output

输出一个n层等腰三角形，由字符c组成，格式见sample。
*/

void print_char(int i1, char c){
    if(i1 > 0){ printf("%c", c); }
    if(--i1 > 0){ print_char(i1, c); }
}

void print(uint i, uint n, char c){
    print_char(n - i,' ');
    print_char(i, c);
    print_char(i - 1, c);
    printf("\n");
    if(i++ < n){ print(i, n, c); }
}

void print_graphic(uint n, char c){
    print(1, n, c);
}

int main()
{
    char c;
    int num;
    scanf("%d %c", &num, &c);
    print_graphic(num, c);
    return 0;
}