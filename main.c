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
// #include "raylib.h"

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

/*
yay -S ivpn
yay -S ivpn-ui
*/

#define PI 3.14159

struct PointIn2D{ float x, y; }typedef Vec2;


// What is the problem with this function?
Vec2* circle_points(Vec2 orig, uint32_t r, uint32_t n){
    if((n > 360) | !n){ return (Vec2*)0; }
    Vec2* arr = malloc(n * sizeof(Vec2));
    float ang = 0, inter = (double)(2 * PI) / n;
    for(int i = 0; i < n; i++, ang += inter){
        arr[i].x = r * cos(ang) + orig.x;
        arr[i].y = r * sin(ang) + orig.y;
    }
    return arr;
}

int main(){
    char fname[12] = "frame00.ppm"; 
    matrix_t m;
    uint32_t row = 256, col = 256;
    int a = 1;
    while(a <= 16){
        m = matrix_create(UINT32, row, col);
        pix_from_k_to_m_color(m, (uint32_bytes){ .parts = (four_uint8_struct){0,0,0}}, 0, row * col);
        Vec2* res = circle_points((Vec2){row >> 1, col >> 1}, 100, a);
        for(int i = 0; i < 16; i++){
            straight_line_thr_two_points(m, (uint32_bytes){ .parts = (four_uint8_struct){255, 255, 255}}, 128, 128, (int)res[i].y, (int)res[i].x);
        }
        fname[6] = '0' + (a % 10);
        fname[5] = '0' + a / 10;
        file_filler(fname, m);
        free(res);
        free(m.type);
        ++a;
    }
}
/*
PRIMITIVE PHYSICS ENGINE ELEMENTS:
    Objects
        Dot
            X
            Y
            COLOR
            ... (possibly other values, like denisty, charge, mass, velocity, acceleration, ...)
        Circle
            X
            Y
            R
            COLOR
            COLOR_OUTLINE? (possibly)
            is_filled
            has_outline?
            ... (possibly other values, like bouncyness, material, ...)
        Rectangle
            X
            Y
            W
            H
            COLOR
            COLOR_OUTLINE?
            is_filled
            has_outline?
            ... (possibly other values, ...)
        Line
            X
            Y
            Width
            COLOR
        Triangle
            ?
        FIELD
            VECTOR FIELD (DISCRETE FIELD)
                sub-matrix which has values in each cell
            CONTINUOUS FIELD
                f(float x, float y) - a function which gives you a smooth value given the smooth input
    COLLISION DETECTION - FUNCTIONS THAT CHECK FOR INTERSECTIONS!
        If dot and dot are in the same place?
        If dot is within a circle
        if dot is within a rectangle
        ...
        if circle and circle are intersecting
        ...
    PHYISCS_LOOP - logic of what's happing
        1) objects are created and destroyed
        2) they are checked for collision against each other
        3) you decide what happens when they collide
        4) advance time forward to the next iteration

EXAMPLE of a physics loop:
    1) IF there are no objects, create a dot, else do nothing.
    2) ...
    3) IF (dot.x > width || dot.x < 0) AND (dot.y > height || dot.y < 0) { dot.x = width/2; dot.y = 0; }
       ELSE {
        dot.velocity.x += dot.acceloration.x;
        dot.velocity.y += dot.acceloration.y;
        dot.x += dot.velocity.x;
        dot.y += dot.velocity.y;
         } [3;3]/frame
    4) time++;
    RENDER(); // write ppm with filename_time.ppm

    after the loop: write a script which runs ffmpeg automatically, and delete all frames

*/