#ifndef byte
typedef unsigned char byte;     // 8-Bit
#endif

#ifndef word
typedef unsigned short word;    //16-Bit   
#endif

// Set Bit @ Index, set to 1
void setBit(byte* reg,byte index);

// Clear Bit @ Index, set to 0
void clearBit(byte* reg,byte index);

// Returns Bit @ given Index
byte getBit(byte* reg,byte index);

// Get Binary Presentation of a given value,size used for bit count
void bin(unsigned value,int size);