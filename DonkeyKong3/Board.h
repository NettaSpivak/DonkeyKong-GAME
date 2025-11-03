#pragma once
#include <string>
#include <fstream>
#include "Point.h"
#include <vector>

using std::string;

class Board
{
public:
	const enum Boardsize { MAX_X = 80, MAX_Y = 25, LEGEND_X = 20, LEGEND_Y = 3 }; // const board size
	const enum Board_Type { VICTORY = '0', BOARD_GAME = '1', MAIN_MENU = '2', STAGES = '3', END_OF_GAME = '4', PAUSE_BOARD = '5', ERROR_B = '6', WIN_STAGE = '7', INSTRUCTIONS = '8' }; // const board type - nested in class

private:
	char originalBoard[MAX_Y][MAX_X + 1];
	char currentBoard[MAX_Y][MAX_X + 1]; // +1 for "\0" 
	static constexpr Point DEFAULT_LIFE_COUNT_POS = { 7, 0 };
	static constexpr Point DEFAULT_SCORE_POS = { 8, 1 };
	static constexpr Point DEFAULT_HAMMER_CHECK_POS = { 12, 2 };
	static constexpr Point STAGES_PRINT_POS = { 30, 13 };


	int game_life;
	bool isSilent = false;

	const char* menu_board[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", // 0
		  "*                                                                              *", // 1
		  "*                                                                              *", // 2
		  "*                               DONKEY KONG GAME                               *", // 3
		  "*                                                                              *", // 4
		  "*                                                                              *", // 5
		  "********************************************************************************", // 6
		  "*                                                                              *", // 7
		  "*                                                                              *", // 8
		  "*                                                                              *", // 9
		  "*                                                                              *", // 10
		  "*                       MAIN MENU :                                            *", // 11
		  "*                      ¯¯¯¯¯¯¯¯¯¯¯¯¯                                           *", // 12
		  "*                       (1) Start New Game                                     *", // 13
		  "*                       (3) Select a stage                                     *", // 14
		  "*                       (8) Present Instructions and Keys                      *", // 15
		  "*                       (9) EXIT                                               *", // 16
		  "*                                                                              *", // 17
		  "*                                                                              *", // 18
		  "*                                                                              *", // 19
		  "*                                                                              *", // 20
		  "*                                                                              *", // 21
		  "*                                                                              *", // 22
		  "*                                                                              *", // 23
		  "********************************************************************************"  // 24
	};

	const char* instructions_board[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", // 0
		  "*                                                                              *", // 1
		  "*                         DONKEY KONG - INSTRUCTIONS                           *", // 2
		  "*                       Press (2) to return to main menu                       *", // 3
		  "********************************************************************************", // 4
		  "*  KEYS:                                                                       *", // 5
		  "*  ¯¯¯¯¯¯                                                                      *", // 6
		  "*  LEFT       : a or A       |    *  DOWN     : x or X                         *", // 7
		  "*  RIGHT      : d or D       |    *  STAY     : s or S                         *", // 8
		  "*  UP / JUMP  : w or W       |                                                 *", // 9
		  "*  Use these keys to move Mario (@)                                            *", // 10                                                                             
		  "*                                                                              *", // 11
		  "*   GAME ELEMENTS:                                                             *", // 12
		  "*  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                                             *", // 13
		  "*   Donkey Kong      (&)          |   *   Ladders      (H)                     *", // 14
		  "*   Barrels          (O)          |   *   Mario        (@)                     *", // 15
		  "*   Floors           (<, >, =)    |   *   Pauline      ($)                     *", // 16
		  "*                                                                              *", // 17
		  "*   OBJECTIVE:                                                                 *", // 18
		  "*  ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                                                             *", // 19
		  "*   Rescue Pauline ($) by reaching her location! Avoid barrels (O),            *", // 20
		  "*   explosions, and falls. Use ladders (H) and jumps to navigate the floors.   *", // 21
		  "*                                                                              *", // 22
		  "*   PAUSE: Press ESC to pause. Press ESC again to resume.                      *", // 23
		  "********************************************************************************" // 24
	};

	const char* stages_board[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", // 0
		  "*                                                                              *", // 1
		  "*                                                                              *", // 2
		  "*                               DONKEY KONG GAME                               *", // 3
		  "*                                                                              *", // 4
		  "*                                                                              *", // 5
		  "********************************************************************************", // 6
		  "                                                                                ", // 7
		  "                               SELECT GAME LEVEL :                              ", // 8
		  "                              ¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯¯                             ", // 9
		  "                        Choose the level you want to challenge :                ", // 10
		  "                        Press (0) to return to the main menu                    ", // 11
		  "                        Press (9) to proceed to next stages screen              ", // 12
		  "                                                                                ", // 13
		  "                                                                                ", // 14
		  "                                                                                ", // 15
		  "                                                                                ", // 16
		  "                                                                                ", // 17
		  "                                                                                ", // 18
		  "                                                                                ", // 19
		  "                                                                                ", // 20
		  "                                                                                ", // 21
		  "                                                                                ", // 22
		  "                                                                                ", // 23
		  "                                                                                "  // 24
	};

