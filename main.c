#include <stdio.h>
#include <curses.h>

typedef struct {
	int x, y;
} Point;

struct Body {
	Point position;
	struct Body *next;
};

typedef struct {
	struct Body *head;
} Snake;

int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);

	endwin();
}
