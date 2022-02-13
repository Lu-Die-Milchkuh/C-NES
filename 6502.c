#ifndef _STDIO_H
#include <stdio.h>
#endif

#ifndef	_STDLIB_H
#include <stdlib.h>
#endif

#include "6502.h"
#include "HelperFunc.h"
#include "Bus.h"

byte A = 0;
byte X = 0;
byte Y = 0;
word PC = 0;
byte SP = 0;
byte P = 0;
byte memory[2048] = {0};

// Var to store temporary data or address
byte temp_data = 0;
word temp_address = 0;


// opcode -> byte that identifies the instruction
byte opcode = 0;

/*

**********************************************
*     Addressing Mode Functions, get Data    *
**********************************************
    
*/

// Return next Byte, Immediate Addressing Mode
byte getImmediate() {
    PC++;
    temp_data = read(PC);   
    return 0;
}

// Return Data from Zero Page, only capable of addressing first 256 Bytes of Memory
byte getZPG() {
    byte zpg_addr = 0;  // Zero Page Address
    PC++;
    zpg_addr = read(PC);
    temp_data = read(zpg_addr);
    return 0;

}


// Return Data from Zero Page + X(offset)
byte getZPGX() {
    byte zpg_addr = 0;  // Zero Page Addrsss
    PC++;
    zpg_addr = read(PC) + X;  // Adding Offset
    temp_data = read(zpg_addr);
    return 0;
}

// Return Data from Zero Page + Y(offset)
byte getZPGY() {
    byte zpg_addr = 0;  // Zero Page Addrsss
    PC++;
    zpg_addr = read(PC) + Y;  // Adding Offset
    temp_data = read(zpg_addr);
    return 0;
}


// Return Data stored @ Address in Memory, Absolute Addressing Mode
byte getAbsolute() {
    word address = 0;
    byte highByte,lowByte;

    PC++;
    highByte = read(PC);
    PC++;
    lowByte = read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte); 
    temp_data = read(address);

    return 0;
}


// Return Data stored @ Address + X in Memory, Absolute X-Indexed Addressing Mode
byte getAbsoluteX() {
    word address = 0;
    byte highByte,lowByte;

    PC++;
    highByte = read(PC);
    PC++;
    lowByte = read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte); 
    address += X;   // Adding Offset
    temp_data = read(address);
    return 0;
}


// Return Data stored @ Address + Y in Memory, Absolute Y-Indexed Addressing Mode
byte getAbsoluteY() {
    word address = 0;
    byte highByte,lowByte;

    PC++;
    highByte = read(PC);
    PC++;
    lowByte = read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte); 
    address += Y;   // Adding Offset
    temp_data = read(address);

    return 0;
}


/*
    NEED TO BE LOOKED AT IN THE FUTURE
*/

// Indirect Addressing Mode, given Address is used to calculate the real address where data is stored
byte getIndirect() {
    byte tmp_addr1 = 0; // HighByte
    byte tmp_addr2 = 0; //LowByte
    word tmp_addr = 0;  //Placeholder Address
    word real_addr = 0;

    PC++;
    tmp_addr1 = read(PC);
    PC++;
    tmp_addr2 = read(PC);

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 

    real_addr = read(tmp_addr+1) << 8 | read(tmp_addr);
    temp_data = read(real_addr);

    return 0;
}

// Indirect Addressing Mode + Offset X
byte getIndirectX() {
    byte tmp_addr1 = 0;
    byte tmp_addr2 = 0;
    word tmp_addr = 0;
    word real_addr = 0;

    PC++;
    tmp_addr1 = read(PC) + X;
    PC++;
    tmp_addr2 = read(PC) + X;

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    real_addr = read(tmp_addr+1) << 8 | read(tmp_addr);

    temp_data = read(real_addr);
    return 0;
}

// Indirect Addressing Mode + Offset Y
byte getIndirectY() {
    byte tmp_addr1 = 0;
    byte tmp_addr2 = 0;
    word tmp_addr = 0;
    word real_addr = 0;

    PC++;
    tmp_addr1 = read(PC);
    PC++;
    tmp_addr2 = read(PC) ;

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    real_addr = read(tmp_addr+1) << 8 | read(tmp_addr);
    real_addr += Y;
    temp_data = read(real_addr);
    return 0;
}

// Some instrctions are implied . which means all the needed infromations are already present
byte noMode() {
    return 0;
}


/*

**********************************************
*     Addressing Mode Functions, get Address *
**********************************************
    
*/

// Returns absolute Address
byte getAbsoluteAddr() {
    byte highByte,lowByte;

    PC++;
    lowByte = read(PC); 
    PC++;
    highByte = read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    temp_address = (highByte << 8 | lowByte); 

    return 0;
}

