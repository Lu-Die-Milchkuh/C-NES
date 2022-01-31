typedef unsigned char byte;
typedef unsigned short word;

extern byte* cartridge_memory;


void loadROM(char* path);
void unloadROM();