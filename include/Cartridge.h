#pragma once
#include "Types.h"


// Pointer containing content of ROM
extern u8* cartridge_pgr_memory;
extern u8* cartridge_chr_memory;

/*
The format of the header is as follows:

0-3: Constant $4E $45 $53 $1A ("NES" followed by MS-DOS end-of-file)
4: Size of PRG ROM in 16 KB units
5: Size of CHR ROM in 8 KB units (Value 0 means the board uses CHR RAM)
6: Flags 6 - Mapper, mirroring, battery, trainer
7: Flags 7 - Mapper, VS/Playchoice, NES 2.0
8: Flags 8 - PRG-RAM size (rarely used extension)
9: Flags 9 - TV system (rarely used extension)
10: Flags 10 - TV system, PRG-RAM presence (unofficial, rarely used extension)
11-15: Unused padding (should be filled with zero, but some rippers put their name across bytes 7-15)
*/

typedef struct {
                char name[4];
		        u8 prg_rom_chunks;
		        u8 chr_rom_chunks;
		        u8 mapper1;
		        u8 mapper2;
		        u8 prg_ram_size;
		        u8 tv_system1;
		        u8 tv_system2;
		        u8 unused[5];
} ROM_HEADER;


void loadROM(char* path);
void unloadROM(void);