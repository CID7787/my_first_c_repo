#include <stdio.h> 
#include <time.h>  
#include <stdlib.h>
#include <math.h>  
#include <stdint.h> 
#define headerfile 1

// #include "constants.c"
// #include "user_defined_datatypes.c"
// #include "logical_functions_of_decision.c"
// #include "additional_functions.c"
// #include "bitwise_functions.c"
// #include "type_cast_functions.c"
// #include "safe_arithmetic_functions.c"
// #include "image_functions.c"
// #include "new_vector_functions.c"
// #include "matrix_functions.c"
// #include "print_binary.c"


typedef struct ListNode{
    uint32_t val;
    struct ListNode *next_n;
} node;

void print_list(node* head){
    if(head->next_n){
        printf("%u", head->next_n->val);
        head = head->next_n;
    }
    else{
        puts("NULL");
        return;
    }
    while(head->next_n){
        printf(" %u", head->next_n->val);
        head = head->next_n;
    }
}

node* concat(node* list1_h, node* list2_h){
    node *res = (node*)malloc(sizeof(node)),
        **node_arr = (node*[]){list1_h->next_n, list2_h->next_n},
         *itr_node = res;
    uint8_t index_cond;
    while(node_arr[0] && node_arr[1]){
        index_cond = node_arr[0]->val > node_arr[1]->val;
        itr_node = (itr_node->next_n = node_arr[index_cond]);
        node_arr[index_cond] = node_arr[index_cond]->next_n;
    }
    itr_node->next_n = node_arr[node_arr[1] != 0];    
    return res;
}


void call_func(){
    node *list1_h = (node*)malloc(sizeof(node)), 
         *list2_h = (node*)malloc(sizeof(node)),
         *list3 = list1_h;
    uint32_t value;
    list1_h->val = list2_h->val = 0;
    scanf("%u",  &value);
    while(value ^ -1){
        list3->next_n = (node*)malloc(sizeof(node));
        list3->next_n->val = value;
        list3 = list3->next_n;
        ++list1_h->val;
        scanf("%u", &value);
    }
    list3->next_n = 0;
    list3 = list2_h;
    scanf("%u",  &value);
    while(value ^ -1){
        list3->next_n = (node*)malloc(sizeof(node));
        list3->next_n->val = value;
        list3 = list3->next_n;
        ++list2_h->val;
        scanf("%u", &value);
    }
    list3->next_n = 0;
    print_list( concat(list1_h, list2_h) );
}

int main(){
    call_func();
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