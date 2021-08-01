Board board_new(Point top_left, Point bottom_right) {
	return (Board) {top_left, bottom_right};
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
