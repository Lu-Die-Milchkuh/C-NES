#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "Cartridge.h"
#include "Mapper.h"


// Pointer to Store Programm Memory of ROM
byte* cartridge_pgr_memory = NULL;

// Pointer to Store Character Memory of ROM
byte* cartridge_chr_memory = NULL;



void loadROM(char* path) {
    FILE* file = fopen(path,"rb");

    if(file == NULL) 
    {
        printf("Failed to load File!");
        exit(-1);
    }
    MAPPER_INIT(file);

    fclose(file);
}


// Freeing allocated memory
void unloadROM() {
    
    if(cartridge_pgr_memory != NULL)
    {
        free(cartridge_pgr_memory);
    }
    
    if(cartridge_chr_memory != NULL) 
    {
        free(cartridge_chr_memory);
    }
}