// Returns absolute, X-indexed Address
byte getAbsoluteXAddr() {
    byte highByte,lowByte;

    PC++;
    lowByte = read(PC);
    PC++;
    highByte = read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    temp_address = (highByte << 8 | lowByte); 
    temp_address += X;   // Adding Offset

    return 0;
}


// Returns absolute, Y-indexed Address
byte getAbsoluteYAddr() {
    byte highByte,lowByte;

    PC++;
    lowByte = read(PC);
    PC++;
    highByte = read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    temp_address = (highByte << 8 | lowByte); 
    temp_address += Y;   // Adding Offset

    return 0;
}


// Returns zeropage address
byte getZPGAddr() {
    PC++;
    temp_address = read(PC);
    
    return 0;

}

// Returns zeropage, X-indexed Address
byte getZPGXAddr() {
    PC++;
    temp_address = read(PC) + X;  // Adding Offset
    
    return 0;
}

// Returns zeropage, X-indexed Address
byte getZPGYAddr() {
    PC++;
    temp_address = read(PC) + Y;  // Adding Offset
    
    return 0;
}


// Returns an indirect Address
byte getIndirectAddr() {
    byte tmp_addr1 = 0; // HighByte
    byte tmp_addr2 = 0; //LowByte
    word tmp_addr = 0;  //Placeholder Address

    PC++;
    tmp_addr1 = read(PC);
    PC++;
    tmp_addr2 = read(PC);

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 

    temp_address = read(tmp_addr+1) << 8 | read(tmp_addr);
    
    return 0;
}

// Returns indirect, x-indexed Address
byte getIndirectXAddr() {
    byte tmp_addr1 = 0;
    byte tmp_addr2 = 0;
    word tmp_addr = 0;

    PC++;
    tmp_addr1 = read(PC) + X;
    PC++;
    tmp_addr2 = read(PC) + X;

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    temp_address = read(tmp_addr+1) << 8 | read(tmp_addr);

    return 0;
}

// Returns indirect, y-indexed Address
byte getIndirectYAddr() {
    byte tmp_addr1 = 0;
    byte tmp_addr2 = 0;
    word tmp_addr = 0;

    PC++;
    tmp_addr1 = read(PC);
    PC++;
    tmp_addr2 = read(PC);

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr2 << 8 | tmp_addr1; 
    temp_address = read(tmp_addr+1) << 8 | read(tmp_addr);
    temp_address += Y;

    return 0;
}


/*

**********************************************
*       6502 Instruction Functions           *
**********************************************

*/

// Incase an illegal/undocumented opcode occurs, exit
void illegalOpcode() {
    printf("ILLEGAL INSTRUCTION: %X",opcode);
    exit(-1);
}


// Bitwise AND Memory with Accumulator
void AND() {
    A &= temp_data;

    if(A == 0) 
    {   // Set Zero Flag
        setBit(&P,1);
    }
    
    // Set Negative Flag 
    // to 7th Bit of A
    clearBit(&P,7);
    P |= getBit(&A,7);
    PC++;
}

// Bitwise XOR with Accumulator
void EOR() {
    A ^= temp_data;

    if(A == 0) 
    {   // Set Zero Flag
        setBit(&P,1);
    }
    
    // Set Negative Flag 
    // to 7th Bit of A
    clearBit(&P,7);
    P |= getBit(&A,7);
    PC++;
}

// Bitwise OR with Accumulator
void ORA() {
    A |= temp_data;
    if(A == 0) 
    {   // Set Zero Flag
        setBit(&P,1);
    }
    
    // Set Negative Flag 
    // to 7th Bit of A
    clearBit(&P,7);
    P |= getBit(&A,7);
    PC++;
}


// NO Operation -> do nothing
void NOP() {
    PC++;
}


// Set Decimal Flag
void SED() {
    setBit(&P,3);
    PC++;
}


// Set Interrupt Flag
void SEI() {
    setBit(&P,2);
    PC++;
}


// Set Carry Flag
void SEC() {
    setBit(&P,0);
    PC++;
}


// Clear Carry Flag
void CLC() {
    clearBit(&P,0);
    PC++;
}


// Clear Decimal Bit
void CLD() {
    clearBit(&P,3);
    PC++;
}


// Clear Interrupt Flag
void CLI() {
    clearBit(&P,2);
    PC++;
}


// Clear Overflow Flag
void CLV() {
    clearBit(&P,6);
    PC++;
}


// Push A on Stack
void PHA() {
    write(A,SP);
    SP--;
    PC++;
}


// Pull A from Stack
void PLA() {
    A = read(SP);
    
    if(A == 0) 
    {   // Set Zero Flag
        setBit(&P,1);
    }
    
    // Set Negative Flag 
    // to 7th Bit of A
    clearBit(&P,7);
    P |= getBit(&A,7);
    SP++;
    PC++;
    
}

// Push Status Register on Stack
void PHP() {
    write(P,SP);
    SP--;
    PC++;
}

