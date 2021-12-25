#include <stdio.h>
#include "Bus.h"
#include "PPU.h"

byte ppu_memory[8192] = {0};
byte* PPUCTRL = &(ppu_memory[0]);



