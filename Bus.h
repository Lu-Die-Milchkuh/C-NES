#ifndef _TYPES
#include "Types.h"
#endif


// Write Data to Address in memory, CPU Bus
void write(byte data,word address);

// Read data from given address, CPU Bus
byte read(word address);