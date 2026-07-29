#include <stdio.h> 
#include <time.h>  
#include <stdlib.h>
#include <math.h>  
#include <stdint.h> 
#define headerfile 1

#include "constants.c"
#include "user_defined_datatypes.c"
#include "logical_functions_of_decision.c"
#include "additional_functions.c"
#include "bitwise_functions.c"
#include "type_cast_functions.c"
#include "safe_arithmetic_functions.c"
#include "image_functions.c"
#include "new_vector_functions.c"
#include "matrix_functions.c"
#include "print_binary.c"


/* type cast functions' tests
int to int, to uint, to float
uint to uint, to int, to float
float to float
*/

void f(matrix_t m){
    for(int r = 0, ri, gi,bi; r < m.row[0]; r++){
        printf("%03u %03u %03u", m.elements.ui8[(r * m.col[0]) << 2], m.elements.ui8[((r * m.col[0]) << 2) + 1], m.elements.ui8[((r * m.col[0]) << 2) + 2]);
        for(int c = 1; c < m.col[0]; c++){
            
            ri = ((r * m.col[0]) + c) << 2;
            gi = ri + 1;
            bi = ri + 2;
            printf("  %03u %03u %03u", m.elements.ui8[ri], m.elements.ui8[gi], m.elements.ui8[bi]);
        }
        puts("\n\n");
    }
}

int main(){    
    matrix_t m = matrix_create(UINT32, 2500, 2000);
    pix_from_k_to_m_color(m, (uint32_bytes){ .parts.b1 = 255, .parts.b2 = 255, .parts.b3 = 255}, 0, 5000000);
    ring(m, (uint32_bytes){ .parts.b1 = 0, .parts.b2 = 0, .parts.b3 = 0}, 500, 500, 100);
    file_filler("example.ppm", m);
    free(m.type);
    return 0;
}


/*Ask 'A' why compiler throws("free(): invalid size
    zsh: IOT instruction (core dumped)  ./main.exe ")
int main(){
matrix_t m = matrix_create(UINT32, 1, 10);
free(m.elements.ui32);
return 0;
}
*/

/*TODO LIST  : FMPG
graphic:
horizontal gradient:
vertical gradient:
diagonal gradient:
    top left corner: red = 0; green = 0; blue = 0; bottom right corner: red = 255;
circle:
*/
