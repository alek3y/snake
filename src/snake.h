#include <stdlib.h>
#include "point.h"
#include "board.h"

#pragma once

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
void snake_move_relative(Snake snake, Board board, Point direction) {
	Point target = point_sum(snake.head->position, direction);

	// Teleport when going out the frame
	if (!board_is_inside(board, target)) {
		switch (direction.x) {
			case 1:
				target.x = board.top_left.x;
				break;
			case -1:
				target.x = board.bottom_right.x;
				break;
			default:
				switch (direction.y) {
					case 1:
						target.y = board.top_left.y;
						break;
					case -1:
						target.y = board.bottom_right.y;
						break;
				}
				break;
		}
	}

	snake_move(snake, target);
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

bool snake_is_dead(Snake snake) {
	Point head = snake.head->position;
	struct Body *body = snake.head->next;
	while (body != NULL) {
		if (!body->hidden && point_equals(body->position, head)) {
			return true;
		}

		body = body->next;
	}
	return false;
}
