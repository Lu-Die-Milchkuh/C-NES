#ifndef _TYPES
#include "Types.h"
#endif



extern u8* chr_rom ;
extern u8* prg_rom ;


// Read from Cartridge
u8 MAPPER_000_CPU_READ(u16 address);
u8 MAPPER_000_PPU_READ(u16 address);


#define VERTICAL 1
#define HORIZONTAL 0

extern u8 MIRROR;
extern u8 MAPPER_ID;
extern u8 PRG_BANKS;
extern u8 CHR_BANKS;