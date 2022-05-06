#ifndef _STDIO_H
#include <stdio.h>
#endif
#include "HelperFunc.h"



void setBit(u8* reg,u8 index) {
    *reg |= (1<<index);
}


void clearBit(u8* reg,u8 index) {
    *reg &= ~(1<<index);
}


u8 getBit(u8* reg,u8 index) {
    u8 foo = 0;
    foo = (*reg & (1<<index));
    return foo;
}

void bin(unsigned n,int size)
{
    unsigned i;
    for (i = 1 << (size-1); i > 0; i = i / 2)
        (n & i) ? printf("1") : printf("0");
}