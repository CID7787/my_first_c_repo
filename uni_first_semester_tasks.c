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
/*1456
Description

验证“每个不小于6的偶数都是两个奇素数之和”，输入一个不小于6的偶数n，找出两个奇素数，使它们的和为n。

Input

输入一个不小于6的偶数n，n<10000。

Output

找出两个奇素数，使它们的和为n。如果有多种答案，则每个答案占一行。注意：在输出时，第一个数不能大于第二个数。比如对于10，输出的答案只有3 + 7 = 10，没有7 + 3 =10。

每行的输出格式为：

a + b = c

a和b是满足条件的两个数，a<=b，c为输入的数。

多个答案的输出顺序，要按照第一个数递增的顺序输出。
*/


uint bit_arr[350] = {0};

uint is_prime(uint n){
    if(n < 2){ return 0; }
    uint i = 2, f = 1;
    while(((i * i) <= n) && f){
        f = n % i;
        ++i;
    }
    return f;
}

void sorted_prime_arr(uint n, uint *primes){
    uint c = 1, i = 0, f = 1;
    while((++c < n) && f){
        if(is_prime(c))
            if(is_prime(n - c) && (n - c >= c))
            primes[i++] = c; 
    }
    primes[i] = 0;
}


int main(){
    uint n, i;
    scanf("%u", &n);
    uint sol_arr[n >> 1];
    sorted_prime_arr(n, sol_arr);
    for(i = 0; sol_arr[i]; i++){
        printf("%u + %u = %u\n", sol_arr[i], n - sol_arr[i], n);
    }
}