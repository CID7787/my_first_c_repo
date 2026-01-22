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
long long unsigned int comb(int n, int k){ // n! / (k! * (n - k)!)
    if(k > n){ return 0; }
    if(k < 1){ return 1; }
    return (long long unsigned int)comb(n - 1, k - 1) + comb(n - 1, k);
}
*/

// very important: memory layout, virtual machine, c99 compiler either on comp or online(source that will be available no matter where or through what use it), how to compile and execute program in Windows

#include <stdlib.h>
#include <stdio.h>
#define uint unsigned int

const uint N = 10;

void char_draw_data(char *arr, uint n){
    for(uint i = 0; i < n*n; i++){
        uint row = i / n; // 1 / 4 = 0
        uint col = i % n; // 1 % 4 = 1
        if (!col) { printf("%d ", row);} // print 0
        printf("%c ",  arr[i]);
        if (col + 1 == n) { printf("\n"); }
    }
}

void char_draw_pretty(char *arr, uint n){
    uint i;
    printf(" _");
    for(i = 0; i < n; i++){ printf("__"); }
    printf("\n");
    for(i = 0; i < n*n; i++){
        uint row = i / n;
        uint col = i % n;
        if (!col) { printf("| ", row);} // print 0
        printf("%c ", arr[i]);
        if (col + 1 == n){ printf("|\n"); }
    }
    printf(" -");
    for(i = 0; i < n; i++){printf("--"); }
    printf("\n");
}


void walls(uint m, uint len){
    if(m < len){ printf("|"); walls(++m, len); }
}

void roof_bott(uint m, uint len){
    if(m < len){ printf("__"); roof_bott(++m, len); }
}

void up_rec(uint i, uint n){
    if(i){
        uint m = 0;
        walls(m, n - i);
        printf(" ");
        roof_bott(m, i);
        printf(" ");
        walls(m, n - i);
        printf("\n");
        up_rec(--i, n);
    }
}
void down_rec(uint i, uint n){
    if(i < n){
        uint m = 0;
        walls(m, n + 1 - i);
        roof_bott(m, i);
        walls(m, n + 1 - i);
        printf("\n");
        down_rec(++i, n);
    }
}

int main(){
    uint n = 4, i = 0;
    scanf("%u", &n);
    i = n;
    up_rec(i, n);
    i = 1;
    down_rec(i, n);
}


// void f1(){ printf("int"); }
// void f2(){ printf("double"); }

// void* my_if(int cond, void* (*true_cond_func)(), void* (*false_cond_func)()){
//     cond = !(!cond); // 1; 0
//     void* t = true_cond_func; // convert function pointer to data pointer
//     void* f = false_cond_func;
//     int big = cond << (sizeof(int) << 1); // 0/0010000....
//     int r = (~0) >> big; // 00000000... | 11111111......
//     void* result = (void*)((r & (long long int)t) | (r & (long long int)f)); // leave only one data pointer according to IF/OR logic, based on cond value
//     void* (*resulting_function)() = (void* (*)())result; // convert back to the function pointer
//     resulting_function(); // call a function
// }
