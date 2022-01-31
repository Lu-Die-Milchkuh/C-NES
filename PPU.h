typedef unsigned char byte; // 8-Bit
typedef unsigned short word; // 16-Bit


extern byte ppu_memory[8192];

// PPU Register
#define PPUCTRL  ppu_memory[0]   // Sets Control Bits for PPU
#define PPUMASK  ppu_memory[1]   // Controls draw order,switches
#define PPUSTATUS ppu_memory[2]
#define OAMADDR ppu_memory[3]   // Sets OAM starting address
#define OAMDATA ppu_memory[4]	// Writes data to OAM
#define PPUSCROLL ppu_memory[5] // Controls screen scrolling
#define PPUADDR ppu_memory[6]   // Sets address where PPUDATA shall write to
#define PPUDATA ppu_memory[7]   // Writes to VRAM
//extern byte* OAMDMA;  // Transfers 256 bytes to OAM from offset value


// PPU Register Address
#define PPUCTRL_ADDRESS 0x2000
#define PPUMASK_ADDRESS 0x2001  
#define PPUSTATUS_ADDRESS 0x2002 
#define OAMADDR_ADDRESS 0x2003   
#define OAMDATA_ADDRESS 0x2004 	
#define PPUSCROLL_ADDRESS 0x2005  
#define PPUADDR_ADDRESS 0x2006    
#define PPUDATA_ADDRESS 0x2007