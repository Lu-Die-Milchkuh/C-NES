#ifndef _TYPES
#include "Types.h"
#endif


// Write Data to Address in memory, CPU Bus
void write(u8 data,u16 address);

// Read data from given address, CPU Bus
u8 read(u16 address);