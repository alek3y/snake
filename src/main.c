#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>
#include "point.h"
#include "snake.h"
#include "apple.h"
#include "board.h"

#define SNAKE_HEAD '0'
#define SNAKE_BODY 'o'
#define SNAKE_BODY_EAT 'O'
#define APPLE '+'
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

	Board board = board_new(point_new(1, 1), point_new(width-2, height-2));

	Snake player = snake_new(point_new(width/2, height/2), SNAKE_HEAD, SNAKE_BODY);
	Point direction = point_new(-1, 0);

	Apple apple = apple_new(APPLE, APPLE_VALUE);
	apple_place(&apple, board, player);
	size_t apple_eaten = 0;

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
				if (direction.y != 1) {
					point_set(&direction, 0, -1);
				}
				break;

			case 's':
			case 'k':
			case KEY_DOWN:
				if (direction.y != -1) {
					point_set(&direction, 0, 1);
				}
				break;

			case 'd':
			case 'l':
			case KEY_RIGHT:
				if (direction.x != -1) {
					point_set(&direction, 1, 0);
				}
				break;

			case 'a':
			case 'j':
			case KEY_LEFT:
				if (direction.x != 1) {
					point_set(&direction, -1, 0);
				}
				break;

			case 'p':
				body_tail_show(player.head, SNAKE_BODY);
				break;

			case 'q':
				run = false;
				break;
		}

		snake_move_relative(player, board, direction);

		if (snake_is_dead(player)) {
			break;
		} else if (point_equals(player.head->position, apple.position)) {
			apple_eaten++;		// Start animation
		}

		if (apple_eaten > 0) {
			struct Body *body = player.head->next;		// Head shouldn't change
			bool is_body_eating;
			while (body != NULL) {
				is_body_eating = point_equals(body->position, apple.position);

				if (!body->hidden) {
					if (is_body_eating) {
						body->symbol = SNAKE_BODY_EAT;
					} else {
						body->symbol = SNAKE_BODY;
					}
				}

				body = body->next;
			}

			// Add a new body part when it reached the tail
			if (is_body_eating) {

				// TODO: Add apple to the snake as soon as it gets eaten on the head (and
				// check if the animation works with multiple apples)
				apple_place(&apple, board, player);
				apple_eaten--;

				for (size_t i = 0; i < APPLE_VALUE; i++) {
					body_tail_show(player.head, SNAKE_BODY);
				}
			}
		}
	}

	snake_destroy(player);
	endwin();
	exit_curses(0);
}
