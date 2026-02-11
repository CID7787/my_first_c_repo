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

/*1245
Description

“Hanoi Tower”问题的背景和搬移规则大家是否都很熟悉了？为了突出重点，我把问题描述放在下面的HINT部分，不了解的同学可以参考。

首先我们Hanoi塔上的盘子按从上到下编号，假设Hanoi塔上有n个盘子，那么最小的那个盘子就是1号盘子，然后是2号、3号……最大的盘子是n号。

Hanoi塔的3根针我们也进行编号，最左边的是1号，中间的是2号，最右边的是3号。

如果我们想把n=2个盘子从1号针搬到2号针，那么3号针作为暂存使用。整个搬移过程是这样的：

1号盘子：从1号针搬到3号针
2号盘子：从1号针搬到2号针
1号盘子：从3号针搬到2号针

你的任务是编个程序把上面的搬移过程输出来，程序需要输入盘子的个数n，并且这n个盘子一开始在哪根针，要搬到哪根针都是从输入得到的。

Input

输入为多行，至EOF结束。

每行输入四个整数，第一个整数为盘子数n（1<=n<=10），后面的三个整数是三根针的编号，它们排列的顺序是有不同含义的：第二个整数是n个盘子一开始的位置，第四个整数是盘子最终要放置的位置，第三个整数是搬移过程中用来暂存盘子的。
如：
输入“1 2 3 1”表示只有一个盘子，从第2根针搬到第1跟针上。

Output

每一行输入都对应一个搬移过程，首先输出一个“case i”，表示对应的第i个输入。然后再它后面输出搬移的步骤。如：

输入“1 2 3 1”表示只有一个盘子，从第2根针搬到第1跟针上。那么它的搬移步骤只有一步：把1号盘子从第2跟针搬到第1跟针，即输出：

plate 1 : from 2 to 1
*/



uint circle3(uint num){ return num % 3 ? num % 3 : 3; }  

uint move_n_pl(uint pl){
    uint a = ~pl;
    a &= ++pl;
    for(pl = 0; a; ++pl) 
        a >>= 1;
    return pl;
}

int main(){
    uint cases = 1, till, i, i1, i2, n, pl, stick[3];
    uch step1, step2, *stick_pl[4];
    while(scanf("%u%u%u%u", &pl, stick, stick + 1, stick + 2) ^ EOF){
        printf("case %u :\n", cases);
        for(i = till = 0; i < pl; ++i){ till |= (uch)1 << i; }
        step1 = 1 + ((pl & 1 && circle3(stick[0] + 2) == stick[2]) | (!(pl & 1) && circle3(stick[0] + 1) == stick[2]));
        ++pl;
        for(i = 1; i < 4; ++i){ stick_pl[i] = calloc(pl, sizeof(uch)); }
        for(i = 1; i < pl; ++i){ stick_pl[stick[0]][i] = i; }
        i = 0;
        while(i < till){
            step2 = step1;
            n = move_n_pl(i++);
            for(i1 = 1; !stick_pl[i1][n]; ++i1){ }
            pl = circle3(i1 + step1);
            for(i2 = 1; stick_pl[pl][i2] < stick_pl[i1][n]; ++i2){ 
                if(stick_pl[pl][i2]){ step2 += step1; break; } 
            }
            pl = circle3(i1 + step2);
            stick_pl[pl][n] = stick_pl[i1][n];
            stick_pl[i1][n] = 0;
            printf("   plate %u : from %u to %u\n", n, i1, pl);
        }
        for(i = 1; i < 4; ++i){ free(stick_pl[i]); }
        cases++;
    }
    return 0;
}
