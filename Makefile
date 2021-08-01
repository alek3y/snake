CC := gcc
OUTDIR := build

all:
	mkdir -p $(OUTDIR)
	$(CC) src/main.c -Iinclude -lncurses -o $(OUTDIR)/main

run: all
	./$(OUTDIR)/main

clean:
	rm -r $(OUTDIR)