// Pull Status Register from Stack
void PLP() {
    P = read(SP);
    SP++;
    PC++;
}


// Increase X by 1
void INX() {
    X++;

    if(X == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&X,7);
    PC++;
}

// Increase Y by 1
void INY() {
    Y++;

    if(Y == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&Y,7);
    PC++;
}

// Increase Memory by 1
void INC() {
    byte foo = read(temp_address);
    foo++;
    write(foo,temp_address);

    if(read(temp_address) == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&foo,7);
    PC++;
}

// Decrement Memory by 1
void DEC() {
    byte data = read(temp_address);
    data = data - 1;

    if(data == 0)
    {
        setBit(&P,1);
    }
    write(data,temp_address);
    clearBit(&P,7);
    P |= getBit(&data,7);
    PC++;
}

// Decrement X by 1
void DEX() {
    X--;

    if(X == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&X,7);
    PC++;
}

// Decrement Y by 1
void DEY() {
    Y--;
    
    if(Y == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&Y,7);
    PC++;
}

// Transfer A to X
void TAX(){
    X = A;
    if(X == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&X,7);
    PC++;
}

// Transfer A to Y
void TAY(){
    Y = A;
    if(Y == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&Y,7);
    PC++;
}

// Transfer Stackpointer to X
void TSX(){
    X = SP;
    if(X == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&X,7);
    PC++;
}

// Transfer X to A
void TXA(){
    A = X;
    if(A == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&A,7);
    PC++;
}

// Transfer Y to A
void TYA(){
    A = Y;
    if(A == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&A,7);
    PC++;
}

// Transfer X to Stackpointer
void TXS(){
    SP = X;
    if(SP == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&SP,7);
    PC++;
}

// Store A in Memory(Address might be only 8-Bit)
void STA() {
    write(A,temp_address);
    PC++;
}

// Store X in Memory
void STX() {
    write(X,temp_address);
    PC++;
}

// Store Y in Memory
void STY() {
    write(Y,temp_address);
    PC++;
}

// FLAGS NOT DONE
// Rotate Memory 1 Bit right
void ROR() {
    byte data = read(temp_address);
    data = data >> 1;
    write(data,temp_address);
    PC++;
}

// ROtate Accumulator 1 Bit right
void ROR_A() {
    A = A >> 1;
    PC++;
}

//FLAGS NOT DONE
// Rotate Memory 1 Bit left
void ROL() {

    byte data = read(temp_address);
    data = data << 1;
    write(data,temp_address);
    PC++;
}

// Rotate Accumulator 1 Bit left
void ROL_A() {
    A = A << 1;
    PC++;
}

// FLAGS NOT DONE
// Shift Bits to right by 1
void LSR() {
    byte data = read(temp_address);
    data = data >> 1;
    write(data,temp_address);
    PC++;
}

// Shift Accumulator right by 1
void LSR_A() {
    A = A >> 1;
    PC++;
}

// ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
void ASL() {
    byte data = read(temp_address);
    byte oldCarry = getBit(&data,7) >> 7;

    P |= oldCarry;
    data = data << 1;
    clearBit(&data,0);

    if(data == 0)
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&data,7);
    // Writing new Value back to memory location
    write(data,temp_address);
    PC++;
}

// ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
void ASL_A(){
    byte oldCarry = getBit(&A,7) >> 7;
    P |= oldCarry;
    A = A << 1;
    clearBit(&A,0);

    if(A == 0)
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&A,7);
    PC++;
}


// Load A with data
void LDA() {
    A = temp_data;
    if(A == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&A,7);
    PC++;
}

// Load X with data
void LDX() {
    X = temp_data;
    if(X == 0) {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&X,7);
    PC++;
}

// Load Y with data
void LDY() {
    Y = temp_data;
    if(Y == 0) {
        setBit(&P,1);
    }
    
    clearBit(&P,7);
    P |= getBit(&Y,7);
    PC++;
}

// Jump to Address, set PC to given Address
void JMP() {
    PC = temp_address;
}

// Jump to Subroutine
void JSR() {
    write((PC+2),SP); // Push return address to Stack
    SP--;
    //word address = getAbsoluteAddr();
    PC = read(temp_address);

}

// Add memory to A
void ADC() {
    word temp = A + temp_data; // Temp Variable to check for Overflow
    A = A + temp_data;

    if(temp > 255) // Value > 255 means overflow
    {
        setBit(&P,6);
    }

    if(A == 0) 
    {
       setBit(&P,1); 
    }

    // Load Sign Bit into Status Register
    clearBit(&P,7);
    P |= getBit(&Y,7);
    PC++;
}


