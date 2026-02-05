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

#include <stdio.h>
#include <stdlib.h>
#define uint unsigned int
/*2599
Description

有一楼梯共M级，若每次只能跨上一级或二级，要走上第M级，共有多少种走法？

-----------------------------------------------------------------------------

Invalid Word（禁用单词）错误：在解决这个题目时，某些关键词是不允许被使用的。如果提交的程序中包含了下列的关键词之一，就会产生这个错误。

被禁用的关键字：for, while, do, break, continue, goto。

Input

输入数据每行包含一个整数M（1<=M<=30）,表示楼梯的级数，至EOF结束。

Output

对于每个测试实例，请输出不同走法的数量

Sample Input
1
2
3
Sample Output
1
2
3
*/

uint steps(uint n){
    return n < 4 ? n : steps(n - 1) + steps(n - 2);
}

int main(){
    uint n;
    while(scanf("%u", &n) ^ EOF){
        printf("%u\n", steps(n));
    }
}

/*
how were hyperbolic functions derived
https://www.ms.uky.edu/~droyster/courses/fall11/MA341/Classnotes/Lecture%2027%20Handouts.pdf
https://www.cuemath.com/calculus/hyperbolic-functions/
https://math.stackexchange.com/questions/718863/geometric-meanings-of-hyperbolic-cosine-and-sine
https://math.stackexchange.com/questions/455480/how-were-hyperbolic-functions-derived-discovered
https://www.cuemath.com/geometry/latus-rectum/
*/