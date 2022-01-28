#include <stdio.h>
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
    tmp_addr1 = read(PC) + Y ;

    PC++;
    tmp_addr2 = read(PC) + Y;

    // Real Address has to be calculated, from 
    // the content of the given address and (address+1)!
    tmp_addr = tmp_addr1 << 8 | tmp_addr2; 
    real_addr = read(tmp_addr) << 8 | read((tmp_addr+1));

    byte data = read(real_addr);
    return data;
}



/*

**********************************************
*     Addressing Mode Functions, get Address *
**********************************************
    
*/

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
}

// Branch if Result was zero
void BEQ() {
  if(getBit(&P,1) == 0x2) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
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
}

// Branch if Result was positive
void BPE() {
    if(getBit(&P,7) == 0x00) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
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
}

// Branch if Carry is 0
void BCC() {
    if(getBit(&P,0) == 0x00) 
    {   
        PC++;
        byte foo = read(PC);
        PC = PC + foo;
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
}