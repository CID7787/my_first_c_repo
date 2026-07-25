#ifndef headerfile
    #include <stdio.h> 
    #include <stdlib.h>
    #include <stdint.h>
    #include "logical_functions_of_decision.c"
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "safe_arithmetic_functions.c"
#endif


void file_filler(const char *str, matrix_t pic){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    FILE* fptr = fopen(str, "w");
    int8_t *arr = malloc((pic.row[0] * pic.col[0] * 3) + 1);
    uint32_t i = 0, r, c, row = pic.row[0], col = pic.col[0];
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c++, i+=3){
            arr[i    ] = pic.elements.ui8[ ( ((r * col) + c) << 2 )    ];
            arr[i + 1] = pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 1];
            arr[i + 2] = pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 2];
            // in case of their value being zero(0)
            arr[i] += !arr[i];
            arr[i + 1] += !arr[i + 1];
            arr[i + 2] += !arr[i + 2];
        }
    }
    arr[i] = 0;
    fprintf(fptr, "P6\n%u %u\n255\n%s", pic.col[0], pic.row[0], arr);
    fclose(fptr);
    free(arr);
}



void left_side_color(matrix_t pic, uint32_bytes col_b){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t r, c, row = pic.row[0], col = pic.col[0], col_till = col >> 1; 
    for(r = 0; r < row; r++){
        for(c = 0; c < col_till; c++){ // col 
            pic.elements.ui8[ (((r * col) + c) << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[ (((r * col) + c) << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[ (((r * col) + c) << 2) + 2] = col_b.parts.b3;
        }
    } 
}

void right_side_color(matrix_t pic, uint32_bytes col_b){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t r, c, row = pic.row[0], col = pic.col[0]; 
    for(r = 0; r < row; r++){
        for(c = col >> 1; c < col; c++){ 
            pic.elements.ui8[(((r * col) + c ) << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(((r * col) + c ) << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(((r * col) + c ) << 2) + 2] = col_b.parts.b3;
            printf("%u %u %u   ", pic.elements.ui8[(((r * col) + c ) << 2)    ], pic.elements.ui8[(((r * col) + c ) << 2) +1], pic.elements.ui8[(((r * col) + c ) << 2)+2]);
            if(c == (col - 1)){ printf("\n\n"); }
        }
    } 
}   



void top_side_color(matrix_t pic, uint32_bytes col_b){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t r, c, row = pic.row[0] >> 1, col = pic.col[0]; 
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c++){ // col 
            pic.elements.ui8[(((r * col) + c ) << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(((r * col) + c ) << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(((r * col) + c ) << 2) + 2] = col_b.parts.b3;
        }
    } 
}  

void down_side_color(matrix_t pic, uint32_bytes col_b){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t r, c, row = pic.row[0], col = pic.col[0];
    for(r = row >> 1; r < row; r++){
        for(c = 0; c < col; c++){
            pic.elements.ui8[(((r * col) + c ) << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(((r * col) + c ) << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(((r * col) + c ) << 2) + 2] = col_b.parts.b3;
        }
    } 
}



void even_pix_color(matrix_t pic, uint32_bytes col_b){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t i, n = pic.row[0] * pic.col[0];
    for(i = 0; i < n; i++){
        if((i ^ 1) & 1){
            pic.elements.ui8[(i << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(i << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(i << 2) + 2] = col_b.parts.b3;    
        }
    }
}

void odd_pix_color(matrix_t pic, uint32_bytes col_b){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t i, n = pic.row[0] * pic.col[0];
    for(i = 0; i < n; i++){
        if(i & 1){
            pic.elements.ui8[(i << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(i << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(i << 2) + 2] = col_b.parts.b3;    
        }
    }
}



void vertical_line_width_n_offset_k(matrix_t pic, uint32_bytes col_b, uint32_t width, uint32_t offset, uint32_t from_row, uint32_t till_row){// width add up on offset value so vertical line will start from offset value and till offset + width draw it
    if(!(pic.elements.ui8 && pic.row && pic.col)){ 
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t c, row = pic.row[0], col = pic.col[0], right_b;
    offset = ternary(offset > col, col - !!col, offset);
    right_b = offset + ternary((col - offset) >= width, width, col - offset - !!col);
    from_row = ternary(from_row > row, row, from_row);
    till_row = ternary(till_row > row, row, till_row);
    if(from_row > till_row){ from_row ^= till_row; till_row ^= from_row; from_row ^= till_row; }
    for( ; from_row < till_row; from_row++){
        for(c = offset; c <= right_b; c++){
            pic.elements.ui8[ ( ((from_row * col) + c) << 2 )    ] = col_b.parts.b1;
            pic.elements.ui8[ ( ((from_row * col) + c) << 2 ) + 1] = col_b.parts.b2;
            pic.elements.ui8[ ( ((from_row * col) + c) << 2 ) + 2] = col_b.parts.b3;
        }
    }
}

void horizontal_line_width_n_offset_k(matrix_t pic, uint32_bytes col_b, uint32_t width, uint32_t offset, uint32_t from_col, uint32_t till_col){
    if(!(pic.elements.ui8 && pic.row && pic.col)){ 
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t r, col = pic.col[0], row = pic.row[0], down_b;
    offset = ternary(offset > row, row - !!row, offset);
    down_b = offset + ternary((row - offset) >= width, width, row - offset - !!row);
    from_col = ternary(from_col > col, col, from_col);
    till_col = ternary(till_col > col, col, till_col);
    if(from_col > till_col){ from_col ^= till_col; till_col ^= from_col; from_col ^= till_col; }
    for( ; from_col < till_col; from_col++){
        for(r = offset; r <= down_b; r++){
            pic.elements.ui8[ ( ((r * col) + from_col) << 2 )    ] = col_b.parts.b1;
            pic.elements.ui8[ ( ((r * col) + from_col) << 2 ) + 1] = col_b.parts.b2;
            pic.elements.ui8[ ( ((r * col) + from_col) << 2 ) + 2] = col_b.parts.b3;
        }
    }
}

void diagonal_line(matrix_t pic, uint32_bytes col_b, int8_t slope, uint32_t from_col, uint32_t till_col, uint32_t from_row, uint32_t till_row){// slope variable determines whether from left to right it decreases(false == 0) or increases(true == any number except 0
    if(!(pic.col && pic.row && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t row = pic.row[0], col = pic.col[0];
    from_col = ternary(from_col > col, col, from_col);
    till_col = ternary(till_col > col, col, till_col);
    from_row = ternary(from_row > row, row, from_row);
    till_row = ternary(till_row > row, row, till_row);
    if(from_row > till_row){ from_row ^= till_row; till_row ^= from_row; from_row ^= till_row; }
    if(((from_col > till_col) & !slope) | ((from_col <= till_col) && slope)){ from_col ^= till_col; till_col ^= from_col; from_col ^= till_col; }
    if(slope){
        for( ; (from_row < till_row) && (from_col > till_col); from_col--, from_row++){
            pic.elements.ui8[ ( ((from_row * col) + from_col ) << 2 )    ] = col_b.parts.b1;
            pic.elements.ui8[ ( ((from_row * col) + from_col ) << 2 ) + 1] = col_b.parts.b2;
            pic.elements.ui8[ ( ((from_row * col) + from_col ) << 2 ) + 2] = col_b.parts.b3;            
        }
    }
    else{
        for( ; (from_row < till_row) & (from_col < till_col); from_col++, from_row++){
            pic.elements.ui8[ ( ((from_row * col) + from_col ) << 2 )    ] = col_b.parts.b1;
            pic.elements.ui8[ ( ((from_row * col) + from_col ) << 2 ) + 1] = col_b.parts.b2;
            pic.elements.ui8[ ( ((from_row * col) + from_col ) << 2 ) + 2] = col_b.parts.b3;            
        }
    }
}//// what is this???????????????????????



void pix_from_k_to_m_color(matrix_t pic, uint32_bytes col_b, uint32_t from, uint32_t to){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t i, n = pic.row[0] * pic.col[0];
    if(from > to){ from ^= to; to ^= from; from ^= to; }
    to = ternary(to > n, n, to);
    from = ternary(from > n, n, from);
    for(i = from; i < to; i++){
        pic.elements.ui8[(i << 2)    ] = col_b.parts.b1;
        pic.elements.ui8[(i << 2) + 1] = col_b.parts.b2;
        pic.elements.ui8[(i << 2) + 2] = col_b.parts.b3;
    }
}

void coordinate_axis(matrix_t pic, uint32_bytes col_b){
    if(!(pic.elements.ui8 && pic.row && pic.col)){ 
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    horizontal_line_width_n_offset_k(pic, col_b, 0, pic.row[0] >> 1, 0, pic.col[0]);    
    vertical_line_width_n_offset_k(pic, col_b, 0, pic.col[0] >> 1, 0, pic.row[0]);    
}



void horizontal_gradient(matrix_t pic, uint32_bytes col1, uint32_bytes col2){
    if(!(pic.row && pic.col && pic.elements.ui8 && pic.err)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    int64_t row = pic.row[0], col = pic.col[0], r, c, r_i, g_i, b_i, row_r;
    if(!(col && row)){
        if(pic.err){ pic.err[0] = INCOMPATIBLE; }
        return;
    }
    int32_t col1_red   = col1.parts.b1, 
            col1_green = col1.parts.b2,
            col1_blue  = col1.parts.b3,
            red_diff   = col2.parts.b1 - col1_red,
            green_diff = col2.parts.b2 - col1_green,
            blue_diff  = col2.parts.b3 - col1_blue,
            red_int_rat   = red_diff   / col,
            green_int_rat = green_diff / col,
            blue_int_rat  = blue_diff  / col;
    double  red_fl_rat   = (double)(red_diff   % col) / col,
            green_fl_rat = (double)(green_diff % col) / col,
            blue_fl_rat  = (double)(blue_diff  % col) / col;
    for(r = 0; r < row; r++){
        row_r = r * col;
        for(c = 0; c < col; c++){
            r_i = (row_r + c) << 2;
            g_i =  r_i   + 1;
            b_i =  r_i   + 2;
            pic.elements.ui8[r_i] = col1_red   + c * (red_int_rat   + red_fl_rat  );
            pic.elements.ui8[g_i] = col1_green + c * (green_int_rat + green_fl_rat);
            pic.elements.ui8[b_i] = col1_blue  + c * (blue_int_rat  + blue_fl_rat );
        }
    }
}

void vertical_gradient(matrix_t pic, uint32_bytes col1, uint32_bytes col2){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    int64_t row = pic.row[0], col = pic.col[0], r, c, r_i, g_i, b_i;
    if(!(col && row)){
        if(pic.err){ pic.err[0] = INCOMPATIBLE; }
        return;
    }
    int32_t col1_red   = col1.parts.b1, 
            col1_green = col1.parts.b2,
            col1_blue  = col1.parts.b3,
            red_diff   = col2.parts.b1 - col1_red,
            green_diff = col2.parts.b2 - col1_green,
            blue_diff  = col2.parts.b3 - col1_blue,
            red_int_rat   = red_diff   / row,
            green_int_rat = green_diff / row,
            blue_int_rat  = blue_diff  / row;
    double  red_fl_rat   = (double)(red_diff   % row) / row,
            green_fl_rat = (double)(green_diff % row) / row,
            blue_fl_rat  = (double)(blue_diff  % row) / row;
    for(c = 0; c < col; c++){
        for(r = 0; r < row; r++){
            r_i = (r * col + c) << 2;
            g_i =  r_i   + 1;
            b_i =  r_i   + 2;
            pic.elements.ui8[r_i] = col1_red   + r * (red_int_rat   + red_fl_rat  );
            pic.elements.ui8[g_i] = col1_green + r * (green_int_rat + green_fl_rat);
            pic.elements.ui8[b_i] = col1_blue  + r * (blue_int_rat  + blue_fl_rat );
        }
    }
}

void left_top_to_right_bottom_diag_grad(matrix_t pic, uint32_bytes col1, uint32_bytes col2){
    if(!(pic.col && pic.row && pic.elements.ui32)){
        if(pic.err) pic.err[0] = NULL_POINTER;
        return;
    }
    int64_t row = pic.row[0], col = pic.col[0], r, c, r_i, g_i, b_i;
    if(!(col && row)){
        if(pic.err){ pic.err[0] = INCOMPATIBLE; }
        return;
    }
    int32_t col1_red = col1.parts.b1, col1_green = col1.parts.b2, col1_blue = col1.parts.b3,
            red_diff = col2.parts.b1 - col1.parts.b1, 
            green_diff = col2.parts.b2 - col1.parts.b2, 
            blue_diff = col2.parts.b3 - col1.parts.b3,
            red_int_rat = red_diff / (row + col), 
            green_int_rat = green_diff / (row + col),
            blue_int_rat = blue_diff / (row + col);
    double  red_fl_rat   = (double)(red_diff   % (row + col)) / (row + col),
            green_fl_rat = (double)(green_diff % (row + col)) / (row + col),
            blue_fl_rat  = (double)(blue_diff  % (row + col)) / (row + col);
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c++){
            r_i = (r * col + c) << 2;
            g_i = r_i + 1;
            b_i = r_i + 2;
            pic.elements.ui8[r_i] = col1_red   + (r + c) * (red_int_rat   + red_fl_rat);
            pic.elements.ui8[g_i] = col1_green + (r + c) * (green_int_rat + green_fl_rat);
            pic.elements.ui8[b_i] = col1_blue  + (r + c) * (blue_int_rat  + blue_fl_rat);
        }
    }
}

void right_top_to_left_bottom_diag_grad(matrix_t pic, uint32_bytes col1, uint32_bytes col2){
    if(!(pic.col && pic.row && pic.elements.ui32)){
        if(pic.err) pic.err[0] = NULL_POINTER;
        return;
    }
    int64_t row = pic.row[0], col = pic.col[0], r, col_c, c, r_i, g_i, b_i;
    if(!(col && row)){
        if(pic.err){ pic.err[0] = INCOMPATIBLE; }
        return;
    }
    int32_t col1_red = col1.parts.b1, col1_green = col1.parts.b2, col1_blue = col1.parts.b3,
            red_diff = col2.parts.b1 - col1.parts.b1, 
            green_diff = col2.parts.b2 - col1.parts.b2, 
            blue_diff = col2.parts.b3 - col1.parts.b3,
            red_int_rat = red_diff / (row + col), 
            green_int_rat = green_diff / (row + col),
            blue_int_rat = blue_diff / (row + col);
    double  red_fl_rat   = (double)(red_diff   % (row + col)) / (row + col),
            green_fl_rat = (double)(green_diff % (row + col)) / (row + col),
            blue_fl_rat  = (double)(blue_diff  % (row + col)) / (row + col);
    for(r = 0; r < row; r++){
        for(c = col, col_c = 0; c--; col_c++){
            r_i = (r * col + c) << 2;
            g_i = r_i + 1;
            b_i = r_i + 2;
            pic.elements.ui8[r_i] = col1_red   + (r + col_c) * (red_int_rat   + red_fl_rat);
            pic.elements.ui8[g_i] = col1_green + (r + col_c) * (green_int_rat + green_fl_rat);
            pic.elements.ui8[b_i] = col1_blue  + (r + col_c) * (blue_int_rat  + blue_fl_rat);
        }
    }
}

void diagonal_gradient(matrix_t pic, uint32_bytes col1, uint32_bytes col2, int8_t slope){// slope == 0 means that it's going from top left angle down to left, and slope == !0 means it's going from top right angle down till left
    if(slope) right_top_to_left_bottom_diag_grad(pic, col1, col2);
    else left_top_to_right_bottom_diag_grad(pic, col1, col2);
}


void rectangle_filled(matrix_t pic, uint32_bytes color, uint32_t x, uint32_t y, uint32_t xlength, uint32_t ylength){// x and y are coordinates of upper-left corner of rectangle(both starts from 1)
    if(!(x && y && pic.col && pic.row && pic.elements.ui8)){
        if(pic.err) pic.err[0] = NULL_POINTER;
        return;
    } 
    uint32_t row = pic.row[0], col = pic.col[0], c, red_i, gre_i, blu_i;
    if(!(row && col)){
        if(pic.err) pic.err[0] = INCOMPATIBLE;
        return;
    }
    uint8_t red_c = color.parts.b1, green_c = color.parts.b2, blue_c = color.parts.b3;
    xlength += x;
    ylength  += y;
    ylength = ternary(ylength > col, col, ylength);
    xlength = ternary((xlength > row) | (ylength == col), row, xlength);
    --y;
    for( --x; x < xlength; x++){
        for(c = y; c < ylength; c++){
            red_i = ((x * col) + c) << 2;
            gre_i = red_i + 1;
            blu_i = red_i + 2;
            pic.elements.ui8[red_i] = red_c;
            pic.elements.ui8[gre_i] = green_c;
            pic.elements.ui8[blu_i] = blue_c;
        }
    }
}



void rectangle_outline(matrix_t pic, uint32_bytes color, uint32_t x, uint32_t y, uint32_t xlength, uint32_t ylength){ // TODO
    if(!(x && y && pic.col && pic.row && pic.elements.ui8)){
        if(pic.err) pic.err[0] = NULL_POINTER;
        return;
    } 
    uint32_t row = pic.row[0], col = pic.col[0], r, c, red_i, gre_i, blu_i;
    if(!(row && col)){
        if(pic.err) pic.err[0] = INCOMPATIBLE;
        return;
    }
    uint8_t red_c = color.parts.b1, green_c = color.parts.b2, blue_c = color.parts.b3, i;
    xlength += x;
    ylength  += y;
    ylength = ternary(ylength > col, col, ylength);
    xlength = ternary((xlength > row) | (ylength == col), row, xlength);
    for(--x, --y, i = 0; i < 2; i++){
        for(c = y; c < ylength; c++){
            red_i = ((x * col) + c) << 2;
            gre_i = red_i + 1;
            blu_i = red_i + 2;
            pic.elements.ui8[red_i] = red_c;
            pic.elements.ui8[gre_i] = green_c;
            pic.elements.ui8[blu_i] = blue_c;
        }
        x = xlength - 1;
    }
    for(c = y, i = 0; i < 2; i++){
        
    }
}


void grid(){}
void circle(){ }
void ring(){ }
void diag_line_thr_middle_of_img(){ }
void line_segment(){ }
void straigh_line_thr_two_points(){ }
void line_at_angle_a(){ }
