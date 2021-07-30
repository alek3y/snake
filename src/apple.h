#include "point.h"
#include "snake.h"

#pragma once

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
