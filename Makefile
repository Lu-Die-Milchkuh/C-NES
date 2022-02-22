CC = gcc
CFLAGS = -Wall -O2 -lSDL2
FILES = 6502.c Bus.c HelperFunc.c PPU.c Mapper.c Cartridge.c
OUT_FILE = Nessi.elf

default: dir compile 

dir:
	mkdir -p build

compile:
	$(CC) $(CFLAGS) $(FILES) -o build/$(OUT_FILE)