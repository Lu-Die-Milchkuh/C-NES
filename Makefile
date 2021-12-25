CC = gcc
CFLAGS = -Wall -O2 
FILES = *.c
OUT_FILE = CNES

default: compile 

compile:
	mkdir build
	$(CC) $(CFLAGS) $(FILES) -o build/$(OUT_FILE)