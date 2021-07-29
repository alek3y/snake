#include <stdio.h>
#include <stdlib.h>
#include <curses.h>
#include <stdbool.h>

#define SNAKE_HEAD '@'
#define SNAKE_BODY '#'

#define MOVEMENT_SLEEP 500

typedef struct {
	int x, y;
} Point;

Point point_new(int x, int y) {
	return (Point) {x, y};
}

void point_set(Point *point, int x, int y) {
	point->x = x;
	point->y = y;
}

struct Body {
	Point position;
	char symbol;
	bool hidden;
	struct Body *next;
};

struct Body *body_new(char symbol, Point position, bool hidden) {
	struct Body *body = calloc(1, sizeof(*body));
	body->position = position;
	body->symbol = symbol;
	body->hidden = hidden;
}

void body_add_tail(struct Body *body, char symbol) {
	while (body->next != NULL) {
		body = body->next;
	}
	body->hidden = false;
	body->next = body_new(symbol, point_new(0, 0), true);		// Make new tail placeholder
}

typedef struct {
	struct Body *head;
} Snake;

Snake snake_new(Point position, char head_symbol, char body_symbol) {
	struct Body *head = body_new(head_symbol, position, false);
	body_add_tail(head, body_symbol);
	return (Snake) {head};
}

void snake_move(Snake snake, Point new_position) {
	struct Body *body = snake.head;
	Point previous_body_position = body->position;
	body->position = new_position;

	while (body->next != NULL) {
		body = body->next;
		Point current_new_position = previous_body_position;
		previous_body_position = body->position;
		body->position = current_new_position;
	}
}

void snake_draw(Snake snake) {
	struct Body *body = snake.head;
	while (body->next != NULL) {
		if (body->hidden) {
			continue;
		}

		Point position = body->position;
		move(position.y, position.x);
		addch(body->symbol);

		body = body->next;
	}
}

void snake_destroy(Snake snake) {
	struct Body *previous_body = snake.head, *body = previous_body->next;
	while (body != NULL) {
		free(previous_body);
		previous_body = body;
		body = body->next;
	}
	free(previous_body);
}

int main() {
	initscr();
	cbreak();
	noecho();
	keypad(stdscr, TRUE);
	timeout(MOVEMENT_SLEEP);
	curs_set(0);

	int width, height;
	getmaxyx(stdscr, height, width);

	Snake player = snake_new(point_new(width/2, height/2), SNAKE_HEAD, SNAKE_BODY);
	while (true) {
		erase();
		box(stdscr, 0, 0);
		snake_draw(player);
		refresh();
	}

	snake_destroy(player);
	endwin();
	exit_curses(0);
}
