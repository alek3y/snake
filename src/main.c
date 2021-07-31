#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>
#include "point.h"
#include "list.h"
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

	bool game_quit = false;
	while (!game_quit) {
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
				snake_tail_show(&player, SNAKE_BODY);
				break;

			case 'q':
				game_quit = true;
				break;
		}

		snake_move_relative(player, board, direction);

		if (snake_is_dead(player)) {
			break;
		} else if (point_equals(snake_position(player), apple.position)) {
			apple_eaten++;		// Start animation
		}

		if (apple_eaten > 0) {
			struct Node *node = list_get_node(player.body, 1);		// Head shouldn't change

			bool is_body_eating;
			while (node != NULL) {
				struct Body *body = node->content;
				is_body_eating = point_equals(body->position, apple.position);

				if (!body->hidden) {
					if (is_body_eating) {
						body->symbol = SNAKE_BODY_EAT;
					} else {
						body->symbol = SNAKE_BODY;
					}
				}

				node = node->next;
			}

			// Add a new body part when it reached the tail
			if (is_body_eating) {

				// TODO: Add apple to the snake as soon as it gets eaten on the head (and
				// check if the animation works with multiple apples)
				apple_place(&apple, board, player);
				apple_eaten--;

				for (size_t i = 0; i < APPLE_VALUE; i++) {
					snake_tail_show(&player, SNAKE_BODY);
				}
			}
		}
	}

	snake_destroy(&player);
	endwin();
	exit_curses(0);
}
