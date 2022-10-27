CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC = k_means
EXEC_NO_SQUARE_ROOT = k_means_no_square_root
EXEC_NEW_DS = k_means_new_ds
EXEC_MERGE = k_means_merge

# TODO: vectorize? loop unroll»?
CFLAGS = -std=c99 -O2 -funroll-all-loops

.DEFAULT_GOAL = k_means

k_means: $(SRC)k_means.c
	$(CC) $(CFLAGS) $(SRC)k_means.c -o $(BIN)$(EXEC) -lm
	$(CC) $(CFLAGS) $(SRC)k_means_no_square_root.c -o $(BIN)$(EXEC_NO_SQUARE_ROOT)
	$(CC) $(CFLAGS) $(SRC)k_means_new_ds.c -o $(BIN)$(EXEC_NEW_DS) -lm
	$(CC) $(CFLAGS) $(SRC)k_means_merge.c -o $(BIN)$(EXEC_MERGE)

clean:
	rm -r bin/*

run:
	./$(BIN)$(EXEC)

run_no_square_root:
	./$(BIN)$(EXEC_NO_SQUARE_ROOT)

run_new_ds:
	./$(BIN)$(EXEC_NEW_DS)

run_merge:
	./$(BIN)$(EXEC_MERGE)
