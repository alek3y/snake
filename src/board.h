#include "point.h"
#include "util.h"

#pragma once

typedef struct {
	Point top_left, bottom_right;
} Board;

Board board_new(Point top_left, Point bottom_right) {
	return (Board) {top_left, bottom_right};
}

Point board_random_position(Board board) {
	return point_new(
		random_range(board.top_left.x, board.bottom_right.x),
		random_range(board.top_left.y, board.bottom_right.y)
	);
}
