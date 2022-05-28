#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "6502.h"
#include "HelperFunc.h"
#include "Bus.h"

u8 A = 0;
u8 X = 0;
u8 Y = 0;
u16 PC = 0;
u8 SP = 0;
STATUS SR;
u8 memory[2048] = {0};

// Var to store temporary data or address
u8 temp_data = 0;
u16 temp_address = 0;


// opcode -> u8 that identifies the instruction
u8 opcode = 0;
/*

**********************************************
*     Addressing Mode Functions, get Data    *
**********************************************
    
*/

// Return next Byte, Immediate Addressing Mode
void getImmediate(void) {
    PC++;
    temp_data = bus_read(PC);   
}

// Return Data from Zero Page, only capable of addressing first 256 Bytes of Memory
void getZPG(void) {
    u8 zpg_addr = 0;  // Zero Page Address
    PC++;
    zpg_addr = bus_read(PC);
    temp_data = bus_read(zpg_addr);

}


// Return Data from Zero Page + X(offset)
void getZPGX(void) {
    u8 zpg_addr = 0;  // Zero Page Addrsss
    PC++;
    zpg_addr = bus_read(PC) + X;  // Adding Offset
    temp_data = bus_read(zpg_addr);
}

// Return Data from Zero Page + Y(offset)
void getZPGY(void) {
    u8 zpg_addr = 0;  // Zero Page Addrsss
    PC++;
    zpg_addr = bus_read(PC) + Y;  // Adding Offset
    temp_data = bus_read(zpg_addr);

}


// Return Data stored @ Address in Memory, Absolute Addressing Mode
void getAbsolute(void) {
    u16 address = 0;
    u8 highByte,lowByte;

    PC++;
    lowByte = bus_read(PC);
    PC++;
    highByte = bus_read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte);
    temp_data = bus_read(address);

}


// Return Data stored @ Address + X in Memory, Absolute X-Indexed Addressing Mode
void getAbsoluteX(void) {
    u16 address = 0;
    u8 highByte,lowByte;

    PC++;
    lowByte = bus_read(PC);
    PC++;
    highByte = bus_read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte); 
    address += X;   // Adding Offset
    temp_data = bus_read(address); 

}


// Return Data stored @ Address + Y in Memory, Absolute Y-Indexed Addressing Mode
void getAbsoluteY(void) {
    u16 address = 0;
    u8 highByte,lowByte;

    PC++;
    lowByte = bus_read(PC);
    PC++;
    highByte = bus_read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte); 
    address += Y;   // Adding Offset
    temp_data = bus_read(address);

}


/*
    NEED TO BE LOOKED AT IN THE FUTURE
*/

// Indirect Addressing Mode, given Address is used to calculate the real address where data is stored
void getIndirect(void) {
    u8 tmp_addr1 = 0; // HighByte
    u8 tmp_addr2 = 0; //LowByte
    u16 tmp_addr = 0;  //Placeholder Address
    u16 real_addr = 0;

    PC++;
    tmp_addr1 = bus_read(PC);
    PC++;
    tmp_addr2 = bus_read(PC);

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 

    real_addr = bus_read(tmp_addr+1) << 8 | bus_read(tmp_addr);
    temp_data = bus_read(real_addr);

}

// Indirect Addressing Mode + Offset X
void getIndirectX(void) {
    u8 tmp_addr1 = 0;
    u8 tmp_addr2 = 0;
    u16 tmp_addr = 0;
    u16 real_addr = 0;

    PC++;
    tmp_addr1 = bus_read(PC) + X;
    PC++;
    tmp_addr2 = bus_read(PC) + X;

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    real_addr = bus_read(tmp_addr+1) << 8 | bus_read(tmp_addr);

    temp_data = bus_read(real_addr);

}

// Indirect Addressing Mode + Offset Y
void getIndirectY(void) {
    u8 tmp_addr1 = 0;
    //u8 tmp_addr2 = 0;
    //u16 tmp_addr = 0;
    u16 real_addr = 0;

    u8 lowByte = 0;
    u8 highByte = 0;

    PC++;
    tmp_addr1 = bus_read(PC);
    //PC++;
    lowByte = bus_read(tmp_addr1);
    highByte = bus_read(tmp_addr1+1);

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    //tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    real_addr = highByte << 8 | lowByte;
    real_addr += Y;
    temp_data = bus_read(real_addr);

}