	const char* end_of_game_board[MAX_Y] = {

		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", //0
		  "*                                                                              *", //1
		  "*                                  GAME OVER                                   *", //2
		  "*                                  SCORE:                                      *", //3
		  "********************************************************************************", //4
		  "*                                                                              *", //5
		  "*                                                                              *", //6
		  "*                      Mario has lost all his lives.                           *", //7
		  "*                                                                              *", //8
		  "*                                                                              *", //9
		  "*                                                                              *", //10
		  "*                     ##############################                           *", //11
		  "*                     #                            #                           *", //12
		  "*                     #     BETTER LUCK NEXT       #                           *", //13
		  "*                     #           TIME!            #                           *", //14
		  "*                     #                            #                           *", //15
		  "*                     ##############################                           *", //16
		  "*                                                                              *", //17
		  "*                                                                              *", //18
		  "*                                                                              *", //19
		  "*                                                                              *", //20
		  "*                       Press (2) for finish game.                             *", //21
		  "*                                                                              *", //22
		  "*                                                                              *", //23
		  "********************************************************************************"  //24
	};

	const char* win_stage_board[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", //0
		  "*                                                                              *", //1
		  "*                                  YOU WIN!                                    *", //2
		  "*                                  SCORE:                                      *", //3
		  "********************************************************************************", //4
		  "*                                                                              *", //5
		  "*                        ############################                          *", //6
		  "*                        #                          #                          *", //7
		  "*                        #  ** CONGRATULATIONS! **  #                          *", //8
		  "*                        #                          #                          *", //9
		  "*                        #  Mario rescued Pauline!  #                          *", //10
		  "*                        #                          #                          *", //11
		  "*                        ############################                          *", //12
		  "*                                                                              *", //13
		  "*                                                                              *", //14
		  "*          #########################################################           *", //15
		  "*          #                                                       #           *", //16
		  "*          #              Press (1) for next level                 #           *", //17
		  "*          #                                                       #           *", //18
		  "*          #########################################################           *", //19
		  "*                                                                              *", //20
		  "*                                                                              *", //21
		  "*                   Press (2) to Return to the Main Menu                       *", //22
		  "*                                                                              *", //23
		  "********************************************************************************" //24
	};

	const char* victory_board[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", //0
		  "*                                                                              *", //1
		  "*                                  VICTORY!!!                                  *", //2
		  "*                                  SCORE:                                      *", //3
		  "********************************************************************************", //4
		  "*                                                                              *", //5
		  "*                        ############################                          *", //6
		  "*                        #                          #                          *", //7
		  "*                        #         Good job!        #                          *", //8
		  "*                        #  You've reached the end  #                          *", //9
		  "*                        #       of the Game!       #                          *", //10
		  "*                        #                          #                          *", //11
		  "*                        ############################                          *", //12
		  "*                                                                              *", //13
		  "*                                                                              *", //14
		  "*          #########################################################           *", //15
		  "*          #                                                       #           *", //16
		  "*          #            Press (2) for finish game.                 #           *", //17
		  "*          #                                                       #           *", //18
		  "*          #########################################################           *", //19
		  "*                                                                              *", //20
		  "*                             SEE YOU NEXT TIME!                               *", //21
		  "*                                                                              *", //22
		  "*                                                                              *", //23
		  "********************************************************************************"  //24
	};

	static constexpr Point END_SCORE_PRINT_POS = { 41, 3 };

	const char* pause_board[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", // 0
		  "*                                                                              *", // 1
		  "*                                                                              *", // 2
		  "*                               GAME PAUSED                                    *", // 3
		  "*                                                                              *", // 4
		  "*                                                                              *", // 5
		  "********************************************************************************", // 6
		  "*                                                                              *", // 7
		  "*                                                                              *", // 8
		  "*                         The game is currently paused.                        *", // 9
		  "*                         Take a deep breath...                                *", // 10
		  "*                                                                              *", // 11
		  "*                                                                              *", // 12
		  "*                                                                              *", // 13
		  "********************************************************************************", // 14
		  "*                                                                              *", // 15
		  "*                                                                              *", // 16
		  "*                                                                              *", // 17
		  "*                      Press (ESC) to continue playing,                        *", // 18
		  "*                   or (2) to end the game and return to the main menu.        *", // 19
		  "*                                                                              *", // 20
		  "*                                                                              *", // 21
		  "*                                                                              *", // 22
		  "*                                                                              *", // 23
		  "********************************************************************************"  // 24
	};

