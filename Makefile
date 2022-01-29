CC = gcc
CFLAGS = -Wall -O2 -lSDL2
FILES = 6502.c Bus.c HelperFunc.c PPU.c
OUT_FILE = CNES

default: dir compile 

dir:
	mkdir -p build

compile:
	$(CC) $(CFLAGS) $(FILES) -o build/$(OUT_FILE)