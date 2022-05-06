#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "Cartridge.h"
#include "Mapper.h"


ROM_HEADER header;


void loadROM(char* path) {
    FILE* file = fopen(path,"rb");
    u8 file_type = 1; // INES Format
    size_t chr_size = 0;
    size_t prg_size = 0;

    if(file == NULL) 
    {
        printf("Failed to load File!");
        exit(-1);
    }
    
   
    fread(&header,sizeof(header),1,file);
    
    // Is a trainer present?
    if(header.mapper1 & 0x04)
    {
        // Skip 512 bytes
        fseek(file,512,SEEK_SET);
    }


    MAPPER_ID = ((header.mapper2 >> 4) << 4) | (header.mapper1 >> 4);
    MIRROR = (header.mapper1 & 0x01) ? HORIZONTAL : VERTICAL;
    
    //Check if NES 2.0 File
    if((header.mapper2 & 0x0C) == 0x08)
    {   
        // NES 2.0 Format
        file_type = 2;
    }


    if(file_type == 1)
    {   
        PRG_BANKS = header.prg_rom_chunks;
        CHR_BANKS = header.chr_rom_chunks;

        prg_size = (sizeof(u8) * PRG_BANKS * 16384);
         

        prg_rom = malloc(prg_size);
        fread(prg_rom,prg_size,1,file);

        if(header.chr_rom_chunks == 0) 
        {   
            chr_size = (sizeof(u8) * 8192);
            chr_rom = malloc(chr_size);
        }

        else
        {   
            chr_size = (sizeof(u8) * 8192 * header.chr_rom_chunks);
            chr_rom = malloc(chr_size);
        }

        fread(chr_rom,chr_size,1,file);
    }

    if(file_type == 2)
    {   
        PRG_BANKS = ((header.prg_ram_size & 0x07) << 8) | header.prg_rom_chunks;
        CHR_BANKS = ((header.prg_ram_size & 0x38) << 8) | header.chr_rom_chunks;

        prg_size = (sizeof(u8) * PRG_BANKS * 16384);
        chr_size =(sizeof(u8) * CHR_BANKS * 8192);

        prg_rom = malloc(prg_size);

        fread(prg_rom,prg_size,1,file);

        chr_rom = malloc(chr_size);

        fread(chr_rom,chr_size,1,file);

    }
    
    fclose(file);
}



// Freeing allocated memory
void unloadROM() {
    
    if(prg_rom != NULL) 
    {
        free(prg_rom);
    }

    if(chr_rom != NULL) 
    {
        free(chr_rom);
    }
   
}