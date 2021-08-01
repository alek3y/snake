#include <stdlib.h>
#include "point.h"
#include "list.h"

#pragma once

// NOTE: Boundaries are inclusive
long random_range(long min, long max) {
	return random() % (max - min + 1) + min;
}

/* Board */

typedef struct {
	Point top_left, bottom_right;
} Board;

Board board_new(Point, Point);
Point board_random_position(Board);
bool board_is_inside(Board, Point);

/* Snake */

struct Body {
	Point position;
	char symbol;
	bool hidden;
};

struct Body *body_new(char, Point, bool);

typedef struct {
	List body;
} Snake;

void snake_tail_show(Snake *, char);
Snake snake_new(Point, char, char);
Point snake_position(Snake);
void snake_move(Snake, Point);
void snake_move_relative(Snake, Board, Point);
void snake_draw(Snake);
void snake_destroy(Snake *);
bool snake_is(Snake, Point);
bool snake_is_dead(Snake);

/* Apple */

typedef struct {
	Point position;
	char symbol;
	int value;
} Apple;

Apple apple_new(char, int);
void apple_place(Apple *, Board, Snake);
void apple_draw(Apple);

/* Function definitions */

#include "board.h"
#include "snake.h"
#include "apple.h"
