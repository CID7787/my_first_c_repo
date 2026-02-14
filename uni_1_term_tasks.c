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

#include <stdio.h>
#include <stdlib.h>
#define uint unsigned int
#define uch unsigned char


 
double mabs(double x){ return (x < 0)? -x : x; } 
 
int n_root(void) {
    double num = 625;
    int rootDegree = 2;
 
    printf("Число а = %f\n", num); 
    printf("Степень n = %d\n", rootDegree); 
    
    double eps = 0.00001;  
    double root = num / rootDegree; 
    double rn = num;                
    int countiter = 0;                 
    while(mabs(root - rn) >= eps){
        rn = num;
        for(int i = 1; i < rootDegree; i++){
            rn = rn / root;
        }
        root = 0.5 * ( rn + root);
        countiter++;
    }
    printf("root = %f\n", root); 
    printf("Число итераций = %i\n", countiter); 
    
    return 0;
}