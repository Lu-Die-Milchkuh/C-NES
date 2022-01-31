#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "Cartridge.h"

byte* cartridge_memory = NULL;


void loadROM(char* path) {
    FILE* file = fopen(path,"rb");
    size_t file_size = 0;

    if(file == NULL) 
    {
        printf("Failed to load File!");
        exit(-1);
    }

    fseek(file,0,SEEK_END);
    file_size = ftell(file);
    fseek(file,0,SEEK_SET);

    // Loading Cartridge into "memory"
    cartridge_memory = malloc((file_size*sizeof(byte)));
    
    // Check if malloc failed
    if(cartridge_memory == NULL)
    {
        printf("Failed to allocat memory!");
        exit(-1);
    }

    fread(cartridge_memory,sizeof(byte),file_size,file);

    fclose(file);
}


// Freeing allocated memory
void unloadROM() {
    if(cartridge_memory != NULL) {
        free(cartridge_memory);
    }
    
}