// Some instrctions are implied . which means all the needed informations are already present
void noMode(void) {
    
}


/*

**********************************************
*     Addressing Mode Functions, get Address *
**********************************************
    
*/

// Returns absolute Address
void getAbsoluteAddr(void) {
    u8 highByte,lowByte;

    PC++;
    lowByte = bus_read(PC); 
    PC++;
    highByte = bus_read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    temp_address = (highByte << 8 | lowByte); 

}

// Returns absolute, X-indexed Address
void getAbsoluteXAddr(void) {
    u8 highByte,lowByte;

    PC++;
    lowByte = bus_read(PC);
    PC++;
    highByte = bus_read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    temp_address = (highByte << 8 | lowByte); 
    temp_address += X;   // Adding Offset

}


// Returns absolute, Y-indexed Address
void getAbsoluteYAddr(void) {
    u8 highByte,lowByte;

    PC++;
    lowByte = bus_read(PC);
    PC++;
    highByte = bus_read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    temp_address = (highByte << 8 | lowByte); 
    temp_address += Y;   // Adding Offset

}


// Returns zeropage address
void getZPGAddr(void) {
    PC++;
    temp_address = bus_read(PC);
    
}

// Returns zeropage, X-indexed Address
void getZPGXAddr(void) {
    PC++;
    temp_address = bus_read(PC) + X;  // Adding Offset
    
}

// Returns zeropage, X-indexed Address
void getZPGYAddr(void) {
    PC++;
    temp_address = bus_read(PC) + Y;  // Adding Offset
    
}


// Returns an indirect Address
void getIndirectAddr(void) {
    u8 tmp_addr1 = 0; // HighByte
    u8 tmp_addr2 = 0; //LowByte
    u16 tmp_addr = 0;  //Placeholder Address

    PC++;
    tmp_addr1 = bus_read(PC);
    PC++;
    tmp_addr2 = bus_read(PC);

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 

    temp_address = bus_read(tmp_addr+1) << 8 | bus_read(tmp_addr);
    
}

// Returns indirect, x-indexed Address
void getIndirectXAddr(void) {
    u8 tmp_addr1 = 0;
    u8 tmp_addr2 = 0;
    u16 tmp_addr = 0;

    PC++;
    tmp_addr1 = bus_read(PC) + X;
    PC++;
    tmp_addr2 = bus_read(PC) + X;

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    temp_address = bus_read(tmp_addr+1) << 8 | bus_read(tmp_addr);

}

// Returns indirect, y-indexed Address
void getIndirectYAddr(void) {
    u8 tmp_addr1 = 0;
    u8 tmp_addr2 = 0;
    u16 tmp_addr = 0;

    PC++;
    tmp_addr1 = bus_read(PC);
    PC++;
    tmp_addr2 = bus_read(PC);

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    temp_address = bus_read(tmp_addr+1) << 8 | bus_read(tmp_addr);
    temp_address += Y;
;
}


/*

**********************************************
*       6502 Instruction Functions           *
**********************************************

*/

// Incase an illegal/undocumented opcode occurs, exit
void illegalOpcode(void) {
    printf("ILLEGAL INSTRUCTION: %X",opcode);
    exit(-1);

}


// Bitwise AND Memory with Accumulator
void AND(void) {
    A &= temp_data;

    SR.Z = (A==0); // Set Zero Flag
    SR.N = (A >> 7); // Set Negative Flag;
    
    PC++;
}

// Bitwise XOR with Accumulator
void EOR(void) {
    A ^= temp_data;

    SR.Z = (A==0); // Set Zero Flag
    SR.N = (A >> 7); // Set Negative Flag;

    PC++;
}

// Bitwise OR with Accumulator
void ORA(void) {
    A |= temp_data;

    SR.Z = (A==0); // Set Zero Flag
    SR.N = (A >> 7); // Set Negative Flag;

    PC++;
}


// NO Operation -> do nothing
void NOP(void) {
    PC++;
}


// Set Decimal Flag
void SED(void) {
    SR.D = 1;
    PC++;
}


// Set Interrupt Flag
void SEI(void) {
    SR.I = 1;
    PC++;
}


// Set Carry Flag
void SEC(void) {
    SR.C = 1;
    PC++;
}


// Clear Carry Flag
void CLC(void) {
    SR.C = 0;
    PC++;
}


// Clear Decimal Bit
void CLD(void) {
    SR.D = 0;
    PC++;
}


