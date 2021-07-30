CC := gcc
OUTDIR := target

all:
	mkdir -p $(OUTDIR)
	$(CC) src/main.c -lncurses -o $(OUTDIR)/main

run: all
	./$(OUTDIR)/main
