#pragma once
#include "Types.h"


// Write Data to Address in memory, CPU Bus
void bus_write(u8 data,u16 address);

// Read data from given address, CPU Bus
u8 bus_read(u16 address);