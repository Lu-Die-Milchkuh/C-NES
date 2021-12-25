#ifndef STDIO_H
#include <stdio.h>
#endif
#include "HelperFunc.h"



void setBit(byte* reg,byte index) {
    *reg |= (1<<index);
}


void clearBit(byte* reg,byte index) {
    *reg &= ~(1<<index);
}


byte getBit(byte* reg,byte index) {
    byte foo = 0;
    foo = (*reg & (1<<index));
    return foo;
}

void bin(unsigned n,int size)
{
    unsigned i;
    for (i = 1 << (size-1); i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}