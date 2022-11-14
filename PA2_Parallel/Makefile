CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC = k_means
THREADS = 4
CFLAGS = -std=c99 -O2 -Wall -funroll-loops -fopenmp

include .env

k_means:
	$(CC) $(CFLAGS) $(SRC)k_means_old.c -o $(BIN)$(EXEC)_old
	$(CC) $(CFLAGS) $(SRC)k_means_new.c -o $(BIN)$(EXEC)_new

clean:
	rm -r bin/*

runpar_old:
	./$(BIN)$(EXEC)_old 10000000 $(CP_CLUSTERS) $(THREADS)

runpar_new:
	./$(BIN)$(EXEC)_new 10000000 $(CP_CLUSTERS) $(THREADS)