// Clear Interrupt Flag
void CLI(void) {
    SR.I = 0;
    PC++;
}


// Clear Overflow Flag
void CLV(void) {
    SR.V = 0;
    PC++;
}


// Push A on Stack
void PHA(void) {
    bus_write(A,SP);
    SP--;
    PC++;
}


// Pull A from Stack
void PLA(void) {
    A = bus_read(SP);

    SR.Z = (A==0);
    SR.N = (A>>7);
    
    SP++;
    PC++;
    
}

// Push Status Register on Stack
void PHP(void) {
    bus_write(SR.reg,SP);
    SP--;
    PC++;
}

// Pull Status Register from Stack
void PLP(void) {
    SR.reg = bus_read(SP);
    SP++;
    PC++;
}


// Increase X by 1
void INX(void) {
    X++;

    SR.Z = (X==0);
    SR.N = (X>>7);

    PC++;
}

// Increase Y by 1
void INY(void) {
    Y++;

    SR.Z = (Y==0);
    SR.N = (Y>>7);

    PC++;
}

// Increase Memory by 1
void INC(void) {
    u8 foo = bus_read(temp_address);
    foo++;
    bus_write(foo,temp_address);

    SR.Z = (foo==0);
    SR.N = (foo>>7);

    PC++;
}

// Decrement Memory by 1
void DEC(void) {
    u8 data = bus_read(temp_address);
    data = data - 1;

    SR.Z = (data==0);
    SR.N = (data>>7);

    bus_write(data,temp_address);
    
    PC++;
}

// Decrement X by 1
void DEX(void) {
    X--;

    SR.Z = (X==0);
    SR.N = (X>>7);

    PC++;
}

// Decrement Y by 1
void DEY(void) {
    Y--;

    SR.Z = (Y==0);
    SR.N = (Y>>7);
    
    PC++;
}

// Transfer A to X
void TAX(void){
    X = A;

    SR.Z = (X==0);
    SR.N = (X>>7);

    PC++;
}

// Transfer A to Y
void TAY(void){
    Y = A;

    SR.Z = (Y==0);
    SR.N = (Y>>7);

    PC++;
}

// Transfer Stackpointer to X
void TSX(void){
    X = SP;

    SR.Z = (X==0);
    SR.N = (X>>7);

    PC++;
}

// Transfer X to A
void TXA(void){
    A = X;

    SR.Z = (A==0);
    SR.N = (A>>7);

    PC++;
}

// Transfer Y to A
void TYA(void){
    A = Y;

    SR.Z = (A==0);
    SR.N = (A>>7);
   
    PC++;
}

// Transfer X to Stackpointer
void TXS(void){
    SP = X;
    PC++;
}

// Store A in Memory(Address might be only 8-Bit)
void STA(void) {
    bus_write(A,temp_address);
    PC++;
}

// Store X in Memory
void STX(void) {
    bus_write(X,temp_address);
    PC++;
}

// Store Y in Memory
void STY(void) {
    bus_write(Y,temp_address);
    PC++;
}

// FLAGS NOT DONE
// Rotate Memory 1 Bit right
void ROR(void) {
    u8 data = bus_read(temp_address);
    data = data >> 1;
    bus_write(data,temp_address);
    PC++;
}

// ROtate Accumulator 1 Bit right
void ROR_A(void) {
    A = A >> 1;
    PC++;
}

//FLAGS NOT DONE
// Rotate Memory 1 Bit left
void ROL(void) {

    u8 data = bus_read(temp_address);
    data = data << 1;
    bus_write(data,temp_address);
    PC++;
}

// Rotate Accumulator 1 Bit left
void ROL_A(void) {
    A = A << 1;
    PC++;
}

// FLAGS NOT DONE
// Shift Bits to right by 1
void LSR(void) {
    u8 data = bus_read(temp_address);
    data = data >> 1;
    bus_write(data,temp_address);
    PC++;
}

// Shift Accumulator right by 1
void LSR_A(void) {
    A = A >> 1;
    PC++;
}

// ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
void ASL(void) {
    u8 data = bus_read(temp_address);
     
    SR.C = data >> 7;;
    data = data << 1;
    clearBit(&data,0);

    SR.Z = (data==0);
    SR.N = (data>>7);
   
    // Writing new Value back to memory location
    bus_write(data,temp_address);
    PC++;
}

// ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
void ASL_A(void){
    
    SR.C = A >> 7;
    A = A << 1;
    clearBit(&A,0);

    SR.Z = (A==0);
    SR.N = (A>>7);
    
    PC++;
}


// Load A with data
void LDA(void) {
    A = temp_data;

    SR.Z = (A==0);
    SR.N = (A>>7);

    PC++;
}

// Load X with data
void LDX(void) {
    X = temp_data;

    SR.Z = (X==0);
    SR.N = (X>>7);
   
    PC++;
}

// Load Y with data
void LDY(void) {
    Y = temp_data;

    SR.Z = (Y==0);
    SR.N = (Y>>7);

    PC++;
}

// Jump to Address, set PC to given Address
void JMP(void) {
    PC = temp_address;
}

// Jump to Subroutine
void JSR(void) {
    bus_write((PC+2),SP); // Push return address to Stack
    SP--;
    PC = bus_read(temp_address);

}

// Add memory to A
void ADC(void) {
    u16 temp = A + temp_data; // Temp Variable to check for Overflow
    A = A + temp_data;

    SR.V = (temp > 255); // Value > 255 means overflow
    SR.Z = (A==0);
    SR.N = (A>>7);

    PC++;
}


// NEED CHECKING FOR CARRY/Borrow
// Substract memory from A
void SBC(void) {
    
    // Substracting data from Accumulator
    A = A - temp_data;

    // Checking for Underflow, ex. if A = 0, and we substract 1 from A -> A = 255
    SR.V = ((A - temp_data) > A);
    SR.Z = (A==0);
    SR.N = (A>>7);
   
    PC++;
}


// Return from Interrupt
void RTI(void) {
    //u8 oldStatus = P;
    SR.reg = bus_read(SP);
    SP++;

    PC = bus_read(SP);
    SP++;
    
}

// Return from Subroutine
void RTS(void) {
    PC = bus_read(SP);
    SP++;
}

// Force Break, software interrupt
void BRK(void) {
    bus_write((PC+2),SP); // Push return address to Stack
    SP--;
    // Set Break Bit
    SR.B = 1;
    bus_write(SR.reg,SP); // Push Status Register to Stack
    SP--;
    PC++; // NOT SURE

}

// Branch if Overflow Bit is 0
void BVC(void) {
    // Skip n-bytes if overflow is cleared
    if(!SR.V) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo;
    } else {
        PC++;
    }
    PC++; 
}

// Branch if Overflow Bit is 1
void BVS(void) {
    // Skip n-bytes if overflow is set
    if(SR.V) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo;
    } else {
        PC++;
    }
    PC++; 
}

// Branch if Result was zero
void BEQ(void) {
    // Skip n-bytes if zero is set
    if(SR.Z) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo; 
    } else {
        PC++;
    }
    PC++; 
}

// Branch if Result was NOT zero 
void BNE(void) {
    if(!SR.Z) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo;
    } else {
        PC++;
    }
    PC++; 
}

// Branch if Result was positive
void BPL(void) { 
    if(!SR.N) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo;
    } else {
        PC++;
    }
    PC++; 
}

// Branch if Result was negative
void BMI(void) {
    if(SR.N) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo;
    } else {
        PC++;
    }
    PC++; 
}

// Branch if Carry is 0
void BCC(void) {
    if(!SR.C) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo;
    } else {
        PC++;
    }
    PC++; 
}

// Branch if carry is 1
void BCS(void) {
    if(SR.C) 
    {   
        PC++;
        u8 foo = bus_read(PC);
        PC = PC + foo;
    } else {
        PC++;
    }
    PC++; 
}

// Compare Accumulator with data
void CMP(void) {
    u8 result = A - temp_data;

    // Set zero Bit if Result is 0 (A and data same value)
    SR.Z = (A == temp_data);
    // Set Carry
    SR.C = (A >= temp_data);
    SR.N = (result >> 7);

    PC++;
}


// Compare X with data
void CPX(void) {
    u8 result = X - temp_data;

    // Set zero Bit if Result is 0 (X and data same value)
    SR.Z = (X == temp_data);
    // Set Carry
    SR.C = (X >= temp_data);
    SR.N = (result >> 7);

    PC++;
}

// Compare Y with data
void CPY(void) {
    u8 result = Y - temp_data;

    // Set zero Bit if Result is 0 (Y and data same value)
    SR.Z = (Y == temp_data);
    // Set Carry
    SR.C = (Y >= temp_data);
    SR.N = (result >> 7);

    PC++;
}

