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

void matrix_mult(int *a, int *b, int *c, uint ar, uint cr, uint bc){
    uint i, i1, i2;
    for(i = 0; i < bc; i++)
        for(i1 = 0; i1 < ar; ++i1){
            c[i1 * cr + i] = 0;
            for(i2 = 0; i2 < cr; ++i2){
                printf("%d + ", c[i1 * cr + i]);
                c[i1 * cr + i] += a[i1 * cr + i2] * b[i2 * bc + i];
                printf("%d * %d = %d\n", a[i1 * cr + i2], b [i2 * bc + i], c[i1 * cr + i]);
            }
        }
}

void print(int *mat, uint r, uint c){
    uint i1, i2;
    for(i1= 0; i1 < r; i1++){
        for(i2 = 0; i2 < c; i2++)
            printf("%d ", mat[i1 * c + i2]);
        printf("\n");
    }
}

int main(){
    uint dim[2], rc, i, i1, c;
    while(scanf("%u%u%u", dim, &rc, dim + 1) ^ EOF){
        int *matrix[3] = { calloc(dim[0] * rc, sizeof(int)), calloc(dim[1] * rc, sizeof(int)), calloc(dim[0] * dim[1], sizeof(int))};
        for(i = 0; i < 2; ++i){
            c = dim[i] * rc;
            for(i1 = 0; i1 < c; ++i1)
                scanf("%d", matrix[i] + i1);
        }
        matrix_mult(matrix[0], matrix[1], matrix[2], dim[0], rc, dim[1]);
        print(matrix[2], dim[0], dim[1]);
        free(matrix[0]);
        free(matrix[1]);
        free(matrix[2]);
    }
    return 0;
}

/*
how were hyperbolic functions derived
https://www.ms.uky.edu/~droyster/courses/fall11/MA341/Classnotes/Lecture%2027%20Handouts.pdf
https://www.cuemath.com/calculus/hyperbolic-functions/
https://math.stackexchange.com/questions/718863/geometric-meanings-of-hyperbolic-cosine-and-sine
https://math.stackexchange.com/questions/455480/how-were-hyperbolic-functions-derived-discovered
https://www.cuemath.com/geometry/latus-rectum/
*/