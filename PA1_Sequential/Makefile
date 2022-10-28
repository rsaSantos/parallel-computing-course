CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC = k_means
EXEC_NO_SQUARE_ROOT = k_means_no_square_root
EXEC_NEW_DS = k_means_optimized

CFLAGS = -std=c99 -O2 -Wall -funroll-loops

.DEFAULT_GOAL = k_means

k_means: $(SRC)0_k_means.c
	$(CC) $(CFLAGS) $(SRC)0_k_means.c -o $(BIN)$(EXEC) -lm
	$(CC) $(CFLAGS) $(SRC)1_k_means_no_square_root.c -o $(BIN)$(EXEC_NO_SQUARE_ROOT)
	$(CC) $(CFLAGS) $(SRC)2_k_means_optimized.c -o $(BIN)$(EXEC_NEW_DS) -lm

clean:
	rm -r bin/*

run:
	./$(BIN)$(EXEC_NEW_DS)

run_base_version:
	./$(BIN)$(EXEC)

run_no_square_root:
	./$(BIN)$(EXEC_NO_SQUARE_ROOT)
