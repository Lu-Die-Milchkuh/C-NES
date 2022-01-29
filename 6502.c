#include <stdio.h>
#include <stdlib.h>
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


/*

**********************************************
*     Addressing Mode Functions, get Data    *
**********************************************
    
*/

// Return next Byte, Immediate Addressing Mode
byte getImmediate() {
    PC++;
    byte data = read(PC);   
    return data;
}

// Return Data from Zero Page, only capable of addressing first 256 Bytes of Memory
byte getZPG() {
    byte zpg_addr = 0;  // Zero Page Address
    PC++;
    zpg_addr = read(PC);
    byte data = read(zpg_addr);
    return data;

}


// Return Data from Zero Page + X(offset)
byte getZPGX() {
    byte zpg_addr = 0;  // Zero Page Addrsss
    PC++;
    zpg_addr = read(PC) + X;  // Adding Offset
    byte data = read(zpg_addr);
    return data;
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
    byte data = read(address);

    return data;
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
    byte data = read(address);
    return data;
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
    byte data = read(address);

    return data;
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
    tmp_addr = tmp_addr1 << 8 | tmp_addr2; 

    real_addr = read(tmp_addr) << 8 | read((tmp_addr+1));
    byte data = read(real_addr);

    return data;
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
    tmp_addr = tmp_addr1 << 8 | tmp_addr2; 
    real_addr = read(tmp_addr) << 8 | read((tmp_addr+1));

    byte data = read(real_addr);
    return data;
}

// Indirect Addressing Mode + Offset Y
byte getIndirectY() {
    byte tmp_addr1 = 0;
    byte tmp_addr2 = 0;
    word tmp_addr = 0;
    word real_addr = 0;

    PC++;
    tmp_addr1 = read(PC)  ;

    PC++;
    tmp_addr2 = read(PC) ;

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr1 << 8 | tmp_addr2; 
    real_addr = read(tmp_addr) << 8 | read((tmp_addr+1));
    real_addr += Y;
    byte data = read(real_addr);
    return data;
}



/*

**********************************************
*     Addressing Mode Functions, get Address *
**********************************************
    
*/

// Returns absolute Address
word getAbsoluteAddr() {
    word address = 0;
    byte highByte,lowByte;

    PC++;
    highByte = read(PC);
    PC++;
    lowByte = read(PC);

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte); 

    return address;
}

// Returns absolute, X-indexed Address
word getAbsoluteXAddr() {
    word address = 0;
    byte highByte,lowByte;

    PC++;
    highByte = memory[PC];
    PC++;
    lowByte = memory[PC];

    // Creating a 16 Bit Address out of 2 Bytes
    address = (highByte << 8 | lowByte); 
    address += X;   // Adding Offset

    return address;
}

// Returns zeropage, X-indexed Address
word getZPGXAddr() {
    byte zpg_addr = 0;  // Zero Page Addrsss
    PC++;
    zpg_addr = read(PC) + X;  // Adding Offset
    
    return zpg_addr;
}


/*

**********************************************
*       6502 Instruction Functions           *
**********************************************

*/


// Bitwise AND Memory with Accumulator
void AND(byte data) {
    A &= data;

    if(A == 0) 
    {   // Set Zero Flag
        setBit(&P,1);
    }
    
    // Set Negative Flag 
    // to 7th Bit of A
    clearBit(&P,7);
    P |= getBit(&A,7);

}

// Bitwise XOR with Accumulator
void EOR(byte data) {
    A ^= data;

    if(A == 0) 
    {   // Set Zero Flag
        setBit(&P,1);
    }
    
    // Set Negative Flag 
    // to 7th Bit of A
    clearBit(&P,7);
    P |= getBit(&A,7);
}

// Bitwise OR with Accumulator
void ORA(byte data) {
    A |= data;
    if(A == 0) 
    {   // Set Zero Flag
        setBit(&P,1);
    }
    
    // Set Negative Flag 
    // to 7th Bit of A
    clearBit(&P,7);
    P |= getBit(&A,7);
}


// NO Operation -> do nothing
void NOP() {

}


// Set Decimal Flag
void SED() {
    setBit(&P,3);
}


// Set Interrupt Flag
void SEI() {
    setBit(&P,2);
}


// Set Carry Flag
void SEC() {
    setBit(&P,0);
}


// Clear Carry Flag
void CLC() {
    clearBit(&P,0);
}


// Clear Decimal Bit
void CLD() {
    clearBit(&P,3);
}


// Clear Interrupt Flag
void CLI() {
    clearBit(&P,2);
}


// Clear Overflow Flag
void CLV() {
    clearBit(&P,6);
}


// Push A on Stack
void PHA() {
    write(A,SP);
    //memory[SP] = A;
    SP--;
}


// Pull A from Stack
void PLA() {
    //A = memory[SP];
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
    
}

