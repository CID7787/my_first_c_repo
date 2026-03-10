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
