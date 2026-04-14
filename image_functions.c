#ifndef headerfile
    #include <stdio.h> 
    #include <stdlib.h>
    #include <stdint.h> 
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



void felem_filler(const char *str, matrix_t pic){
    FILE* fptr = fopen(str, "w");
    unsigned char *arr = malloc((pic.row[0] * pic.col[0]) + 1);
    int i;
    for(i = 0; i < pic.row[0] * pic.col[0]; i++){ arr[i] = pic.elements.ui8[i] + !(pic.elements.ui8[i]); }
    arr[i] = 0;
    fprintf(fptr, "P6\n%u %u\n255\n%s", pic.row[0], pic.col[0] / 3, (char*)arr);
    fclose(fptr);
    free(arr);
  }
  
  
  void left_side_color(matrix_t pic, uint8_t red_b, uint8_t green_b, uint8_t blue_b){
    // ASSUMPTIONS: matrix_type always == uint8, pic elements amount == 3 * col * row, if col odd then left side have one more color column
    uint32_t r, c, row = pic.row[0], col = pic.col[0]; 
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c+=3){ // col 
            if(c <= (col >> 1)){
                pic.elements.ui8[(r * col) + c    ] = red_b;
                pic.elements.ui8[(r * col) + c + 1] = green_b;
                pic.elements.ui8[(r * col) + c + 2] = blue_b;
            }
        }
    } 
  }
  
  void right_side_color(matrix_t pic, uint8_t red_b, uint8_t green_b, uint8_t blue_b){
    // ASSUMPTIONS: matrix_type always == uint8, pic elements amount == 3 * col * row, if col odd then left side have one more color column
    uint32_t r, c, row = pic.row[0], col = pic.col[0]; 
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c+=3){ // col 
            if(c > (col >> 1)){
                pic.elements.ui8[(r * col) + c    ] = red_b;
                pic.elements.ui8[(r * col) + c + 1] = green_b;
                pic.elements.ui8[(r * col) + c + 2] = blue_b;
            }
        }
    } 
  }
  
  
  
  
  void top_down_side_color(matrix_t pic){
    // ASSUMPTIONS: matrix_type always == uint8, pic elements amount == 3 * col * row, if col odd then left side have one more color column
    uint32_t r, c, row = pic.row[0], col = pic.col[0]; 
    for(r = 0; r < row; r++){
        for(c = 0; c < col; c+=3){ // col 
            if(r <= (row >> 1)){
                pic.elements.ui8[(r * col) + c    ] = 0;
                pic.elements.ui8[(r * col) + c + 1] = 0;
                pic.elements.ui8[(r * col) + c + 2] = 0;
            }
            else{
                pic.elements.ui8[(r * col) + c    ] = 0;
                pic.elements.ui8[(r * col) + c + 1] = 0;
                pic.elements.ui8[(r * col) + c + 2] = 255;
            }
        }
    } 
  }