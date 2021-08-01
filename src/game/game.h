#include <stdlib.h>
#include "point.h"
#include "list.h"

#pragma once

// NOTE: Boundaries are inclusive
long random_range(long min, long max) {
	return random() % (max - min + 1) + min;
}

/* Types */

// Board

typedef struct {
	Point position;
	char symbol;
	int value;
} Apple;

typedef struct {
	Point top_left, bottom_right;
	List apples;
} Board;

// Snake

struct Body {
	Point position;
	char symbol;
	bool hidden;
};

typedef struct {
	List body;
} Snake;

/* Prototypes */

// Board

Apple *apple_new(char, int);
Board board_new(Point, Point);
Point board_random_position(Board);
bool board_is_inside(Board, Point);
size_t board_apple_find(Board, Point);
bool board_is_apple(Board, Point);
void board_apple_place(Board *, Apple *, Snake);
void board_apples_draw(Board);

// Snake

struct Body *body_new(char, Point, bool);
void snake_tail_show(Snake *, char, Point);
Snake snake_new(Point, char, char);
Point snake_position(Snake);
void snake_move(Snake, Point);
void snake_move_relative(Snake, Board, Point);
void snake_draw(Snake);
void snake_destroy(Snake *);
bool snake_is(Snake, Point);
bool snake_is_dead(Snake);

/* Functions */

#include "board.h"
#include "snake.h"
