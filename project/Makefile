CC = gcc
CFLAGS = -Wall -Wextra -Iincludes

SRC_DIR = src
OBJ_DIR = obj
BIN_DIR = bin

SOURCES = $(wildcard $(SRC_DIR)/*.c)
OBJECTS = $(patsubst $(SRC_DIR)/%.c,$(OBJ_DIR)/%.o,$(SOURCES))

TARGET = $(BIN_DIR)/green_app

all: $(TARGET)

$(TARGET): $(OBJECTS)
<TAB>$(CC) $(CFLAGS) -o $@ $^

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c
<TAB>mkdir -p $(OBJ_DIR)
<TAB>$(CC) $(CFLAGS) -c $< -o $@

clean:
<TAB>rm -rf $(OBJ_DIR) $(TARGET)

.PHONY: all clean
