#pragma once
#include "Types.h"


/*
    Registers
*/
extern u8 A;  // Accumulator, 8-Bit
extern u8 X;  // X-Register, 8-Bit
extern u8 Y;  // Y-Register,  8-Bit

//Stack Pointer
extern u8 SP;

//Programm Counter
extern u16 PC;

typedef union 
    {
        struct 
        {
            u8 N:1;
            u8 V:1;
            u8 unused:1;
            u8 B:1;
            u8 D:1;
            u8 I:1;
            u8 Z:1;
            u8 C:1;
        };

        u8 reg;
} STATUS;

/*  
    Status Register(bit 7 to bit 0)

    N	Negative
    V	Overflow
    -	ignored
    B	Break
    D	Decimal (use BCD for arithmetics)
    I	Interrupt (IRQ disable)
    Z	Zero
    C	Carry
*/

extern STATUS SR; // Status Register

/*
    Memory
*/      
extern u8 memory[2048];

// Reset all internal Register
void CPU_RESET();

// Execute Instruction
void CPU_RUN();


void CPU_STATUS();