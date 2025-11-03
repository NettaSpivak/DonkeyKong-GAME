#include "GameLoad.h"

void GameLoad::start() {
	ShowConsoleCursor(false);
	getAllFileNames(steps_file, ".steps");// get all steps files name
	getAllFileNames(results_file, ".result"); // get all results files name
	getSavedScreensFileNames();
	if (run(first_stage)) {
		victory();
	}
}

bool GameLoad::run_level(std::string fileName) {
	int game_loops = ZERO;
	bool is_player_alive = true;
	bool is_p_pressed = false;
	bool is_unmatching_result_found = false;
	b.reset(); // reset to original board
	if (!isSilent) {
		b.print(Board::Board_Type::BOARD_GAME); // prints the reset board
	}
	restart_game();
	set_barrels(); // set the barrel arr
	size_t next_loss_iter = ZERO;
	while (true) { // game loop
		if (game_loops % COMMON_START_BARREL == ZERO) {
			find_availble_barrel();// find a non in game barrle and turn it on
		}
		if (!isSilent) {
			player.get_pos().draw(player.get_ch()); // draw mario to screen
			draw_enemys(); // draws all in game barrels and ghosts
			Sleep(50);
		}
		if (steps.isNextStepOnIteration(game_loops)) { // if file has new step at curr game loop
			std::array<char, 2> next_step = steps.popStep(game_loops);
			keyPressed(next_step[0]);
			if (next_step[0] == b.hammer_ch || next_step[1] == b.hammer_ch) {
				is_p_pressed = true;
			}
		}
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
			checkUnMatchFinishGame(game_loops, fileName);
			return true; // continue to next level
		}
		if (!is_player_alive || enemys_hit_something(player.get_pos()) || Is_mario_and_enemy_switch_and_hit()) { // if mario is still alive after movement or hited by a barrel or an explotion
			player.take_life();
			score -= TAKE_LIFE_SCORE;
			if (score < 0) score = 0;
			b.draw_score(score);
			if (player.get_life_count() == ZERO) { // if mario end his lifes
				if (!checkUnMatchFinishGame(game_loops, fileName))
					fail();
				return false; // exit game
			}
			else {
				if (checkUnMatchMidGameResult(game_loops, fileName))
					return false;
				restart_game();
			}

		}
		else {
			if (results.isFinishedGame(game_loops)) { // if at result file game ended but at board didnt 
				results.reportResultError("Results file reached finish while game hadn't!", fileName, game_loops);
				return false;
			}
			else {
				next_loss_iter = results.getNextBombIteration();
				if (next_loss_iter != 0 && game_loops > next_loss_iter) {
					results.reportResultError("Results file have a loss event while game hadn't!", fileName, game_loops);
					return false;
				}
			}
		}

		game_loops++;

	}
}


bool GameLoad::run(int start_stage) {
	for (int i = start_stage; i <= stages.size(); i++) {
		set_level(i);
		if (!checksAndPreparesGame()) { // board isnt valid
			continue; // check next stage
		}
		else { // stage is valid
			bool next_stage = run_level(stages[i - 1]);
			if (!next_stage) { // user dont want to continue to next stage or failed game
				return false;
			}
		}
	}
	return true; // player finish all stages and didnt fail them - victory
}

void GameLoad::set_level(int level) {
	steps.clearSteps();
	results.clearResults();
	steps.loadSteps(steps_file[level - 1]);
	random_seed = steps.getRandomSeed();
	results.loadResults(results_file[level - 1]);
	srand(random_seed);
	Game::set_level(level);
}

bool GameLoad::checkUnMatchFinishGame(int iteration, std::string fileName) {
	bool unMatch = false;
	if (results.popResult() != std::pair{ iteration, Results::Finish }) {
		results.reportResultError("Results file doesn't match finished event!", fileName, iteration);
		unMatch = true;
	}
	if (results.popResult().second != Results::NoResult) {
		results.reportResultError("Results file has additional events after finish event!", fileName, iteration);
		unMatch = true;
	}
	if (results.getScore() != score) {
		results.reportResultError("Results file score doesn't match game score!", fileName, iteration);
		unMatch = true;
	}
	return unMatch;
}

bool GameLoad::checkUnMatchMidGameResult(int iteration, std::string fileName) {
	if (results.popResult() != std::pair{ iteration, Results::Loss }) {
		results.reportResultError("Results file doesn't match loss event!", fileName, iteration);
		return true;
	}
	return false;
}

void GameLoad::getSavedScreensFileNames() {
	namespace fs = std::filesystem;
	for (size_t i = 0; i < steps_file.size(); i++) { // goes threw all the saved stages
		std::string filename_prefix = steps_file[i].substr(0, steps_file[i].find_last_of('.'));
		std::string stageFilename = filename_prefix + ".screen";
		stages.push_back(stageFilename);
	}
}
