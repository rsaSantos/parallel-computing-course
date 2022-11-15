CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC = k_means
THREADS = 4 # OPTIMAL VALUE?

CFLAGS = -std=c99 -O2 -Wall -funroll-loops -fopenmp # LOOP UNROLL FLAG?

.DEFAULT_GOAL = k_means

CP_CLUSTERS = 4 # REMOVE

k_means:
	$(CC) $(CFLAGS) $(SRC)$(EXEC).c -o $(BIN)$(EXEC)

clean:
	rm -r bin/*

runseq:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS)
runpar:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS) $(THREADS)
