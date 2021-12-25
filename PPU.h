#ifndef byte
typedef unsigned char byte; // 8-Bit
#endif

#ifndef word
typedef unsigned short word; // 16-Bit
#endif

extern byte ppu_memory[8192];

// PPU Register
extern byte* PPUCTRL;   // Sets Control Bits for PPU
extern byte* PPUMASK;   // Controls draw order,switches
extern byte* PPUSTATUS;
extern byte* OAMADDR;   // Sets OAM starting address
extern byte* OAMDATA;	// Writes data to OAM
extern byte* PPUSCROLL; // Controls screen scrolling
extern byte* PPUADDR;   // Sets address where PPUDATA shall write to
extern byte* PPUDATA;   // Writes to VRAM
//extern byte* OAMDMA;  // Transfers 256 bytes to OAM from offset value


