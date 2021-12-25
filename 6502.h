
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


/*
    Instructions:
*/

//add with carry
void ADC(byte data);

//and (with accumulator)
void AND(byte data);

//arithmetic shift left
void ASL();

//branch on carry clear
void BCC();

//branch on carry set
void BCS();

//branch on equal (zero set)
void BEQ();

//bit test
void BIT();

//branch on minus (negative set)
void BMI();

//branch on not equal (zero clear)
void BNE();

//branch on plus (negative clear)
void BPL();

//break / interrupt
void BRK();

//branch on overflow clear
void BVC();

//branch on overflow set
void BVS();

//clear carry
void CLC();

//clear decimal
void CLD();

//clear interrupt disable
void CLI();

//clear overflow
void CLV();

//compare (with accumulator)
void CMP();

//compare with X
void CPX();

//compare with Y
void CPY();

//decrement
void DEC();

//decrement X
void DEX();

//decrement Y
void DEY();

//exclusive or (with accumulator)
void EOR(byte data);

//increment
void INC(word address);

//increment X
void INX();

//increment Y
void INY();

//jump
void JMP(word address);

//jump subroutine
void JSR();

//load accumulator
void LDA(byte data);

//load X
void LDX(byte data);

//load Y
void LDY(byte data);

//logical shift right
void LSR();

//no operation
void NOP();

//or with accumulator
void ORA(byte data);

//push accumulator
void PHA();

//push processor status (SR)
void PHP();

//pull accumulator
void PLA();

//pull processor status (SR)
void PLP();

//rotate left
void ROL(byte* data);

//rotate right
void ROR();

//return from interrupt
void RTI();

//return from subroutine
void RTS();

//subtract with carry
void SBC(byte data);

//set carry
void SEC();

//set decimal
void SED();

//set interrupt disable
void SEI();

//store accumulator
void STA(word address);

//store X
void STX(word address);

//store Y
void STY(word address);

//transfer accumulator to X
void TAX();

//transfer accumulator to Y
void TAY();

//transfer stack pointer to X
void TSX();

//transfer X to accumulator
void TXA();

//transfer X to stack pointer
void TXS();

//transfer Y to accumulator
void TYA();


// Addressing Modes
byte getImmediate();    // Get next Byte(Data) from Memory
byte getAbsolute();     // Get Data stored at absolute Address in Memory
byte getAbsoluteX();    // Get Data stored at absolute Address + X(offset)  in Memory
byte getAbsoluteY();    // Get Data stored at absolute Address + Y(offset)  in Memory
byte getZPG();          // Get Data from Zero Page of Memory, first 256 Bytes of Memory
byte getZPGX();         // Get Data from Zero Page + X (offset) of Memory
byte getIndirect();
byte getIndirectX();
byte getIndirectY();


