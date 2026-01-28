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
/*1096
Description

向标准输出上打印一些用ASCII字符组成的图形。
Input

输入为多个整数n，0<n<100。当n为0时结束输入。
Output

若n为偶数，则输出一个正向的n层等腰三角形；n为奇数，则输出一个倒向的n层等腰三角形。三角形由“+”组成。任意两个图形之间有一个空行分隔，格式见sample。
*/

void print_char(int i1, char c){
    if(i1 > 0){ printf("%c", c); }
    if(--i1 > 0){ print_char(i1, c); }
}

void print_triangle_from_up_to_down(uint i, uint n, char c){
    print_char(n - i,' ');
    print_char(i, c);
    print_char(i - 1, c);
    printf("\n");
    if(i++ < n){ print_triangle_from_up_to_down(i, n, c); }
}

void print_triangle_from_down_to_up(uint i, uint n, char c){
    print_char(n - i, ' ');
    print_char(i, c);
    print_char(i ? i - 1 : 0, c);
    printf("\n");
    if(--i){ print_triangle_from_down_to_up(i, n, c);}
}

void up_down_tri_handler(uint n){
    if(n & 1){ print_triangle_from_down_to_up(n, n, '+'); }
    else{ print_triangle_from_up_to_down(1, n, '+'); }
}

int main()
{
    uint n;
    scanf("%u", &n);
    while(n){
        up_down_tri_handler(n);
        printf("\n");
        scanf("%u", &n);
    }
}