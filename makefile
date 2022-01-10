CC=gcc
FLAGS=$(CFLAGS) -g -Wall -Wpedantic -std=gnu99 -ljemalloc -lm
SRC=src/player.c src/main.c
EXEC=pairings

build:
	$(CC) $(FLAGS) $(SRC) -o $(EXEC)

format:
	astyle src/* --style=kr

