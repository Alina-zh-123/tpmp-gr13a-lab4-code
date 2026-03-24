CC = gcc
LIBS = -lsqlite3
CFLAGS = -Wall -Wextra -std=c11 -MMD -MP -Iproject/includes

TARGET = project/bin/green_app
SRC = project/src/main.c project/src/database.c
OBJ = $(SRC:.c=.o)
DEPS = $(OBJ:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CC) $(OBJ) -o $@ $(LIBS)

project/src/%.o: project/src/%.c
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	rm -f $(TARGET) $(OBJ) $(DEPS)

-include $(DEPS)
