#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>
#include "game/game.h"

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

	board_apple_place(&board, apple_new(APPLE, APPLE_VALUE), player);

	bool game_quit = false;
	while (!game_quit) {
		erase();
		box(stdscr, 0, 0);
		board_apples_draw(board);
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
				snake_tail_show(&player, SNAKE_BODY, point_new(0, 0));
				break;

			case 'q':
				game_quit = true;
				break;
		}

		snake_move_relative(player, board, direction);

		if (snake_is_dead(player)) {
			break;
		}

		// Handle interaction with apples
		struct Node *player_head = list_get_node(player.body, 0), *player_node = player_head;
		while (player_node != NULL) {
			struct Body *body = player_node->content;

			size_t apple_number = board_apple_find(board, body->position);
			if (apple_number != -1) {

				// Add a new apple when the snake eats it
				if (player_node == player_head) {
					board_apple_place(&board, apple_new(APPLE, APPLE_VALUE), player);

				// Add a new body part when the apple reaches the tail
				} else if (player_node->next == NULL) {
					Apple *apple = list_get(board.apples, apple_number);
					for (size_t i = 0; i < apple->value; i++) {
						snake_tail_show(&player, SNAKE_BODY, body->position);
					}

					free(list_remove(&board.apples, apple_number));

				// Otherwise animate the body
				} else {
					body->symbol = SNAKE_BODY_EAT;
				}
			} else {
				if (player_node != player_head && !body->hidden) {
					body->symbol = SNAKE_BODY;
				}
			}

			player_node = player_node->next;
		}
	}

	snake_destroy(&player);
	board_destroy(&board);
	endwin();
	exit_curses(0);
}
