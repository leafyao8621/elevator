CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = src/test.c $(wildcard src/*/*.c) $(wildcard src/*/*/*.c) $(wildcard src/*/*/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = test

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN) -lm

.PHONY: clean valgrind gdb run
clean:
	@rm $(OBJ)
	@rm $(BIN)

valgrind: $(BIN)
	@valgrind --leak-check=full -s ./$(BIN)

gdb: $(BIN)
	@gdb ./$(BIN)

run: $(BIN)
	@./$(BIN)
