CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC = k_means
THREADS = 18

CFLAGS = -std=c99 -O2 -Wall -g -fno-omit-frame-pointer -funroll-loops -fopenmp

.DEFAULT_GOAL = k_means

k_means:
	$(CC) $(CFLAGS) $(SRC)$(EXEC).c -o $(BIN)$(EXEC)

clean:
	rm -r bin/*

runseq:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS)
runpar:
	./$(BIN)$(EXEC) 10000000 $(CP_CLUSTERS) $(THREADS)
