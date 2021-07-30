#include "point.h"
#include "snake.h"
#include "board.h"

#pragma once

typedef struct {
	Point position;
	char symbol;
	int value;
} Apple;

Apple apple_new(char symbol, int value) {
	return (Apple) {point_new(0, 0), symbol, value};
}

void apple_place(Apple *apple, Board board, Snake snake) {
	Point found = point_new(0, 0);
	do {
		found = board_random_position(board);
	} while (snake_is(snake, found));
	apple->position = found;
}

void apple_draw(Apple apple) {
	move(apple.position.y, apple.position.x);
	addch(apple.symbol);
}
