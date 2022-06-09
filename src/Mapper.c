#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "Mapper.h"
#include "Cartridge.h"

u8 MAPPER_ID = 0;

u8 PRG_BANKS = 0;
u8 CHR_BANKS = 0;

u8* chr_rom = NULL;
u8* prg_rom = NULL;

u8 MIRROR = HORIZONTAL;




u8 MAPPER_000_CPU_READ(u16 address) {
    u8 data = 0;
    u16 mapped_addr = 0;
    if(address >= 0x8000 && address <= 0xFFFF)
    {
        mapped_addr = address & (PRG_BANKS > 1 ? 0x7FFF : 0x3FFF);
        data = prg_rom[mapped_addr];
    }
    
    return data;
}


u8 MAPPER_000_PPU_READ(u16 address) {
    u8 data = 0;

    if(address >= 0x0000 && address <= 0x1FFF)
    {
        data = chr_rom[address];
    }
    return data; 
}
