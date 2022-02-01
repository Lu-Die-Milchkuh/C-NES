typedef unsigned char byte;
typedef unsigned short word;

// Pointer containing content of ROM
extern byte* cartridge_pgr_memory;
extern byte* cartridge_chr_memory;


void loadROM(char* path);
void unloadROM();