// NEED CHECKING FOR CARRY/Borrow
// Substract memory from A
void SBC() {
    // Checking for Underflow, ex. if A = 0, and we substract 1 from A -> A = 255
    if(A < (A - temp_data)) 
    {
        setBit(&P,6);
    }
    // Substracting data from Accumulator
    A = A - temp_data;

    if(A == 0) 
    {
       setBit(&P,1); 
    }

    // Load Sign Bit into Status Register
    clearBit(&P,7);
    P |= getBit(&Y,7);
    PC++;
}


// Return from Interrupt
void RTI() {
    //byte oldStatus = P;
    P = read(SP);
    SP++;

    PC = read(SP);
    SP++;
    
}

// Return from Subroutine
void RTS() {
    PC = read(SP);
    SP++;
}

// Force Break, software interrupt
void BRK() {
    write((PC+2),SP); // Push return address to Stack
    SP--;
    // Set Break Bit
    setBit(&P,5);
    write(P,SP); // Push Status Register to Stack
    SP--;
    PC++; // NOT SURE

}

// Branch if Overflow Bit is 0
void BVC() {
    // Skip n-bytes if overflow is cleared
    if(getBit(&P,6) == 0x00) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Branch if Overflow Bit is 1
void BVS() {
    // Skip n-bytes if overflow is set
    if(getBit(&P,6) == 0x40) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Branch if Result was zero
void BEQ() {
  if(getBit(&P,1) == 0x2) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Branch if Result was NOT zero 
void BNE() {
    if(getBit(&P,1) == 0x00) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Branch if Result was positive
void BPL() {
    if(getBit(&P,7) == 0x00) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Branch if Result was negative
void BMI() {
    if(getBit(&P,7) == 0x80) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Branch if Carry is 0
void BCC() {
    if(getBit(&P,0) == 0x00) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Branch if carry is 1
void BCS() {
    if(getBit(&P,0) == 0x01) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
    }
    else {
       PC++; 
    }
}

// Compare Accumulator with data
void CMP() {
    byte result = A - temp_data;

    // Set zero Bit if Result is 0 (A and data same value)
    if(A == temp_data)
    {
        setBit(&P,1);
    }

    if(A >= temp_data) 
    {
        setBit(&P,0); // Set Carry
    }

    // If Bit 7 of Result is 1, Bit 7 of P will be 1 too
    clearBit(&P,7);
    P |= getBit(&result,7);
    PC++;
}


// Compare X with data
void CPX() {
    byte result = X - temp_data;

    // Set zero Bit if Result is 0 (X and data same value)
    if(X == temp_data)
    {
        setBit(&P,1);
    }

    if(X >= temp_data) 
    {
        setBit(&P,0); // Set Carry
    }

    // If Bit 7 of Result is 1, Bit 7 of P will be 1 too
    clearBit(&P,7);
    P |= getBit(&result,7);
    PC++;
}

// Compare Y with data
void CPY() {
    byte result = Y - temp_data;

    // Set zero Bit if Result is 0 (Y and data same value)
    if(Y == temp_data)
    {
        setBit(&P,1);
    }

    if(Y >= temp_data) 
    {
        setBit(&P,0); // Set Carry
    }

    // If Bit 7 of Result is 1, Bit 7 of P will be 1 too
    clearBit(&P,7);
    P |= getBit(&result,7);
    PC++;
}

// Bit Test -> check if Bits of a target memory location are set
void BIT() {
    byte data = read(temp_address);
    byte result = A & data;

    if(result == 0)
    {
        setBit(&P,1);
    }

    // Copy Bit 7 and 6 of the value to
    clearBit(&P,7);
    clearBit(&P,6);
    P |= getBit(&data,7);
    P |= getBit(&data,6);
    PC++;
}


/*

**********************************************
*               Lookup Tables                *
**********************************************

*/

// Addressing Mode Lookup-table
const byte (*mode_lookup[256])() = {
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
const void (*inst_lookup[256])() = {
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
const byte cycle_lookup[256] = {
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
void CPU_RESET() {
    A = 0;
    X = 0;
    Y = 0;
    P = 0;
    PC = read(0xFFFD)  << 8 | read(0xFFFC); // Set PC to Reset Vector
    SP = 0xFF;
    printf("PC on RESET: HEX: %x DEC: %d \n",PC,PC);
}


// Decoding and executing instructions
void CPU_RUN() {
    
    opcode = read(PC);
    printf("OPCODE: %x\n",opcode);

    // Checking which addressing mode the instruction will be using
    (*mode_lookup[opcode])(); 

    // Checking which instruction corresponds to the opcode, then executing that instruction
    (*inst_lookup[opcode])();
    
}

void CPU_STATUS() {
    printf("A:\t%X\n",A);
    printf("X:\t%X\n",X);
    printf("Y:\t%X\n",Y);
    printf("SP:\t%X\n",SP);
    printf("PC:\t%X\n",PC);
    bin(P,8);
}