	const char* error_board[MAX_Y] = {
		// 01234567890123456789012345678901234567890123456789012345678901234567890123456789
		  "********************************************************************************", // 0
		  "*                                                                              *", // 1
		  "*                                                                              *", // 2
		  "*                               BOARD ERROR                                    *", // 3
		  "*                                                                              *", // 4
		  "*                                                                              *", // 5
		  "********************************************************************************", // 6
		  "*                                                                              *", // 7
		  "*                                                                              *", // 8
		  "*             The selected board is invalid or no boards are available.        *", // 9
		  "*                                                                              *", // 10
		  "*                                                                              *", // 11
		  "*                Please check your board files and try again.                  *", // 12
		  "*                                                                              *", // 13
		  "********************************************************************************", // 14
		  "*                                                                              *", // 15
		  "*                                                                              *", // 16
		  "*                                                                              *", // 17
		  "*                      Press (2) to return to the main menu.                   *", // 18
		  "*                                                                              *", // 19
		  "*                                                                              *", // 20
		  "*                                                                              *", // 21
		  "*                                                                              *", // 22
		  "*                                                                              *", // 23
		  "********************************************************************************"  // 24
	};

	char legend[LEGEND_Y][LEGEND_X + 1] = {
		//01234567890123456789
		 "# Life:             ",
		 "# Score:            ",
		 "# Is Hammer:        ",
	};

public:
	Board(int life) : game_life(life) {
		for (size_t col = 0; col < MAX_X; col++)
			for (size_t row = 0; row < MAX_Y; row++)
				originalBoard[row][col] = ' ';
	}

	static constexpr Point DEFAULT_POS = { -1, -1 };
	Point DONKEY_KONG_POS = DEFAULT_POS; // point for donkey kong place
	Point PAULINE_POS = DEFAULT_POS; // point for pauline place
	Point LEGEND_POS = DEFAULT_POS; // point for the legend place
	Point MARIO_START = DEFAULT_POS; // point for mario place
	Point HAMMER_POS = DEFAULT_POS; // point for hammer place
	Point LIFE_COUNT_POS = DEFAULT_LIFE_COUNT_POS; // point for the life count place according to legend board
	Point SCORE_POS = DEFAULT_SCORE_POS; // point for the score according to legend board
	Point HAMMER_CHECK_POS = DEFAULT_HAMMER_CHECK_POS; // point for the hammer check according to legend board

	static constexpr char regular_floor = '=';
	static constexpr char left_dir_floor = '<';
	static constexpr char right_dir_floor = '>';
	static constexpr char wall_ch = 'Q';
	static constexpr char ladder_ch = 'H';
	static constexpr char mario_ch = '@';
	static constexpr char donkey_ch = '&';
	static constexpr char paulin_ch = '$';
	static constexpr char barrel_ch = 'O';
	static constexpr char ghost_ch = 'x';
	static constexpr char climbed_ghost_ch = 'X';
	static constexpr char hammer_ch = 'p';
	static constexpr char legend_ch = 'L';

	void loadGameBoard(const std::string& f_name, std::vector<Point>& ghost_point, std::vector<Point>& climbing_ghost_point);
	void reset();
	void checkCharFromBoard(char c, int pos_x, int pos_y, std::vector<Point>& ghost_point, std::vector<Point>& climbing_ghost_point);
	void print(Board_Type board);
	char getChar(Point p) const { // returns the char on board that at the point
		return originalBoard[p.get_y()][p.get_x()];
	}
	void restore(Point p);
	void edit_og_board(Point pos, char c) {
		originalBoard[pos.get_y()][pos.get_x()] = c;
	}
	bool IsThereAFloorUnder(Point p) const;//check if there is a floor at point
	bool IsThereAFloor(Point p) const;//check if there is a floor at point 
	bool IsThereAWall(Point p) const;//check if there is a at wall point 
	bool IsThereALadder(Point p) const;//check if there is a ladder at point 
	bool ReachToEndOfBoard(Point p, Point::Direction& dir); // check if point is out of the board size , if so change dir {0,0}
	bool checksAndPreparesBoard();
	void draw_score(int score) const;
	bool IsThereAHammer(Point p) const {
		if (originalBoard[p.get_y()][p.get_x()] == hammer_ch) //current board because we don't want to change the original board 
			return true;
		return false;
	}
	void DeleteHammerFromBoard() {
		originalBoard[HAMMER_POS.get_y()][HAMMER_POS.get_x()] = ' ';
	}
	void restartHammer();
	void update_default_pos();
	Point get_score_print_pos() {
		return END_SCORE_PRINT_POS;
	}
	void clear_row(int row);

	Point get_stages_print_pos() {
		return STAGES_PRINT_POS;
	}
	void set_isSilent(bool _is_silent) {
		isSilent = _is_silent;
	}
	bool get_isSilent() {
		return isSilent;
	}
};


