#ifndef headerfile
    #include <stdio.h> 
    #include <stdlib.h>
    #include <stdint.h>
    #include "user_define_datatypes.c" 
    #include "constants.c"
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
    FILE* fptr = fopen(str, "w");
    uint8_t *arr = malloc((pic.row[0] * pic.col[0] * 3) + 1);
    uint32_t i = 0, r, c, row = pic.row[0], col = pic.col[0];
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c++, i+=3){
            arr[i    ] = pic.elements.ui8[ ( ((r * col) + c) << 2 )    ] + !(pic.elements.ui8[ ( ((r * col) + c) << 2 )    ]);
            arr[i + 1] = pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 1] + !(pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 1]);
            arr[i + 2] = pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 2] + !(pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 2]);
            if(!((r * col + c) % col)){ printf("\n\n"); }
            printf("%u %u %u  ", arr[i], arr[i + 1], arr[i + 2]);
        }
    }
    arr[i] = 0;
    fprintf(fptr, "P6\n%u %u\n255\n%s", pic.col[0], pic.row[0], (char*)arr);
    fclose(fptr);
    free(arr);
}
  
  
void left_side_color(matrix_t pic, uint32_bytes col_b){
    uint32_t r, c, row = pic.row[0], col = pic.col[0], col_till = col >> 1; 
    for(r = 0; r < row; r++){
        for(c = 0; c < col_till; c++){ // col 
            pic.elements.ui8[ (((r * col) + c) << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[ (((r * col) + c) << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[ (((r * col) + c) << 2) + 2] = col_b.parts.b3;
            printf("%u %u %u   ", pic.elements.ui8[(((r * col) + c) << 2)    ], pic.elements.ui8[(((r * col) + c) << 2) + 1], pic.elements.ui8[(((r * col) + c) << 2) + 2]);
            if(c == (col_till - 1)){ printf("\n\n"); }

        }
    } 
}
  
void right_side_color(matrix_t pic, uint32_bytes col_b){
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
    uint32_t r, c, row = pic.row[0], col = pic.col[0];
    for(r = row >> 1; r < row; r++){
        for(c = 0; c < col; c++){ // col 
            pic.elements.ui8[(((r * col) + c ) << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(((r * col) + c ) << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(((r * col) + c ) << 2) + 2] = col_b.parts.b3;
        }
    } 
}


void even_pix_color(matrix_t pic, uint32_bytes col_b){
    uint32_t i, n = pic.row[0] * pic.col[0];
    for(i = 0; i < n; i++){
        if(i ^ 1){
            pic.elements.ui8[(i << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(i << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(i << 2) + 2] = col_b.parts.b3;    
        }
    }
}

void odd_pix_color(matrix_t pic, uint32_bytes col_b){
    uint32_t i, n = pic.row[0] * pic.col[0];
    for(i = 0; i < n; i++){
        if(i & 1){
            pic.elements.ui8[(i << 2)    ] = col_b.parts.b1;
            pic.elements.ui8[(i << 2) + 1] = col_b.parts.b2;
            pic.elements.ui8[(i << 2) + 2] = col_b.parts.b3;    
        }
    }
}
    

void pix_from_k_to_m_color(matrix_t pic, uint32_bytes col_b, uint32_t from, uint32_t to){
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

void vertical_line_width_n_offset_k(matrix_t pic, uint32_bytes col_b, uint32_t width, uint32_t offset){// width add up on offset value so vertical line will start from offset value and till offset + width draw it
    if(!(pic.elements.ui8 && pic.row && pic.col)){ 
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t r = 0, row = pic.row[0], col = pic.col[0], right_b, c;
    offset = ternary(offset < col, offset, col - !!col);
    right_b = offset + ternary((col - offset) >= width, width, col - offset - !!col);
    for(r = 0; r < row; r++){
        for(c = offset; c <= right_b; c++){
            pic.elements.ui8[ ( ((r * col) + c) << 2 )    ] = col_b.parts.b1;
            pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 1] = col_b.parts.b2;
            pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 2] = col_b.parts.b3;
            }
    }
}

void horizontal_line_width_n_offset_k(matrix_t pic, uint32_bytes col_b, uint32_t width, uint32_t offset){
    if(!(pic.elements.ui8 && pic.row && pic.col)){ 
        if(pic.err){ pic.err[0] = NULL_POINTER; }
        return;
    }
    uint32_t c, col = pic.col[0], row = pic.row[0], r, down_b;
    offset = ternary(offset < row, offset, row - !!row);
    down_b = offset + ternary((row - offset) >= width, width, row - offset - !!row);
    for(c = 0; c < col; c++){
        for(r = offset; r <= down_b; r++){
            pic.elements.ui8[ ( ((r * col) + c) << 2 )    ] = col_b.parts.b1;
            pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 1] = col_b.parts.b2;
            pic.elements.ui8[ ( ((r * col) + c) << 2 ) + 2] = col_b.parts.b3;
        }
    }
}