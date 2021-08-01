Apple *apple_new(char symbol, int value) {
	Apple *apple = malloc(sizeof(*apple));
	*apple = (Apple) {point_new(0, 0), symbol, value};
	return apple;
}

Board board_new(Point top_left, Point bottom_right) {
	return (Board) {top_left, bottom_right, list_new()};
}

Point board_random_position(Board board) {
	return point_new(
		random_range(board.top_left.x, board.bottom_right.x),
		random_range(board.top_left.y, board.bottom_right.y)
	);
}

bool board_is_inside(Board board, Point position) {
	return (
		(position.x >= board.top_left.x && position.x <= board.bottom_right.x) &&
		(position.y >= board.top_left.y && position.y <= board.bottom_right.y)
	);
}

size_t board_apple_find(Board board, Point position) {
	size_t index = 0;
	struct Node *node = list_get_node(board.apples, 0);
	while (node != NULL) {
		Apple *apple = node->content;
		if (point_equals(apple->position, position)) {
			return index;
		}

		index++;
		node = node->next;
	}
	return -1;
}

bool board_is_apple(Board board, Point position) {
	return board_apple_find(board, position) != -1;
}

void board_apple_place(Board *board, Apple *apple, Snake snake) {
	Point found = point_new(0, 0);
	do {
		found = board_random_position(*board);
	} while (snake_is(snake, found) || board_is_apple(*board, found));
	apple->position = found;
	list_push(&board->apples, apple);
}

void board_apples_draw(Board board) {
	struct Node *node = list_get_node(board.apples, 0);
	while (node != NULL) {
		Apple *apple = node->content;
		move(apple->position.y, apple->position.x);
		addch(apple->symbol);

		node = node->next;
	}
}

void board_destroy(Board *board) {
	list_free_contents(board->apples);
	list_destroy(&board->apples);
}
