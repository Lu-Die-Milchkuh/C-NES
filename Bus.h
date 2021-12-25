#ifndef byte
typedef unsigned char byte;
#endif

#ifndef word
typedef unsigned short word;
#endif

// Write Data to Address in memory
void write(byte data,word address);

// Read data @ given address
byte read(word address);