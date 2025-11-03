#include "UserGame.h"


bool UserGame::run_level(std::string fileName) {
	int game_loops = ZERO;
	bool is_player_alive = true;
	b.reset(); // reset to original board
	b.print(Board::Board_Type::BOARD_GAME); // prints the reset board
	restart_game();
	set_barrels(); // set the barrel arr
	while (true) { // game loop
		if (game_loops % COMMON_START_BARREL == ZERO) {
			find_availble_barrel();// find a non in game barrle and turn it on
		}
		player.get_pos().draw(player.get_ch()); // draw mario to screen
		draw_enemys(); // draws all in game barrels and ghosts
		if (_kbhit()) { // if the user press a key then check if the key is a direction and send it to mario to change direction of movement
			char key = _getch();
			if (key == ESC) {
				if (!pause()) {
					return false; // if player want to exit game
				}
				else {
					b.print(Board::Board_Type::BOARD_GAME); // prints the curr game board
					b.LIFE_COUNT_POS.draw(('0' + player.get_life_count()));
					b.draw_score(score);
					b.HAMMER_CHECK_POS.draw(player.Is_Hold_A_Hammer() ? 'V' : 'X');
				}
			}
			std::array<char, 2> curr_step = { ' ', ' ' };
			if (keyPressed(key)) {
				curr_step[0] = key;
				if (player.Is_Hold_A_Hammer()) {
					Sleep(70);
					if (std::tolower(key) != b.hammer_ch && _kbhit()) { // if first key isnt hammer and another key was pressed
						key = _getch();
						if (std::tolower(key) == b.hammer_ch) { // if mario with hammer and 'p' pressed
							curr_step[1] = b.hammer_ch;
							is_p_pressed = true;
						}
					}
				}
				steps.addStep(game_loops, curr_step);
				Flush();
			}
		}
		Sleep(150);
		b.restore(player.get_pos()); // puts back the part that mario hide while moving
		restore_enemys(); // puts back the part that all the in the game enemys hide while moving
		is_player_alive = player.Mario_Movement(); // if mario loses life after fall
		if (player.Grab_Hammer()) { //garb hammer 
			score += GRAB_HAMMER_SCORE;
			b.draw_score(score);
		}
		enemys_movement(); // moves all in game enemys
		if (is_p_pressed) {
			p_pressed(); // execute hammer 
			is_p_pressed = false;
		}
		if (player.get_pos() == b.PAULINE_POS) {
			score += calc_win_score(game_loops);
			b.draw_score(score);
			results.addResult(game_loops, Results::Finish);
			return win_stage();
		}
		if (!is_player_alive || enemys_hit_something(player.get_pos()) || Is_mario_and_enemy_switch_and_hit()) { // if mario is still alive after movement or hited by a barrel or an explotion
			player.take_life();
			score -= TAKE_LIFE_SCORE;
			if (score < 0) score = 0;
			b.draw_score(score);
			if (player.get_life_count() == ZERO) { // if mario end his lifes
				results.addResult(game_loops, Results::Finish);
				fail();
				return false; // exit game
			}
			else {
				results.addResult(game_loops, Results::Loss);
				restart_game();
			}
		}
		game_loops++;
	}
}


void UserGame::start() { // main menu
	Flush(); // cleaning buffer
	ShowConsoleCursor(false);
	getAllFileNames(stages, ".screen");
	char key;
	bool is_win;
	while (true) {
		b.print(Board::Board_Type::MAIN_MENU); // print board to screen 
		key = _getch(); // wait for key from user
		if (key == PLAY || key == STAGES) {
			if (stages.empty()) {
				error_board();
				continue;
			}
			else {
				bool full_game = true; // defult start
				int stage = 1; // defult start
				if (key == STAGES) { //case stages
					stage = stage_menu();
					full_game = false; // not a full game
					if (stage == 0) // get back to main menu
						continue;
				}
				is_win = run(stage, full_game);
				if (is_win) {
					victory();
				}

				player.reset_life_count(); // reset life 
				score = ZERO; // reset score

				continue; // get back to main menu
			}
		}
		else if (key == INSTRUCTIONS) {
			instructions_menu();
		}
		else if (key == EXIT) { //9==EXIT
			break;
		}
	}

}

bool UserGame::run(int start_stage, bool full_game) {
	for (int i = start_stage; i <= stages.size(); i++) {
		random_seed = static_cast<long>(std::chrono::system_clock::now().time_since_epoch().count());
		if (isSave)
			steps.setRandomSeed(random_seed);
		srand(random_seed);
		set_level(i);
		if (!checksAndPreparesGame()) { // board isnt valid
			if (!full_game) { // selected game is'nt valid
				error_board();
				return false; // get back to main menu
			}
			else { //full game
				continue; // check next stage
			}
		}
		else { // stage is valid
			full_game = true; // start the game from chosen stage and continue till end 
			bool next_stage = run_level(stages[i - 1]);
			if (isSave) {
				std::string filename_prefix = stages[i - 1].substr(0, stages[i - 1].find_last_of('.'));
				std::string stepsFilename = filename_prefix + ".steps";
				std::string resultsFilename = filename_prefix + ".result";
				steps.saveSteps(stepsFilename);
				results.saveResults(resultsFilename, score);
			}

			if (!next_stage) { // user dont want to continue to next stage or failed game
				return false;
			}
		}
	}

}

void UserGame::set_level(int level) {
	steps.clearSteps();
	results.clearResults();
	Game::set_level(level);
}


int UserGame::stage_menu() { // select a stage for game
	b.print(Board::Board_Type::STAGES);
	Point start_print;
	int curr_print_stage = 1;
	int page_num;
	int stage = ZERO;
	for (page_num = 0; page_num < (stages.size() / 8) + 1; page_num++) { // there is 8 stages at page, get the amount of pages that needed
		int i;
		for (int j = 1; j <= 8; j++) {
			b.clear_row(b.get_stages_print_pos().get_y() + j);
		}
		for (i = 1; (i <= 8) && (curr_print_stage <= stages.size()); i++, curr_print_stage++) {
			start_print = { b.get_stages_print_pos().get_x(),b.get_stages_print_pos().get_y() + i };
			start_print.gotoxy();
			std::cout << "(" << i << ") Stage " << curr_print_stage << std::endl;
		}
		while ((stage = _getch() - '0') < 0 || stage > i); // checks if there is a stage as user want
		if (stage != 9) // user choose level
			break;
	}
	if (stage == ZERO) // get back to main menu
		return stage;
	else
		return (stage + (8 * page_num));
}


bool UserGame::pause() { // pause the game , wait for a key from user
	Flush(); // cleaning buffer
	char key;
	b.print(Board::Board_Type::PAUSE_BOARD);
	while (true) { // until getting a right key
		key = _getch();
		if (key == ESC) {
			return true; // player want to continu play
		}
		else if (key == Board::Board_Type::MAIN_MENU) {
			return false; // player want to return to menu
		}
	}

}



void UserGame::error_board() { // not valid board or no boards
	b.print(Board::Board_Type::ERROR_B);
	while (_getch() != Board::Board_Type::MAIN_MENU);
}


void UserGame::instructions_menu() {
	b.print(Board::Board_Type::INSTRUCTIONS);
	while (_getch() != Board::Board_Type::MAIN_MENU);
}