// Push Status Register on Stack
void PHP() {
    //memory[SP] = P;
    write(P,SP);
    SP--;
}

// Pull Status Register from Stack
void PLP() {
    //P = memory[SP];
    P = read(SP);
    SP++;
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
}

// Increase Memory by 1
void INC(word address) {
    byte foo = read(address);
    foo++;
    write(foo,address);

    if(read(address) == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&foo,7);

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
}

// Store A in Memory(Address might be only 8-Bit)
void STA(word address) {
    //memory[address] = A;
    write(A,address);
}

// Store X in Memory
void STX(word address) {
    //memory[address] = X;
    write(X,address);
}

// Store Y in Memory
void STY(word address) {
    //memory[address] = Y;
    write(Y,address);
}

// FLAGS NOT DONE
// Rotate Accumulator or Memory 1 Bit right
void ROR(byte* data) {
    *data = *data >> 1;
}

//FLAGS NOT DONE
// Rotate Accumulator or Memory 1 Bit left
void ROL(byte* data) {
    *data = *data << 1;
}

// FLAGS NOT DONE
// Shift Bits to right by 1
void LSR(byte* data) {
    *data = *data >> 1;
}

// ASL shifts all bits left one position. 0 is shifted into bit 0 and the original bit 7 is shifted into the Carry.
void ASL(word address) {
    byte data = read(address);
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
    write(data,address);
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

}


// Load A with data
void LDA(byte data) {
    A = data;
    if(A == 0) 
    {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&A,7);
}

// Load X with data
void LDX(byte data) {
    X = data;
    if(X == 0) {
        setBit(&P,1);
    }

    clearBit(&P,7);
    P |= getBit(&X,7);
}

// Load Y with data
void LDY(byte data) {
    Y = data;
    if(Y == 0) {
        setBit(&P,1);
    }
    
    clearBit(&P,7);
    P |= getBit(&Y,7);
}

// Jump to Address, set PC to given Address
void JMP(word address) {
    PC = address;
}

// Jump to Subroutine
void JSR() {
    write((PC+2),SP); // Push return address to Stack
    SP--;
    word address = getAbsoluteAddr();
    PC = read(address);

}

// Add memory to A
void ADC(byte data) {
    word temp = A + data; // Temp Variable to check for Overflow
    A = A + data;

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
}


// NEED CHECKING FOR CARRY/Borrow
// Substract memory from A
void SBC(byte data) {
    // Checking for Underflow, ex. if A = 0, and we substract 1 from A -> A = 255
    if(A < (A - data)) 
    {
        setBit(&P,6);
    }
    // Substracting data from Accumulator
    A = A - data;

    if(A == 0) 
    {
       setBit(&P,1); 
    }

    // Load Sign Bit into Status Register
    clearBit(&P,7);
    P |= getBit(&Y,7);

}


