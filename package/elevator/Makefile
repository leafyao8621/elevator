CC = gcc
CFLAGS = -Wall -Wextra -Werror -pedantic
SRC = src/test.c $(wildcard src/*/*.c)
OBJ = $(SRC:.c=.o)
BIN = test

%.o: %.c
	$(CC) $(CFLAGS) -g -c $< -o $@

$(BIN): $(OBJ)
	$(CC) $(OBJ) -o $(BIN)

.PHONY: clean
clean:
	@rm $(OBJ)
	@rm $(BIN)
