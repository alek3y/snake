CC := gcc

all:
	$(CC) main.c -lncurses -o main

run: all
	./main
