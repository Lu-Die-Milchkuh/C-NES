#ifndef _TYPES
#include "Types.h"
#endif

/*
    Registers
*/
extern byte A;  // Accumulator, 8-Bit
extern byte X;  // X-Register, 8-Bit
extern byte Y;  // Y-Register,  8-Bit

//Stack Pointer
extern byte SP;

//Programm Counter
extern word PC;

typedef union 
    {
        struct 
        {
            byte N:1;
            byte V:1;
            byte unused:1;
            byte B:1;
            byte D:1;
            byte I:1;
            byte Z:1;
            byte C:1;
        };

        byte reg;
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
extern byte memory[2048];

// Reset all internal Register
void CPU_RESET();

// Execute Instruction
void CPU_RUN();


void CPU_STATUS();