#include <stdbool.h>
#include "util.h"

#pragma once

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

bool point_equals(Point from, Point to) {
	return from.x == to.x && from.y == to.y;
}
