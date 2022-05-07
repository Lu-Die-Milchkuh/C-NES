#pragma once
#include "Types.h"



// Set Bit @ Index, set to 1
void setBit(u8* reg,u8 index);

// Clear Bit @ Index, set to 0
void clearBit(u8* reg,u8 index);

// Returns Bit @ given Index
u8 getBit(u8* reg,u8 index);

// Get Binary Presentation of a given value,size used for bit count
void bin(unsigned value,int size);