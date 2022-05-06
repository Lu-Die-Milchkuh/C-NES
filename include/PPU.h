#ifndef _TYPES
#include "Types.h"
#endif

//The pattern table is divided into two 256-tile sections: $0000-$0FFF, nicknamed "left", and $1000-$1FFF, nicknamed "right".
extern u8 patternTable[2][4096];
extern u8 nameTable[2][1024];

// PPU Register Address
#define PPUCTRL_ADDRESS 0x0000
#define PPUMASK_ADDRESS 0x0001  
#define PPUSTATUS_ADDRESS 0x0002 
#define OAMADDR_ADDRESS 0x0003   
#define OAMDATA_ADDRESS 0x0004 	
#define PPUSCROLL_ADDRESS 0x0005  
#define PPUADDR_ADDRESS 0x0006    
#define PPUDATA_ADDRESS 0x0007


typedef union 
	{
		struct
		{
			u8 unused:5;
			u8 sprite_overflow:1;
			u8 sprite_zero_hit:1;
			u8 vertical_blank:1;
		};

		u8 reg;
	} PPUSTATUS;


typedef union 
	{
		struct
		{
			u8 grayscale:1;
			u8 render_background_left:1;
			u8 render_sprites_left:1;
			u8 render_background:1;
			u8 render_sprites:1;
			u8 enhance_red:1;
			u8 enhance_green:1;
			u8 enhance_blue:1;
		};

		u8 reg;
	} PPUMASK;


typedef union 
	{
		struct
		{
			u8 nametable_x:1;
			u8 nametable_y:1;
			u8 increment_mode:1;
			u8 pattern_sprite:1;
			u8 pattern_background:1;
			u8 sprite_size:1;
			u8 slave_mode:1; // unused
			u8 enable_nmi:1;
		};

		u8 reg;
	} PPUCTRL;


typedef union 
	{
		
		struct
		{

			u16 coarse_x:5;
			u16 coarse_y:5;
			u16 nametable_x:1;
			u16 nametable_y:1;
			u16 fine_y:3;
			u16 unused:1;
		};

		u16 reg;
	} loopy_register;



// Functions to access PPU from CPU
u8 PPU_CPU_READ(u16 address);
void PPU_CPU_WRITE(u8 data,u16 address);


// Internal PPU read/write function
u8 PPU_READ(u16 addresss);
void PPU_WRITE(u8 data,u16 address);


void PPU_RUN();