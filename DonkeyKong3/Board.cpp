#include "Board.h"
#include <cstring>
#include <iostream>

void Board::loadGameBoard(const std::string& f_name, std::vector<Point>& ghost_point, std::vector<Point>& climbing_ghost_point) {
	int curr_row = 0;
	int curr_col;
	char c;
	std::ifstream board1(f_name); //open the file 
	if (!board1.is_open()) { // if open dosen't succeeded  - print error message
		std::cerr << "Error: Unable to open file " << f_name << std::endl;
		return;
	}

	while (curr_row < MAX_Y && !board1.eof()) { //read the file 
		curr_col = 0; //Resetting the column counter on each new row
		while (curr_col < MAX_X && board1.get(c) && c != '\n') { // read file until end of line or reach to MAX X
			originalBoard[curr_row][curr_col] = c;
			checkCharFromBoard(c, curr_col, curr_row, ghost_point, climbing_ghost_point);
			curr_col++;
		}

		while (c != '\n' && board1.get(c)); // get chars after max_x 
		curr_row++;
	}

	board1.close();
}

void Board::checkCharFromBoard(char c, int pos_x, int pos_y, std::vector<Point>& ghost_point, std::vector<Point>& climbing_ghost_point) { // get special char point and switch with space
	switch (c) {
	case mario_ch:
		MARIO_START = Point(pos_x, pos_y);
		originalBoard[pos_y][pos_x] = ' ';
		break;
	case donkey_ch:
		DONKEY_KONG_POS = Point(pos_x, pos_y);
		originalBoard[pos_y][pos_x] = ' ';
		break;
	case paulin_ch:
		PAULINE_POS = Point(pos_x, pos_y);
		originalBoard[pos_y][pos_x] = ' ';
		break;
	case legend_ch:
		LEGEND_POS = Point(pos_x, pos_y);
		originalBoard[pos_y][pos_x] = ' ';
		break;
	case ghost_ch:
		ghost_point.push_back({ pos_x, pos_y });
		originalBoard[pos_y][pos_x] = ' ';
		break;
	case climbed_ghost_ch:
		climbing_ghost_point.push_back({ pos_x, pos_y });
		originalBoard[pos_y][pos_x] = ' ';
		break;
	case hammer_ch:
		HAMMER_POS = Point(pos_x, pos_y);
		originalBoard[pos_y][pos_x] = ' ';
		break;
	}
}



void Board::reset() { // reset the curr board to wanted original board
	for (int i = 0; i < MAX_Y; i++) {
		memcpy(currentBoard[i], originalBoard[i], MAX_X + 1);
	}
}

void Board::print(Board_Type board) { // prints the wanted board to screen
	Point start_print = { 0,0 };
	start_print.gotoxy();
	const char* const* board_to_print = nullptr;

	switch (board) // checks the right board
	{
	case Board_Type::MAIN_MENU:
		board_to_print = menu_board;
		break;
	case Board_Type::BOARD_GAME:
		for (int i = 0; i < MAX_Y - 1; i++) {
			std::cout << originalBoard[i] << '\n';
		}
		std::cout << originalBoard[MAX_Y - 1];
		break;
	case Board_Type::INSTRUCTIONS:
		board_to_print = instructions_board;
		break;
	case Board_Type::END_OF_GAME:
		board_to_print = end_of_game_board;
		break;
	case Board_Type::WIN_STAGE:
		board_to_print = win_stage_board;
		break;
	case Board_Type::VICTORY:
		board_to_print = victory_board;
		break;
	case Board_Type::PAUSE_BOARD:
		board_to_print = pause_board;
		break;
	case Board_Type::STAGES:
		board_to_print = stages_board;
		break;
	case Board_Type::ERROR_B:
		board_to_print = error_board;
		break;
	}
	if (board != Board_Type::BOARD_GAME) {
		for (int i = 0; i < MAX_Y - 1; i++) { // we coverd all cases so for sure we wont get a non valod pointer
			std::cout << board_to_print[i] << '\n';
		}
		std::cout << board_to_print[MAX_Y - 1];
	}
}

void Board::restore(Point p) { // get back the char from original board to the curr board at point
	if (isSilent)
		return;
	char c = getChar(p);
	p.draw(c);
}


bool Board::IsThereAFloorUnder(Point p) const { // function checks if there is a floor under point 
	if (p.get_y() > MAX_Y) { //if under first floor
		return false;
	}
	Point under(p.get_x(), p.get_y() + 1);
	char ch = getChar(under);
	if (ch == regular_floor || ch == right_dir_floor || ch == left_dir_floor || ch == wall_ch)
		return true;
	if (p.get_y() == MAX_Y - 1) { // if first floor has no floor
		return true;
	}
	return false;
}

