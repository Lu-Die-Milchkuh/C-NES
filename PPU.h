#ifndef _TYPES
#include "Types.h"
#endif

//The pattern table is divided into two 256-tile sections: $0000-$0FFF, nicknamed "left", and $1000-$1FFF, nicknamed "right".
extern byte patternTable[2][4096];
extern byte nameTable[2][1024];

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
			byte unused:5;
			byte sprite_overflow:1;
			byte sprite_zero_hit:1;
			byte vertical_blank:1;
		};

		byte reg;
	} PPUSTATUS;


typedef union 
	{
		struct
		{
			byte grayscale:1;
			byte render_background_left:1;
			byte render_sprites_left:1;
			byte render_background:1;
			byte render_sprites:1;
			byte enhance_red:1;
			byte enhance_green:1;
			byte enhance_blue:1;
		};

		byte reg;
	} PPUMASK;


typedef union 
	{
		struct
		{
			byte nametable_x:1;
			byte nametable_y:1;
			byte increment_mode:1;
			byte pattern_sprite:1;
			byte pattern_background:1;
			byte sprite_size:1;
			byte slave_mode:1; // unused
			byte enable_nmi:1;
		};

		byte reg;
	} PPUCTRL;


typedef union 
	{
		
		struct
		{

			word coarse_x:5;
			word coarse_y:5;
			word nametable_x:1;
			word nametable_y:1;
			word fine_y:3;
			word unused:1;
		};

		word reg;
	} loopy_register;



// Functions to access PPU from CPU
byte PPU_CPU_READ(word address);
void PPU_CPU_WRITE(byte data,word address);


// Internal PPU read/write function
byte PPU_READ(word addresss);
void PPU_WRITE(byte data,word address);


void PPU_RUN();