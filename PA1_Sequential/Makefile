CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC = k_means
EXEC_NO_SQUARE_ROOT = k_means_no_square_root

CFLAGS = -std=c99 -O2

.DEFAULT_GOAL = k_means

k_means: $(SRC)k_means.c
	$(CC) $(CFLAGS) $(SRC)k_means.c -o $(BIN)$(EXEC) -lm
	$(CC) $(CFLAGS) $(SRC)k_means_no_square_root.c -o $(BIN)$(EXEC_NO_SQUARE_ROOT)

clean:
	rm -r bin/*

run:
	./$(BIN)$(EXEC)

run_no_square_root:
	./$(BIN)$(EXEC_NO_SQUARE_ROOT)