// Bit Test -> check if Bits of a target memory location are set
void BIT(void) {
    u8 data = bus_read(temp_address);
    u8 result = A & data;

    SR.Z = (result==0);
    // Copy Bit 7 and 6 of the value 
    SR.N = (data >> 7);
    SR.V = ((data & (1<<6)) >> 7);
   
    PC++;
}


/*

**********************************************
*               Lookup Tables                *
**********************************************

*/

// Addressing Mode Lookup-table
void (*mode_lookup[256])(void) = {
                                        &noMode,&getIndirectX,&noMode,&noMode,&noMode,&getZPG,&getZPGAddr,&noMode,&noMode,&getImmediate,&noMode,&noMode,&noMode,&getAbsolute,&getAbsoluteAddr,&noMode, // 0x00-0x0F
                                        &noMode,&getIndirectY,&noMode,&noMode,&noMode,&getZPGX,&getZPGXAddr,&noMode,&noMode,&getAbsoluteY,&noMode,&noMode,&noMode,&getAbsoluteX,&getAbsoluteXAddr,&noMode, //0x10-0x1F
                                        &noMode,&getIndirectX,&noMode,&noMode,&getZPG,&getZPGAddr,&noMode,&noMode,&getImmediate,&noMode,&noMode,&noMode,&getAbsoluteAddr,&getAbsoluteAddr,&getAbsoluteAddr,&noMode, // 0x20-0x2F
                                        &noMode,&getIndirectX,&noMode,&noMode,&noMode,&getZPGX,&getZPGXAddr,&noMode,&noMode,&getAbsoluteY,&noMode,&noMode,&noMode,&getAbsoluteX,&getAbsoluteXAddr,&noMode, //0x30-0x3F
                                        &noMode,&getIndirectX,&noMode,&noMode,&noMode,&getZPG,&getZPGAddr,&noMode,&noMode,&getImmediate,&noMode,&noMode,&getAbsoluteAddr,&getAbsolute,&getAbsoluteAddr,&noMode, // 0x40-0x4F
                                        &noMode,&getIndirectY,&noMode,&noMode,&noMode,&getZPGX,&getZPGXAddr,&noMode,&noMode,&getAbsoluteY,&noMode,&noMode,&noMode,&getAbsoluteX,&getAbsoluteXAddr,&noMode, //0x50-0x5F
                                        &noMode,&getIndirectX,&noMode,&noMode,&noMode,&getZPG,&getZPGAddr,&noMode,&noMode,&getImmediate,&noMode,&noMode,&getIndirectAddr,&getAbsolute,&getAbsoluteAddr,&noMode, //0x60-0x6F
                                        &noMode,&getIndirectY,&noMode,&noMode,&noMode,&getZPGX,&getZPGXAddr,&noMode,&noMode,&getAbsoluteY,&noMode,&noMode,&noMode,&getAbsoluteX,&getAbsoluteXAddr,&noMode, //0x70-0x7F
                                        &noMode,&getIndirectXAddr,&noMode,&noMode,&getZPGAddr,&getZPGAddr,&getZPGAddr,&noMode,&noMode,&noMode,&noMode,&noMode,&getAbsoluteAddr,&getAbsoluteAddr,&getAbsoluteAddr,&noMode, // 0x80-0x8F
                                        &noMode,&getIndirectYAddr,&noMode,&noMode,&getZPGXAddr,&getZPGXAddr,&getZPGYAddr,&noMode,&noMode,&getAbsoluteYAddr,&noMode,&noMode,&noMode,&getAbsoluteXAddr,&noMode,&noMode, //0x90-0x9F
                                        &getImmediate,&getIndirectX,&getImmediate,&noMode,&getZPG,&getZPG,&getZPG,&noMode,&noMode,&getImmediate,&noMode,&noMode,&getAbsolute,&getAbsolute,&getAbsolute,&noMode, //0xA0-0xAF
                                        &noMode,&getIndirectY,&noMode,&noMode,&getZPGX,&getZPGX,&getZPGY,&noMode,&noMode,&getAbsoluteY,&noMode,&noMode,&getAbsoluteX,&getAbsoluteX,&getAbsoluteY,&noMode, //0xB0-0xBF
                                        &getImmediate,&getIndirectX,&noMode,&noMode,&getZPG,&getZPG,&getZPGAddr,&noMode,&noMode,&getImmediate,&noMode,&noMode,&getAbsolute,&getAbsolute,&getAbsoluteAddr,&noMode, //0xC0-0xCF
                                        &noMode,&getIndirectY,&noMode,&noMode,&noMode,&getZPGX,&getZPGXAddr,&noMode,&noMode,&getAbsoluteY,&noMode,&noMode,&noMode,&getAbsoluteX,&getAbsoluteXAddr,&noMode, //0xD0-0xDF
                                        &getImmediate,&getIndirectX,&noMode,&noMode,&getZPG,&getZPG,&getZPGAddr,&noMode,&noMode,&getImmediate,&noMode,&noMode,&getAbsolute,&getAbsolute,&getAbsoluteAddr,&noMode, //0xE0-0xEF
                                        &noMode,&getIndirectY,&noMode,&noMode,&noMode,&getZPGX,&getZPGXAddr,&noMode,&noMode,&getAbsoluteY,&noMode,&noMode,&noMode,&getAbsoluteX,&getAbsoluteXAddr,&noMode //0xF0-0xFF
};

