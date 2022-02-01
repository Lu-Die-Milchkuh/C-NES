#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "Mapper.h"
#include "Cartridge.h"

byte MAPPER_ID = 0;

HEADER rom_header;

// Number of Banks
byte PRG_BANKS = 0;
byte CHR_BANKS = 0;


void MAPPER_INIT(FILE* file) {
    size_t pgr_mem_size = 0;
    size_t chr_mem_size = 0;

    // Read Header into struct
    fread(&rom_header,sizeof(rom_header),1,file); 
    //getHeader();

    // Checking if "Trainer" exists
    if((rom_header.mapper1 & 0x04) == 1) 
    {   // Trainer is stored at 0x7000-0x71FF
        //offset = 16 + 512 -> Header + Trainer
        fseek(file,512+16,SEEK_SET);
    }

    // Getting Mapper ID 
    MAPPER_ID = ((rom_header.mapper2 >> 4) << 4) | (rom_header.mapper1 >> 4); 

    // Allocating Memory for Programm Code
    if(MAPPER_ID == 0) 
    {   
        pgr_mem_size = (sizeof(byte)*rom_header.prg_rom_size*16384);

        // Value 0 means board uses CHR RAM
        if(rom_header.chr_rom_size == 0) 
        {
            chr_mem_size = (sizeof(byte)*8192);
        }
        else {
            chr_mem_size = (sizeof(byte)*8192*rom_header.chr_rom_size);
        }
        

        cartridge_pgr_memory = malloc(pgr_mem_size);
        cartridge_chr_memory = malloc(chr_mem_size);
    }
    
    if(cartridge_pgr_memory == NULL) 
    {   
        printf("Failed to allocat cartridge_pgr_memory!");
        exit(-1);
    }

    if(cartridge_chr_memory == NULL)
    {
        printf("Failed to allocat cartridge_chr_memory!");
        exit(-1);
    }

    fread(cartridge_pgr_memory,pgr_mem_size,1,file);

    fread(cartridge_chr_memory,chr_mem_size,1,file);
    
}


/*
byte MAPPER_000_READ(word address) {
    byte data = 0;
    word mapped_address = 0;
    mapped_address = address & (PRG_BANKS > 1 ? 0x7FFF : 0x3FFF);

    data = cartridge_memory[mapped_address];
    return data;
}
*/

