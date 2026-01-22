#include <stdio.h>
#include <math.h>
int is_primer(int number){
    int flag = 1;
    if(number <= 1){ return 0;}
    if(number == 2){ return 1; } 
    for(int i = 2; (i < number) && flag; i++){
        flag = number % i;
    }
    return flag; 
}

int main()
{
    int num;
    while(scanf("%d", &num) != EOF)
    {
        if(is_primer(num))
            puts("YES");
        else
            puts("NO");
    }
    return 0;
}