// Instruction Lookup-table
void (*inst_lookup[256])(void) = {
                                        &BRK,&ORA,&illegalOpcode,&illegalOpcode,&illegalOpcode,&ORA,&ASL,&illegalOpcode,&PHP,&ORA,&ASL_A,&illegalOpcode,&illegalOpcode,&ORA,&ASL,&illegalOpcode, // 0x00-0x0F
                                        &BPL,&ORA,&illegalOpcode,&illegalOpcode,&illegalOpcode,&ORA,&ASL,&illegalOpcode,&CLC,&ORA,&illegalOpcode,&illegalOpcode,&illegalOpcode,&ORA,&ASL,&illegalOpcode, //0x10-0x1F
                                        &JSR,&AND,&illegalOpcode,&illegalOpcode,&BIT,&AND,&ROL,&illegalOpcode,&PLP,&AND,&ROL_A,&illegalOpcode,&BIT,&AND,&ROL,&illegalOpcode, //0x20-0x2F
                                        &BMI,&AND,&illegalOpcode,&illegalOpcode,&illegalOpcode,&AND,&ROL,&illegalOpcode,&SEC,&AND,&illegalOpcode,&illegalOpcode,&illegalOpcode,&AND,&ROL,&illegalOpcode, //0x30-3F
                                        &RTI,&EOR,&illegalOpcode,&illegalOpcode,&illegalOpcode,&EOR,&LSR,&illegalOpcode,&PHA,&EOR,&LSR_A,&illegalOpcode,&JMP,&EOR,&LSR,&illegalOpcode, //0x40-0x4F
                                        &BVC,&EOR,&illegalOpcode,&illegalOpcode,&illegalOpcode,&EOR,&LSR,&illegalOpcode,&CLI,&EOR,&illegalOpcode,&illegalOpcode,&illegalOpcode,&EOR,&LSR,&illegalOpcode,//0x50-0x5F
                                        &RTS,&ADC,&illegalOpcode,&illegalOpcode,&illegalOpcode,&ADC,&ROR,&illegalOpcode,&PLA,&ADC,&ROR_A,&illegalOpcode,&JMP,&ADC,&ROR,&illegalOpcode, //0x60-0x6F
                                        &BVS,&ADC,&illegalOpcode,&illegalOpcode,&illegalOpcode,&ADC,&ROR,&illegalOpcode,&SEI,&ADC,&illegalOpcode,&illegalOpcode,&illegalOpcode,&ADC,&ROR,&illegalOpcode, //0x70-0x7F
                                        &illegalOpcode,&STA,&illegalOpcode,&illegalOpcode,&STY,&STA,&STX,&illegalOpcode,&DEY,&illegalOpcode,&TXA,&illegalOpcode,&STY,&STA,&STX,&illegalOpcode, //0x80-0x8F
                                        &BCC,&STA,&illegalOpcode,&illegalOpcode,&STY,&STA,&STX,&illegalOpcode,&TYA,&STA,&TXS,&illegalOpcode,&illegalOpcode,&STA,&illegalOpcode,&illegalOpcode, //0x90-0x9F
                                        &LDY,&LDA,&LDX,&illegalOpcode,&LDY,&LDA,&LDX,&illegalOpcode,&TAY,&LDA,&TAX,&illegalOpcode,&LDY,&LDA,&LDX,&illegalOpcode, //0xA0-0xAF
                                        &BCS,&LDA,&illegalOpcode,&illegalOpcode,&LDY,&LDA,&LDX,&illegalOpcode,&CLV,&LDA,&TSX,&illegalOpcode,&LDY,&LDA,&LDX,&illegalOpcode, //0xB0-0xBF
                                        &CPY,&CMP,&illegalOpcode,&illegalOpcode,&CPY,&CMP,&DEC,&illegalOpcode,&INY,&CMP,&DEX,&illegalOpcode,&CPY,&CMP,&DEC,&illegalOpcode,//0xC0-0xCF
                                        &BNE,&CMP,&illegalOpcode,&illegalOpcode,&illegalOpcode,&CMP,&DEC,&illegalOpcode,&CLD,&CMP,&illegalOpcode,&illegalOpcode,&illegalOpcode,&CMP,&DEC,&illegalOpcode,//0xD0-0xDF
                                        &CPX,&SBC,&illegalOpcode,&illegalOpcode,&CPX,&SBC,&INC,&illegalOpcode,&INX,&SBC,&NOP,&illegalOpcode,&CPX,&SBC,&INC,&illegalOpcode,//0xE0-0xEF
                                        &BEQ,&SBC,&illegalOpcode,&illegalOpcode,&illegalOpcode,&SBC,&INC,&illegalOpcode,&SED,&SBC,&illegalOpcode,&illegalOpcode,&illegalOpcode,&SBC,&INC,&illegalOpcode //0xF0-0xFF
};

