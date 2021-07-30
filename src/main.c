#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>
#include "point.h"
#include "snake.h"
#include "apple.h"

#define SNAKE_HEAD '@'
#define SNAKE_BODY '#'
#define APPLE '*'
#define APPLE_VALUE 1

#define MOVEMENT_SLEEP 160

int main() {
	srand(time(NULL));

	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	timeout(MOVEMENT_SLEEP);
	curs_set(0);

	// NOTE: Resizing the window while playing will break the game
	int width, height;
	getmaxyx(stdscr, height, width);

	Snake player = snake_new(point_new(width/2, height/2), SNAKE_HEAD, SNAKE_BODY);
	Point direction = point_new(-1, 0);
	Apple apple = apple_new(APPLE, APPLE_VALUE);
	apple_place(&apple, player, point_new(1, 1), point_new(width-2, height-2));

	bool run = true;
	while (run) {
		erase();
		box(stdscr, 0, 0);
		apple_draw(apple);
		snake_draw(player);
		refresh();

		int input = getch();
		switch (input) {
			case 'w':
			case 'i':
			case KEY_UP:
				point_set(&direction, 0, -1);
				break;

			case 's':
			case 'k':
			case KEY_DOWN:
				point_set(&direction, 0, 1);
				break;

			case 'd':
			case 'l':
			case KEY_RIGHT:
				point_set(&direction, 1, 0);
				break;

			case 'a':
			case 'j':
			case KEY_LEFT:
				point_set(&direction, -1, 0);
				break;

			case 'p':
				body_tail_show(player.head, SNAKE_BODY);
				break;

			case 'q':
				run = false;
				break;
		}

		snake_move_relative(player, direction);
	}

	snake_destroy(player);
	endwin();
	exit_curses(0);
}
