#ifndef headerfile
    #include <stdio.h> 
    #include <stdlib.h>
    #include <stdint.h>
    #include "logical_functions_of_decision.c"
    #include "user_defined_datatypes.c"
    #include "constants.c"
    #include "safe_arithmetic_functions.c"
#endif

void image_data_int(char* mem_ptr, char c1, char c2, char c3, char c4){
    mem_ptr[0] = c1;
    mem_ptr[1] = c2;
    mem_ptr[2] = c3;
    mem_ptr[3] = c4;
}

void write_P3_PPM_file(char* filename, unsigned int width, unsigned int height){
    // unsigned int p3_default_file_c = 0;
    int size = width * height * 12;
    char image_data[size + 1];
    for(int i = 0; i < size; i += 12){
        // Red
        image_data[i + 0] = '0';
        image_data[i + 1] = '5';
        image_data[i + 2] = '5';
        image_data[i + 3] = ' ';
        // Green
        image_data[i + 4] = '1';
        image_data[i + 5] = '2';
        image_data[i + 6] = '0';
        image_data[i + 7] = ' ';
        // Blue
        image_data[i + 8] = ' ';
        image_data[i + 9] = ' ';
        image_data[i +10] = '0';
        image_data[i +11] = '\n';
    }
    image_data[size] = 0;
    FILE *fptr;
    
    if(filename || !sizeof(filename)){ fptr = fopen(filename, "w"); }
    else{
        // TODO: add name versioning (default_000.ppm, default_001.ppm, ...)
        fptr = fopen("./default_file_name.ppm", "w");
        // char* def_file_name = "./default_file_name000000000.ppm";
        // def_file_name[19] = p3_default_file_c++ + '0';
        // const char* str = def_file_name;
    }
    
    if(fptr == NULL){
        printf("STOP ERROR NULL!");
        return;
    }
    
    fprintf(fptr, "P3\n%d %d\n255\n%s", width, height, image_data);
    fclose(fptr);
}

void write_P6_PPM_file(const char* filename, uint8_t width, uint8_t height){
    if(!(width && height)){ return; }
    int32_t size = width * height * 3;
    char image_data[size + 1];
    for(int i = 0; i < size; i += 3){
        image_data[i    ] = 1;
        image_data[i + 1] = 1;
        image_data[i + 2] = 1;
    }
    image_data[size] = 0;
    FILE *fptr;
    if(filename || !sizeof(filename)){ fptr = fopen(filename, "w"); }
    else{ fptr = fopen("./default_file_name.ppm", "w"); }
    
    if(fptr == NULL){
        printf("STOP ERROR NULL!");
        return;
    }
    fprintf(fptr, "P6\n%u %u\n255\n%s", width, height, image_data);
    fclose(fptr);
}



void file_filler(const char *str, matrix_t pic){
    if(!(pic.row && pic.col && pic.elements.ui8)){
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    FILE* fptr = fopen(str, "w");
    uint8_t *arr = malloc((pic.row[0] * pic.col[0] * 3) + 1);
    uint32_t i = 0, r, c, row = pic.row[0], col = pic.col[0];
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c++, i+=3){
            arr[i    ] = pic.elements.ui8[ ( ((r * col) + c) << 2 )    ] + !(pic.elements.ui8[ ( ((r * col) + c) << 2 )    ]);
            arr[i + 1] = pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 1] + !(pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 1]);
            arr[i + 2] = pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 2] + !(pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 2]);
        }
    }
    arr[i] = 0;
    fprintf(fptr, "P6\n%u %u\n255\n%s", pic.col[0], pic.row[0], (char*)arr);
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
}



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

void diagonal_gradient(matrix_t pic, uint32_bytes col1, uint32_bytes col2, int8_t slope){
    
    
}