// Cycles per Instruction Lookup Table
const u8 cycle_lookup[256] = {
                                    7, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 4, 4, 6, 6, //0x00-0x0F
                                    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, //0x10-0x1F
                                    6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 4, 4, 6, 6, //0x20-0x2F
                                    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, //0x30-0x3F
                                    6, 6, 0, 8, 3, 3, 5, 5, 3, 2, 2, 2, 3, 4, 6, 6, //0x40-0x4F
                                    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, //0x50-0x5F
                                    6, 6, 0, 8, 3, 3, 5, 5, 4, 2, 2, 2, 5, 4, 6, 6, //0x60-0x6F
                                    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, //0x70-0x7F
                                    2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 0, 4, 4, 4, 4, //0x80-0x8F
                                    2, 6, 0, 0, 4, 4, 4, 4, 2, 5, 2, 0, 0, 5, 0, 0, //0x90-0x9F
                                    2, 6, 2, 6, 3, 3, 3, 3, 2, 2, 2, 0, 4, 4, 4, 4, //0xA0-0xAF
                                    2, 5, 0, 5, 4, 4, 4, 4, 2, 4, 2, 0, 4, 4, 4, 4, //0xB0-0xBF
                                    2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, //0xC0-0xCF
                                    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7, //0xD0-0xDF
                                    2, 6, 2, 8, 3, 3, 5, 5, 2, 2, 2, 2, 4, 4, 6, 6, //0xE0-0xEF
                                    2, 5, 0, 8, 4, 4, 6, 6, 2, 4, 2, 7, 4, 4, 7, 7  //0xF0-0xFF
};


/*

**********************************************
*       CPU Functions                        *
**********************************************

*/


// Reset internal CPU Register 
void CPU_RESET(void) {
    A = 0;
    X = 0;
    Y = 0;
    SR.reg = 0;
    SR.unused = 1;
    PC = bus_read(0xFFFD)  << 8 | bus_read(0xFFFC); // Set PC to Reset Vector
    SP = 0xFF;
    printf("PC on RESET -> HEX: %x DEC: %d \n",PC,PC);
}


// Decoding and executing instructions
void CPU_RUN(void) {
    
    opcode = bus_read(PC);

    // Checking which addressing mode the instruction will be using
    (*mode_lookup[opcode])(); 

    // Checking which instruction corresponds to the opcode, then executing that instruction
    (*inst_lookup[opcode])();
    
}

void CPU_STATUS(void) {
    printf("---------------\n");
    printf("OPCODE: %x\n",opcode);
    printf("A:\t%X\n",A);
    printf("X:\t%X\n",X);
    printf("Y:\t%X\n",Y);
    printf("SP:\t%X\n",SP);
    printf("PC:\t%X\n",PC);
    printf("\t\tCZIDBUVN\n");
    printf("STATUS:\t%x\t",SR.reg);
    bin(SR.reg,8);
    printf("\n");

}
