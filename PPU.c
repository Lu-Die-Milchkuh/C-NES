#ifndef _STDIO_H
#include <stdio.h>
#endif
#include "PPU.h"
#include "Mapper.h"

byte palette[32] = {0};
byte patternTable[2][4096] = {0};
byte nameTable[2][1024] = {0};

byte OAM[64] = {0};
byte oam_address = 0;

// Internal Communication
byte ppu_latch = 0;
byte ppu_data_buffer = 0;

// Pixel offset horizontally
byte fine_x = 0;


loopy_register vram_address;
loopy_register tram_address;


PPUSTATUS status;
PPUMASK mask;
PPUCTRL control;



byte PPU_CPU_READ(word address) {

    byte data = 0;

    switch(address) {
        case PPUCTRL_ADDRESS: // not readable
            break;

        case PPUMASK_ADDRESS: // not readable
            break;

        case PPUSTATUS_ADDRESS:
            ppu_latch = 0; 
            data = (status.reg & 0xE0) | (ppu_data_buffer & 0x1F);
            status.vertical_blank = 0;
            break;

        case OAMADDR_ADDRESS: // not readable
            break;

        case OAMDATA_ADDRESS:
            data = OAM[address];
            break;

        case PPUSCROLL_ADDRESS: // not readable
            break;

        case PPUADDR_ADDRESS: // not readable
            break;

        case PPUDATA_ADDRESS:
            data = ppu_data_buffer;
            ppu_data_buffer = PPU_READ(vram_address.reg);
            
            if (vram_address.reg >= 0x3F00) 
            {
                data = ppu_data_buffer;
            }

            vram_address.reg += (control.increment_mode ? 32 : 1);


            break;
    }

    return data;
}



void PPU_CPU_WRITE(byte data,word address) {
    switch (address)
    {
    case PPUCTRL_ADDRESS:
        control.reg = data;
        tram_address.nametable_x = control.nametable_x;
        tram_address.nametable_y = control.nametable_y;
        break;

    case PPUMASK_ADDRESS:
        mask.reg = data;
        break;

    case PPUSTATUS_ADDRESS:
        break;

    case OAMADDR_ADDRESS:
        oam_address = data;
        break;

    case OAMDATA_ADDRESS:
        
        break;

    case PPUSCROLL_ADDRESS:
        if(ppu_latch == 0)
        {
            fine_x = data & 0x07;
			tram_address.coarse_x = data >> 3;
			ppu_latch = 1;
        }
        else 
        {
            tram_address.fine_y = data & 0x07;
			tram_address.coarse_y = data >> 3;
			ppu_latch = 0;
        }
        break;

    case PPUADDR_ADDRESS:
        if (ppu_latch == 0)
		{
			
			tram_address.reg = (word)((data & 0x3F) << 8) | (tram_address.reg & 0x00FF);
			ppu_latch = 1;
		}
		else
		{
			
			tram_address.reg = (tram_address.reg & 0xFF00) | data;
			vram_address = tram_address;
			ppu_latch = 0;
		}
        break;

    case PPUDATA_ADDRESS:
        PPU_WRITE(data,vram_address.reg);
        vram_address.reg += (control.increment_mode ? 32 : 1);
        break;
    
    }
}




byte PPU_READ(word address) {
    byte data = 0;
    address = address & 0x3FFF;

    if (address >= 0x0000 && address <= 0x1FFF)
	{
		// If the cartridge cant map the address, have
		// a physical location ready here
		data = patternTable[(address & 0x1000) >> 12][address & 0x0FFF];
	}
	else if (address >= 0x2000 && address <= 0x3EFF)
	{
		address &= 0x0FFF;

		if (MIRROR==VERTICAL)
		{
			// Vertical
			if (address >= 0x0000 && address <= 0x03FF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0400 && address <= 0x07FF)
				data = nameTable[1][address & 0x03FF];
			if (address >= 0x0800 && address <= 0x0BFF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0C00 && address <= 0x0FFF)
				data = nameTable[1][address & 0x03FF];
		}
		else if (MIRROR == HORIZONTAL)
		{
			// Horizontal
			if (address >= 0x0000 && address <= 0x03FF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0400 && address <= 0x07FF)
				data = nameTable[0][address & 0x03FF];
			if (address >= 0x0800 && address <= 0x0BFF)
				data = nameTable[1][address & 0x03FF];
			if (address >= 0x0C00 && address <= 0x0FFF)
				data = nameTable[1][address & 0x03FF];
		}
	}
	else if (address >= 0x3F00 && address <= 0x3FFF)
	{
		address &= 0x001F;
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;
		data = palette[address] & (mask.grayscale ? 0x30 : 0x3F);
	}

	return data;
}




void PPU_WRITE(byte data,word address) {

    address &= 0x3FFF;

	
	if (address >= 0x0000 && address <= 0x1FFF)
	{
		patternTable[(address & 0x1000) >> 12][address & 0x0FFF] = data;
	}
	else if (address >= 0x2000 && address <= 0x3EFF)
	{
		address &= 0x0FFF;
		if (MIRROR == VERTICAL)
		{
			// Vertical
			if (address >= 0x0000 && address <= 0x03FF)
				nameTable[0][address & 0x03FF] = data;
			if (address >= 0x0400 && address <= 0x07FF)
				nameTable[1][address & 0x03FF] = data;
			if (address >= 0x0800 && address <= 0x0BFF)
				nameTable[0][address & 0x03FF] = data;
			if (address >= 0x0C00 && address <= 0x0FFF)
				nameTable[1][address & 0x03FF] = data;
		}
		else if (MIRROR == HORIZONTAL)
		{
			// Horizontal
			if (address >= 0x0000 && address <= 0x03FF)
				nameTable[0][address & 0x03FF] = data;
			if (address >= 0x0400 && address <= 0x07FF)
				nameTable[0][address & 0x03FF] = data;
			if (address >= 0x0800 && address <= 0x0BFF)
				nameTable[1][address & 0x03FF] = data;
			if (address >= 0x0C00 && address <= 0x0FFF)
				nameTable[1][address & 0x03FF] = data;
		}
	}
	else if (address >= 0x3F00 && address <= 0x3FFF)
	{
		address &= 0x001F;
		if (address == 0x0010) address = 0x0000;
		if (address == 0x0014) address = 0x0004;
		if (address == 0x0018) address = 0x0008;
		if (address == 0x001C) address = 0x000C;
		palette[address] = data;
	}
}

void PPU_RUN(){}