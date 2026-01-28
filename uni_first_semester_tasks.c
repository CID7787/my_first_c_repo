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

llui my_pow(llui b, llui pow){
    llui r = 1;
    while(pow--)
        r *= b;
    return r;
}

/*2422
Description:
 求1*2*3*……*n的乘积以多少个0结尾。

Input:

输入为多行，至EOF结束。每行为一个整数n，n<=100000。

Output:

输出每个n对应的结尾0的个数。
*/

int main(){
    llui n, p, r, num;
    while(scanf("%llu", &n) ^ EOF){
        r = 0, p = 1;
        num = my_pow(5, p);
        while(n / num){
            r += n / num; 
            num = my_pow(5, ++p);
        }
        printf("%llu\n", r);
    }
}