bool Board::checksAndPreparesBoard() {
	if (DONKEY_KONG_POS == DEFAULT_POS || PAULINE_POS == DEFAULT_POS || LEGEND_POS == DEFAULT_POS || MARIO_START == DEFAULT_POS) // if there is a game obj that is missing
		return false;
	if (!IsThereAFloorUnder(DONKEY_KONG_POS) || !IsThereAFloorUnder(PAULINE_POS))
		return false;

	// if all valid put obj char at original board
	originalBoard[DONKEY_KONG_POS.get_y()][DONKEY_KONG_POS.get_x()] = donkey_ch;
	originalBoard[PAULINE_POS.get_y()][PAULINE_POS.get_x()] = paulin_ch;
	if (HAMMER_POS != DEFAULT_POS)
		originalBoard[HAMMER_POS.get_y()][HAMMER_POS.get_x()] = hammer_ch;
	if (LEGEND_POS.get_x() + LEGEND_X - 1 < MAX_X && LEGEND_POS.get_y() + LEGEND_Y - 1 < MAX_Y) { // check if there is space for the legend board
		// set the life pos and hammer check pos
		LIFE_COUNT_POS = { DEFAULT_LIFE_COUNT_POS.get_x() + LEGEND_POS.get_x() , DEFAULT_LIFE_COUNT_POS.get_y() + LEGEND_POS.get_y() };
		HAMMER_CHECK_POS = { DEFAULT_HAMMER_CHECK_POS.get_x() + LEGEND_POS.get_x() , DEFAULT_HAMMER_CHECK_POS.get_y() + LEGEND_POS.get_y() };
		SCORE_POS = { DEFAULT_SCORE_POS.get_x() + LEGEND_POS.get_x(), DEFAULT_SCORE_POS.get_y() + LEGEND_POS.get_y() };
		for (int i = 0; i < LEGEND_X; i++) {
			for (int j = 0; j < LEGEND_Y; j++) {
				originalBoard[LEGEND_POS.get_y() + j][LEGEND_POS.get_x() + i] = legend[j][i];
			}
		}
	}
	else
		return false; // there isnt enough space for legend
	return true;
}

bool Board::IsThereAFloor(Point p) const { // function checks if there is a floor at point 
	char ch = getChar(p);
	if (ch == regular_floor || ch == right_dir_floor || ch == left_dir_floor)
		return true;
	if (p.get_y() == MAX_Y) // first floor has no floor
		return true;
	return false;
}

bool Board::IsThereAWall(Point p) const { // function checks if there is a floor at point 
	char ch = getChar(p);
	if (ch == wall_ch)
		return true;
	return false;
}


bool Board::IsThereALadder(Point p) const // checks if there is a ladder on point
{
	if (p.get_y() > MAX_Y) { //if under first floor
		return false;
	}
	char ch = getChar(p);
	if (ch == ladder_ch)
		return true;
	return false;

}

bool Board::ReachToEndOfBoard(Point pos, Point::Direction& dir) { // checks if point reaches the end of the board or hit a wall or floor
	bool res = false;
	if (pos.get_x() + dir.x < 0 || pos.get_x() + dir.x >= MAX_X || IsThereAWall({ pos.get_x() + dir.x , pos.get_y() })) { //stops the x movement
		dir.x = ZERO;
		res = true;
	}
	if (pos.get_y() + dir.y < 0 || pos.get_y() + dir.y > MAX_Y || IsThereAWall({ pos.get_x() , pos.get_y() + dir.y })) { //stops the y movement
		dir.y = ZERO;
		res = true;
	}
	return res;
}


void Board::draw_score(int score) const {
	if (isSilent)
		return;

	Point set_score = SCORE_POS;
	for (int i = 0; i < LEGEND_X - DEFAULT_SCORE_POS.get_x(); i++) {
		set_score.draw(' ');
		set_score.set_x(1);
	}
	SCORE_POS.gotoxy();
	std::cout << score;
}

void Board::restartHammer() {
	if (HAMMER_POS != DEFAULT_POS) {
		originalBoard[HAMMER_POS.get_y()][HAMMER_POS.get_x()] = hammer_ch;
		if (!isSilent)
			HAMMER_POS.draw(hammer_ch);
	}
}

void Board::update_default_pos() {
	DONKEY_KONG_POS = DEFAULT_POS; // point for donkey kong place
	PAULINE_POS = DEFAULT_POS; // point for pauline place
	LEGEND_POS = DEFAULT_POS; // point for the legend place
	MARIO_START = DEFAULT_POS; // point for mario place
	HAMMER_POS = DEFAULT_POS; // point for hammer place
	LIFE_COUNT_POS = DEFAULT_LIFE_COUNT_POS; // point for the life count place according to legend board
	SCORE_POS = DEFAULT_SCORE_POS; // point for the score according to legend board
	HAMMER_CHECK_POS = DEFAULT_HAMMER_CHECK_POS; // point for the hammer check according to legend board
}

void Board::clear_row(int row) {
	if (isSilent)
		return;

	Point clear{ 0, row };
	clear.gotoxy();
	for (int i = 0; i < MAX_X; i++) {
		std::cout << ' ';
	}
}