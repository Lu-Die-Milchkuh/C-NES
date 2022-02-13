#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "Mapper.h"
#include "Cartridge.h"

byte MAPPER_ID = 0;

byte PRG_BANKS = 0;
byte CHR_BANKS = 0;

byte* chr_rom = NULL;
byte* prg_rom = NULL;

byte MIRROR = HORIZONTAL;




void MAPPER_INIT(FILE* file) {
    
    
    
}



byte MAPPER_000_CPU_READ(word address) {
    byte data = 0;
    word mapped_addr = 0;
    if(address >= 0x8000 && address <= 0xFFFF)
    {
        mapped_addr = address & (PRG_BANKS > 1 ? 0x7FFF : 0x3FFF);
        data = prg_rom[mapped_addr];
    }
    
    return data;
}


byte MAPPER_000_PPU_READ(word address) {
    byte data = 0;

    if(address >= 0x0000 && address <= 0x1FFF)
    {
        data = chr_rom[address];
    }
    return data; 
}

