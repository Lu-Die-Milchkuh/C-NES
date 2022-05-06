#ifndef _STDIO_H
#include <stdio.h>
#endif
#include "Bus.h"
#include "6502.h"
#include "PPU.h"
#include "Mapper.h"

// Write Data to the Bus aka System,PPU Memory
void bus_write(u8 data,u16 address) {
    /* WRITING TO SYSTEM RAM!
       System RAM Address Space is 8 KB big, even tho only 2 KB is available!
       The other 6 KB are mirrors of the first 2 KB!
       To make sure we only write to RAM that exist, we mask the
       given address with a bitwise AND.
       Example: 0x1FFF AND 0x07FF = 0x7FF -> 2047 -> valid Address 
    */
    if(address >= 0x0000 && address <= 0x1FFF) // Mirrored Memory is ignored for now
    {
        memory[(address & 0x07FF)] = data;
    }

    // Write to PPU
    else if(address >= 0x2000 && address <= 0x3FFF)
    {
        PPU_CPU_WRITE(data,(address & 0x0007));
    }


}


u8 bus_read(u16 address) {
    u8 data = 0;  // variable to store data

    // Read data from system memory
    if(address >= 0x0000 && address <= 0x1FFF) 
    {
        /*  Same problem as write() -> we need to make sure we are reading from a valid address!
            See write()-Function for explanation why we use AND here!
        */
        data = memory[(address & 0x07FF)];
    }

    // Read from PPU 
    else if(address >= 0x2000 && address <= 0x3FFF)
    {
        data = PPU_CPU_READ((address & 0x0007));
    }
    // Read from APU
    else if(address == 0x4015)
    {

    }

    // Reading from Cartridge
    else if(address >= 0x8000 && address <= 0xFFFF)
    {
        data = MAPPER_000_CPU_READ(address); // Only Mapper 00 for now
    }
    return data;
}