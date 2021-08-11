CC := gcc
OUTDIR := build
INSTALLDIR := /usr/local/bin

all:
	mkdir -p $(OUTDIR)
	$(CC) src/main.c -Iinclude -lncurses -o $(OUTDIR)/main

run: all
	./$(OUTDIR)/main

install: all
	cp $(OUTDIR)/main $(INSTALLDIR)/snake

uninstall:
	rm $(INSTALLDIR)/snake

clean:
	rm -r $(OUTDIR)
