#ifndef _TYPES
#include "Types.h"
#endif



extern byte* chr_rom ;
extern byte* prg_rom ;


// Read from Cartridge
byte MAPPER_000_CPU_READ(word address);
byte MAPPER_000_PPU_READ(word address);


#define VERTICAL 1
#define HORIZONTAL 0

extern byte MIRROR;
extern byte MAPPER_ID;
extern byte PRG_BANKS;
extern byte CHR_BANKS;