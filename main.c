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

#define color(r, g, b) (uint32_type){ .parts = (uint32_bytes){(r), (g), (b)}}


void circle_test(){
    uint32_t row = 2000, col = 1700, i = 0, rad, toler;
    scanf("%u%u", &rad, &toler);
    matrix_t mat = matrix_create(UINT32, row, col);
    int32_t *acc = (int32_t[]){0, 2}, *vel = (int32_t[]){ 0, 0 };
    circle_t cir = (circle_t){ .obj.x = col / 2, .obj.y = 0, .obj.accelor = acc, .obj.veloc = vel, .obj.color = color(0, 130, 70), .filled = 1, .outline_col = color(255, 0, 0), .rad = rad };
    char file_name[11] = "frame00.ppm";
    while(cir.obj.y + cir.rad <= row){
        
        draw_pix_from_k_to_m_color(mat, color(0,0,0), 0, row * col);
        draw_circle(mat, cir.obj.color, cir.obj.y, cir.obj.x, cir.rad);
        draw_ring(mat, cir.outline_col, cir.obj.y, cir.obj.x, cir.rad, toler);

        cir.obj.veloc[1] += cir.obj.accelor[1];
        cir.obj.y += cir.obj.veloc[1];        
        
        file_name[6] = (i % 10) + '0';
        file_name[5] = (i / 10) + '0';
        printf("%s\n", file_name);
        // file_name[5] = (i % 1000) / 100;
        file_filler(file_name, mat);
        i++;
    }
    system("rm vid.mp4 | ffmpeg -framerate 3 -i frame%02d.ppm vid.mp4 && rm *.ppm &&  xdg-open /home/cid_0/Desktop/code_runner/my_first_c_repo/vid.mp4 ");
}


int main(){
    // circle_test();
    matrix_t m = matrix_create(UINT32, 300, 300);
    draw_pix_from_k_to_m_color(m, color(255, 255, 255), 0, 90000);
    draw_line_at_angle_a(m, color(0,0,0), 150, 150, PI / 4, 100);
    file_filler("test.ppm", m);
    system("xdg-open /home/cid_0/Desktop/code_runner/my_first_c_repo/test.ppm");
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