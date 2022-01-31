
typedef unsigned char byte;     // 8-Bit 
typedef unsigned short word;    // 16-Bit

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
extern byte P;

/*
    Memory
*/      
extern byte memory[2048];

// Reset all internal Register
void CPU_RESET();

// Execute Instruction
void CPU_RUN();