// Return from Interrupt
void RTI() {
    byte oldStatus = P;
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
void CMP(byte data) {
    byte result = A - data;

    // Set zero Bit if Result is 0 (A and data same value)
    if(A == data)
    {
        setBit(&P,1);
    }

    if(A >= data) 
    {
        setBit(&P,0); // Set Carry
    }

    // If Bit 7 of Result is 1, Bit 7 of P will be 1 too
    clearBit(&P,7);
    P |= getBit(&result,7);
}


// Compare X with data
void CPX(byte data) {
    byte result = X - data;

    // Set zero Bit if Result is 0 (X and data same value)
    if(X == data)
    {
        setBit(&P,1);
    }

    if(X >= data) 
    {
        setBit(&P,0); // Set Carry
    }

    // If Bit 7 of Result is 1, Bit 7 of P will be 1 too
    clearBit(&P,7);
    P |= getBit(&result,7);
}

// Compare Y with data
void CPY(byte data) {
    byte result = Y - data;

    // Set zero Bit if Result is 0 (Y and data same value)
    if(Y == data)
    {
        setBit(&P,1);
    }

    if(Y >= data) 
    {
        setBit(&P,0); // Set Carry
    }

    // If Bit 7 of Result is 1, Bit 7 of P will be 1 too
    clearBit(&P,7);
    P |= getBit(&result,7);
}

// Bit Test -> check if Bits of a target memory location are set
void BIT(word address) {
    byte data = read(address);
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
}


// Decoding and executing instructions, might be better to create a loookup table in the future
void CPU_RUN() {
    byte opcode = read(PC); // opcode -> byte that identifies the instruction

    byte data = 0; // Helper Var to store data
    byte address = 0; // Helper Var to store an address

    switch(opcode) {
        case 0x00: // BRK
            BRK(); // NOT SURE IF PC++
            break;

        case 0x01: // ORA X,ind
            data = getIndirectX();
            ORA(data);
            PC++;
            break;

        case 0x02: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x03: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x04: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x05: // ORA zpg
            address = getImmediate(); // Zero Page Address are always 1-Byte 
            data = read(address);
            ORA(data);
            PC++;
            break;
        
        case 0x06: // ASL zpg
            address = getImmediate(); // Zero Page Address are always 1-Byte 
            data = read(address);
            ASL(data);
            PC++;
            break;
        
        case 0x07: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x08: // PHP impl
            PHP();
            PC++;
            break;
        
        case 0x09: // ORA imm
            data = getImmediate();
            ORA(data);
            PC++;
            break;
        
        case 0x0A: // ASL A
            ASL_A();
            PC++;
            break;
        
        case 0x0B: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x0C: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x0D: // ORA abs
            data = getAbsolute();
            ORA(data);
            PC++;
            break;
        
        case 0x0E: // ASL abs
            address = getAbsoluteAddr();
            ASL(address);
            PC++;
            break;
        
        case 0x0F: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x10: // BPL
            BPL();
            break;
        
        case 0x11: // ORA ind,Y
            data = getIndirectY();
            ORA(data);
            PC++;
            break;
        
        case 0x12: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x13: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x14: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x15: // ORA zpg,x
            data = getZPGX();
            ORA(data);
            PC++;
            break;
        
        case 0x16: // ASL zpg,x
            address = getZPGXAddr();
            ASL(address);
            PC++;
            break;
        
        case 0x17: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x18: // CLC
            CLC();
            break;
        
        case 0x19: // ORA abs,y
            data = getAbsoluteX();
            ORA(data);
            break;
        
        case 0x1A: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x1B: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x1C: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x1D: // ORA abs,x
            data = getAbsoluteX();
            ORA(data);
            PC++;
            break;
        
        case 0x1E: // ASL abs,x
            address = getAbsoluteXAddr();
            ASL(address);
            PC++;
            break;
        
        case 0x1F: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x20: // JSR abs
            JSR();
            break;
        
        case 0x21: // AND X,ind
            data = getIndirectX();
            AND(data);
            PC++;
            break;
        
        case 0x22: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x23: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x24: // BIT zpg
            address = getImmediate();
            BIT(address);
            PC++;
            break;
        
        case 0x25: // AND zpg
            data = getZPG();
            AND(data);
            PC++;
            break;
        
        case 0x26: // ROL zpg
            address = getZPG();
            data = read(address);
            ROL(&data);
            write(data,address); // Write value back to Memory
            PC++;
            break;
        
        case 0x27: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x28: // PLP
            PLP();
            PC++;
            break;
        
        case 0x29: // AND imm
            data = getImmediate();
            AND(data);
            PC++;
            break;
        
        case 0x2A: // ROL A
            ROL(&A);
            PC++;
            break;
        
        case 0x2B: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x2C: // BIT abs
            address = getAbsoluteAddr();
            BIT(address);
            PC++;
            break;
        
        case 0x2D: // AND abs
            data = getAbsolute();
            AND(data);
            PC++;
            break;
        
        case 0x2E: // ROL abs
            address = getAbsoluteAddr();
            data = read(address);
            ROL(&data);
            write(data,address);
            PC++;
            break;
        
        case 0x2F: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x30: // BMI rel
            BMI();
            break;
        
        case 0x31: // AND ind,y
            data = getIndirectY();
            AND(data);
            PC++;
            break;
        
        case 0x32: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x33: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x34: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x35: // AND zpg,x
            data = getZPGX();
            AND(data);
            PC++;
            break;
        
        case 0x36: // ROL zpg,x
            address = getZPGXAddr();
            data = read(address);
            ROL(&data);
            write(data,address);
            PC++;
            break;
        
        case 0x37: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x38: // SEC
            SEC();
            PC++;
            break;
        
        case 0x39: // AND abs,y
            data = getAbsoluteY();
            AND(data);
            PC++;
            break;
        
        case 0x3A: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x3B: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x3C: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x3D: // AND abs,x
            data = getAbsoluteX();
            AND(data);
            PC++;
            break;

        case 0x3E: // ROL abs,x
            address = getAbsoluteXAddr();
            data = read(address);
            ROL(&data);
            write(data,address);
            PC++;
            break;

        case 0x3F: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x40: // RTI
            RTI();
            break;
        
        case 0x41: // EOR x,ind
            data = getIndirectX();
            EOR(data);
            PC++;
            break;

        case 0x42: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x43: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;

        case 0x44: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x45: // EOR zpg
            data = getZPG();
            EOR(data);
            PC++;
            break;
        
        case 0x46: // LSR zpg
            address = getImmediate();
            data = read(address);
            LSR(&data);
            write(data,address);
            PC++;
            break;
        
        case 0x47: // ILLEGAL OPCODE
            printf("ILLEGAL OPCODE: %X",opcode);
            exit(-1);
            break;
        
        case 0x48: // PHA
            PHA();
            PC++;
            break;
    }   
}

