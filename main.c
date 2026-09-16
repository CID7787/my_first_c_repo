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

/*
yay -S ivpn
yay -S ivpn-ui
*/

void dropping_circles(){
    uint32_t row = 1028, col = 1028, i = 0;
    matrix_t m = matrix_create(UINT32, row, col);
    uint32_t vel[2] = {0,0}, acel[2] = {3, 0};
    char frame_name[] = "frame00.ppm";
    circle_t cir = (circle_t){ .obj = (object_t){ .x = 0, .y = row >> 1, .veloc = vel, .accelor = acel, .color = (uint32_bytes){ .parts = (four_uint8_struct){255, 160, 0} } }, .rad = 10 };
    while((cir.obj.x + cir.rad) < col){
        draw_pix_from_k_to_m_color(m, (uint32_bytes){ .parts = (four_uint8_struct){40, 160, 90}}, 0, row * col);
        draw_circle(m, cir.obj.color, cir.obj.y, cir.obj.x, 10);
        cir.obj.veloc[0] += cir.obj.accelor[0];
        cir.obj.veloc[1] += cir.obj.accelor[1];
        cir.obj.x += abs(cir.obj.veloc[0]) < cir.obj.x ? cir.obj.veloc[0] : -(int32_t)cir.obj.x + cir.rad;
        cir.obj.y += abs(cir.obj.veloc[1]) < cir.obj.y ? cir.obj.veloc[1] : -(int32_t)cir.obj.y + cir.rad;
        i++;
        // printf("%u , %u      %d, %d\n", cir.obj.x, cir.obj.y, cir.obj.veloc[0], cir.obj.veloc[1]);
        // if(i > 20){ break; }
        printf("%d  ", i);
        frame_name[6] = (i % 10)   + '0';
        frame_name[5] = (i / 10)   + '0';
        // frame_name[6] = (i / 100)  + '0';
        // frame_name[5] = (i / 1000) + '0';
        // frame_name[5] = (i % 100000) / 10000;
        file_filler(frame_name, m);
    }
    free(m.type);
    // system("ffmpeg -framerate 4 -i frame%05d.ppm vid.mp4");
}

typedef struct linked_list_type{
    uint32_t val;
    struct linked_list_type *next;
} link_list;

void data_str_practical_first_ques(){   
    link_list  list1 = (link_list){ .val = 212}, list2, *list3, *llist = list3 = malloc(sizeof(link_list));
    llist->next = &list1;
    uint32_t value, min_one = -1, i = 0;
    while(i++ < 2){
        printf("start: ");
        value = 1;
        while(value ^ min_one){
            scanf("%u", &value);    
            llist->next->val = value;
            llist->next->next = malloc(sizeof(link_list));
        }
        free(llist->next->next);
        if(i & 1) llist = &list2; 
    }
    free(list3);
}

int main(){
    dropping_circles();
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