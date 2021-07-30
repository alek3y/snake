#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <curses.h>
#include <stdbool.h>

#define SNAKE_HEAD '@'
#define SNAKE_BODY '#'
#define APPLE '*'
#define APPLE_VALUE 1

#define MOVEMENT_SLEEP 160

// NOTE: Boundaries are inclusive
long random_range(long min, long max) {
	return random() % (max - min + 1) + min;
}

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

Point point_sum(Point left, Point right) {
	return (Point) {left.x + right.x, left.y + right.y};
}

Point point_random(Point top_left, Point bottom_right) {
	return point_new(
		random_range(top_left.x, bottom_right.x),
		random_range(top_left.y, bottom_right.y)
	);
}

bool point_equals(Point from, Point to) {
	return from.x == to.x && from.y == to.y;
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
	return body;
}

void body_tail_show(struct Body *body, char symbol) {
	while (body->next != NULL) {
		body = body->next;
	}
	body->symbol = symbol;		// Ensure symbol is the one requested
	body->hidden = false;
	body->next = body_new(0, point_new(0, 0), true);		// Make new tail placeholder
}

typedef struct {
	struct Body *head;
} Snake;

Snake snake_new(Point position, char head_symbol, char body_symbol) {
	struct Body *head = body_new(head_symbol, position, false);
	body_tail_show(head, head_symbol);
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

// NOTE: Top-left is (0, 0)
void snake_move_relative(Snake snake, Point direction) {
	snake_move(snake, point_sum(snake.head->position, direction));
}

void snake_draw(Snake snake) {
	struct Body *body = snake.head;
	while (body != NULL) {
		if (!body->hidden) {
			Point position = body->position;
			move(position.y, position.x);
			addch(body->symbol);
		}

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

bool snake_is(Snake snake, Point position) {
	struct Body *body = snake.head;
	while (body != NULL) {
		if (point_equals(position, body->position)) {
			return true;
		}

		body = body->next;
	}
	return false;
}

typedef struct {
	Point position;
	char symbol;
	int value;
} Apple;

Apple apple_new(char symbol, int value) {
	return (Apple) {point_new(0, 0), symbol, value};
}

void apple_place(Apple *apple, Snake snake, Point top_left, Point bottom_right) {
	Point found = point_new(0, 0);
	do {
		found = point_random(top_left, bottom_right);
	} while (snake_is(snake, found));
	apple->position = found;
}

void apple_draw(Apple apple) {
	move(apple.position.y, apple.position.x);
	addch(apple.symbol);
}

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

	while (true) {
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
		}

		snake_move_relative(player, direction);
	}

	snake_destroy(player);
	endwin();
	exit_curses(0);
}
