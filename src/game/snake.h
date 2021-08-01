struct Body *body_new(char symbol, Point position, bool hidden) {
	struct Body *body = calloc(1, sizeof(*body));
	body->position = position;
	body->symbol = symbol;
	body->hidden = hidden;
	return body;
}

void snake_tail_show(Snake *snake, char symbol) {
	struct Body *last = list_get(snake->body, snake->body.size-1);
	last->symbol = symbol;
	last->hidden = false;
	list_push(&snake->body, body_new(0, point_new(0, 0), true));		// Make new tail placeholder
}

Snake snake_new(Point position, char head_symbol, char body_symbol) {
	Snake snake = {list_new()};
	list_push(&snake.body, body_new(head_symbol, position, false));
	snake_tail_show(&snake, head_symbol);
	return snake;
}

Point snake_position(Snake snake) {
	return ((struct Body *) list_get(snake.body, 0))->position;
}

void snake_move(Snake snake, Point new_position) {
	struct Node *node = list_get_node(snake.body, 0);

	struct Body *body = node->content;
	Point previous_position = body->position;
	body->position = new_position;

	while (node->next != NULL) {
		node = node->next;
		body = node->content;
		Point current_new_position = previous_position;
		previous_position = body->position;
		body->position = current_new_position;
	}
}

// NOTE: Top-left is (0, 0)
void snake_move_relative(Snake snake, Board board, Point direction) {
	Point target = point_sum(snake_position(snake), direction);

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
	struct Node *node = list_get_node(snake.body, 0);
	while (node != NULL) {
		struct Body *body = node->content;
		if (!body->hidden) {
			Point position = body->position;
			move(position.y, position.x);
			addch(body->symbol);
		}

		node = node->next;
	}
}

void snake_destroy(Snake *snake) {
	list_free_contents(snake->body);
	list_destroy(&snake->body);
}

bool snake_is(Snake snake, Point position) {
	struct Node *node = list_get_node(snake.body, 0);
	while (node != NULL) {
		struct Body *body = node->content;
		if (point_equals(position, body->position)) {
			return true;
		}

		node = node->next;
	}
	return false;
}

bool snake_is_dead(Snake snake) {
	Point head = snake_position(snake);
	struct Node *node = list_get_node(snake.body, 1);
	while (node != NULL) {
		struct Body *body = node->content;
		if (!body->hidden && point_equals(body->position, head)) {
			return true;
		}

		node = node->next;
	}
	return false;
}
