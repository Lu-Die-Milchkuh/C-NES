CC = gcc
CFLAGS = -std=c11 -pedantic -Wall -Wextra -O2 -lSDL2 -lvulkan -I include/
SRC_DIR = src
OBJ_DIR = obj
TARGET = Nessi
TARGET_DIR = bin

SRC := $(wildcard $(SRC_DIR)/*.c)
OBJS := $(patsubst $(SRC_DIR)/%.c, $(OBJ_DIR)/%.o, $(SRC))


all:	dir $(OBJS) $(TARGET)


dir:
	mkdir -p $(OBJ_DIR)
	mkdir -p $(TARGET_DIR)


$(OBJS): $(OBJ_DIR)/%.o : $(SRC_DIR)/%.c
	$(CC) $(CFLAGS) -c  $< -o $@


$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(OBJS) -o $(TARGET_DIR)/$(TARGET)

clean:
	rm -rf $(OBJ_DIR)
