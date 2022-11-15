CC = gcc
BIN = bin/
SRC = src/
INCLUDES = include/
EXEC = k_means
THREADS = 4
CFLAGS = -std=c99 -O2 -Wall -funroll-loops -fopenmp
SHELL:=/bin/bash

# REMOVE
CP_CLUSTERS = 4

k_means:
	$(CC) $(CFLAGS) $(SRC)k_means_old.c -o $(BIN)$(EXEC)_old
	$(CC) $(CFLAGS) $(SRC)k_means_new.c -o $(BIN)$(EXEC)_new
	$(CC) $(CFLAGS) $(SRC)k_means_redu_oneliner.c -o $(BIN)$(EXEC)_redu_oneliner

clean:
	rm -r bin/*

runpar_old:
	./$(BIN)$(EXEC)_old 10000000 $(CP_CLUSTERS) $(THREADS)

runpar_new:
	./$(BIN)$(EXEC)_new 10000000 $(CP_CLUSTERS) $(THREADS)

runpar_redu_oneliner:
	./$(BIN)$(EXEC)_redu_oneliner 10000000 $(CP_CLUSTERS) $(THREADS)
