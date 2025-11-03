#include "Game.h"

int Game::calc_win_score(int game_loop) { // clac score addition according to game time
	return (int)(GAME_LOOP_SCORE / (game_loop + 1));
}

bool Game::keyPressed(char key) { // if the pressed key is a moving key then change direction + check if the key is a known key
	if (std::tolower(key) == b.hammer_ch) { // if user press p 
		is_p_pressed = true;
		return true;
	}
	bool is_game_key = false;
	for (size_t i = 0; i < numKeys; i++) {
		//  goes threw the keys array and check if the pressed key is equal
		if (std::tolower(key) == keys[i]) { // if equal change the direction according to the key
			is_game_key = true;
			if (player.IsValidKey(keys[i])) { //if the move according to the presed key is valid for mario
				player.set_prevdir(player.get_dir());
				player.set_dir(Point::directions[i]);
			}
		}
	}
	return is_game_key;
}

void Game::restart_game() { // if mario died but life didnt ended then game restart
	Flush(); // cleaning buffer
	player.mario_restart(); // resrt mario
	enemys_restart(); // reset all barrels
	if (!isSilent) {
		b.LIFE_COUNT_POS.draw(('0' + player.get_life_count()));
		b.HAMMER_CHECK_POS.draw('X');
		b.draw_score(score);
	}
}


void Game::set_level(int level) { // set the level for game start
	b.update_default_pos(); // set all object poses to def
	std::vector<Point> ghost_pos, climbing_ghost_point;
	b.loadGameBoard(stages[level - 1], ghost_pos, climbing_ghost_point); // get the board and all of the ghosts positions
	for (Ghost* ghost : ghosts) {
		delete ghost;
	}
	ghosts.clear();
	Ghost* tmp;
	for (size_t i = 0; i < ghost_pos.size(); i++) { // push ghosts to vec
		tmp = new Ghost(b.ghost_ch, &b, ghost_pos[i]);
		ghosts.push_back(tmp);
	}
	for (size_t i = 0; i < climbing_ghost_point.size(); i++) { // push climbing ghosts to vec
		tmp = new Climbed_Ghost(b.climbed_ghost_ch, &b, climbing_ghost_point[i]);
		ghosts.push_back(tmp);
	}
}

bool Game::win_stage() {
	b.print(Board::Board_Type::WIN_STAGE);
	b.get_score_print_pos().gotoxy();
	std::cout << score;
	char key;
	while (true) {
		key = _getch();
		if (key == Board::Board_Type::MAIN_MENU)
			return false;
		else if (key == Board::Board_Type::BOARD_GAME)
			return true;
	}
}

void Game::victory() { // win all game
	if (isSilent) {
		results.reportEndGameResult(Results::EndResult::WIN, score);
		return;
	}
	b.print(Board::Board_Type::VICTORY);
	b.get_score_print_pos().gotoxy();
	std::cout << score;
	while (_getch() != Board::Board_Type::MAIN_MENU);
}

void Game::fail() {
	if (isSilent) {
		results.reportEndGameResult(Results::EndResult::FAIL, score);
		return;
	}
	b.print(Board::Board_Type::END_OF_GAME);
	b.get_score_print_pos().gotoxy();
	std::cout << score;
	while (_getch() != Board::Board_Type::MAIN_MENU);
}

bool Game::Is_mario_and_enemy_switch_and_hit() const {// checks hit at times that mario and enemy switch so the prev test isnt relevant
	Point mario_prev = Point(player.get_pos().get_x() - player.get_dir().x, player.get_pos().get_y() - player.get_dir().y); // prev pos of mario before move
	return IsAnyEnemyPrevPosHitPoint(player.get_pos(), mario_prev);

}

void Game::find_availble_barrel() { // checks if there is a barrel that not in game , start the first that found
	for (size_t i = 0; i < NUM_OF_BARRELS; i++) {
		if (!barrels[i].get_is_alive()) {
			barrels[i].set_board_p(&b);
			barrels[i].getFirstDir();
			barrels[i].set_start_pos(b.DONKEY_KONG_POS);
			barrels[i].set_alive(true);
			break;
		}
	}
}

void Game::draw_enemys() { // draw all enemys to screen, if one is exploded draw the explosion
	if (isSilent)
		return;
	for (size_t i = 0; i < NUM_OF_BARRELS; i++) {
		if (barrels[i].get_is_alive()) {
			barrels[i].get_pos().draw(barrels[i].get_ch());
			if (barrels[i].get_IsExploded()) {
				barrels[i].get_pos().draw('*');
				Sleep(50);
			}
		}
	}
	for (size_t i = 0; i < ghosts.size(); i++) {
		if (ghosts[i]->get_is_alive()) {
			ghosts[i]->get_pos().draw(ghosts[i]->get_ch());
		}
	}
}

void Game::restore_enemys() // restore the original char from original board to the enemy place
{
	for (size_t i = 0; i < NUM_OF_BARRELS; i++) {
		if (barrels[i].get_is_alive()) {
			b.restore(barrels[i].get_pos());
		}
	}
	for (size_t i = 0; i < ghosts.size(); i++) {
		if (ghosts[i]->get_is_alive()) {
			b.restore(ghosts[i]->get_pos());
		}
	}
}

void Game::enemys_movement() //move all enemys
{
	for (size_t i = 0; i < NUM_OF_BARRELS; i++) {
		if (barrels[i].get_is_alive()) {
			barrels[i].barrel_movement();
		}
	}
	for (size_t i = 0; i < ghosts.size(); i++) {
		if (ghosts[i]->get_is_alive()) {
			ghost_hit_ghost();
			ghosts[i]->movement();

		}
	}
}


void Game::ghost_hit_ghost() { // if two ghosts collides then change their dir
	for (int i = 0; i < ghosts.size(); i++) {
		if (ghosts[i]->get_is_alive()) {
			Point next_ghost_pos = { ghosts[i]->get_pos().get_x() + ghosts[i]->get_dir().x, ghosts[i]->get_pos().get_y() };
			for (int j = i; j < ghosts.size(); j++) {
				if (ghosts[i]->get_is_alive()) {
					Point next_checked_ghost_pos = { ghosts[j]->get_pos().get_x() + ghosts[j]->get_dir().x, ghosts[j]->get_pos().get_y() };
					if ((ghosts[i]->get_pos() == next_checked_ghost_pos && next_ghost_pos == ghosts[j]->get_pos()) || (next_ghost_pos == next_checked_ghost_pos)) {
						ghosts[i]->set_dir({ -ghosts[i]->get_dir().x, -ghosts[i]->get_dir().y });
						ghosts[j]->set_dir({ -ghosts[j]->get_dir().x, -ghosts[j]->get_dir().y });
					}
				}
			}
		}
	}
}

bool Game::enemys_hit_something(Point mario_p) const {  //checks if one of the enemy hit mario
	bool result = false;
	for (int i = 0; i < NUM_OF_BARRELS; i++) { // goes threw all barrels
		if (barrels[i].get_is_alive()) { // if the curr barrel is alive
			if (barrels[i].get_IsExploded()) { // if the barrel is during explotion
				result = barrels[i].barrel_explosion_hit_something(mario_p);
			}
			else {
				result = (barrels[i].get_pos() == mario_p);
			}
		}
		if (result)
			return result;
	}
	for (int i = 0; i < ghosts.size(); i++) { // goes threw all ghosts
		if (ghosts[i]->get_is_alive()) { // if the curr ghost is alive
			result = (ghosts[i]->get_pos() == mario_p);
		}
		if (result) break;
	}
	return result;
}

void Game::enemys_restart() {
	for (size_t i = 0; i < NUM_OF_BARRELS; i++) {
		if (barrels[i].get_is_alive()) {
			barrels[i].reset(); // reset all barrels
		}
	}
	for (int i = 0; i < ghosts.size(); i++) { // goes threw all ghosts
		if (ghosts[i]->get_is_alive()) // if the curr ghost is alive
			ghosts[i]->reset();
	}
}

bool Game::IsAnyEnemyPrevPosHitPoint(Point p, Point p_prev) const {// checks if enemys swich plase with given point       
	for (size_t i = 0; i < NUM_OF_BARRELS; i++) { // goes threw all barrels
		if (barrels[i].get_is_alive()) { // only live barrels
			Point barrel_prev = Point((barrels[i].get_pos().get_x() - barrels[i].get_dir().x), (barrels[i].get_pos().get_y() - barrels[i].get_dir().y)); // prev pos of barrel before move
			if (p == barrel_prev && p_prev == barrels[i].get_pos()) // if barrel is at the prev place of point and point is at prev place of barrel
				return true;
		}
	}
	for (int i = 0; i < ghosts.size(); i++) { // goes threw all ghosts
		if (ghosts[i]->get_is_alive()) { // // only live barrels
			Point ghost_prev = Point((ghosts[i]->get_pos().get_x() - ghosts[i]->get_dir().x), (ghosts[i]->get_pos().get_y() - ghosts[i]->get_dir().y)); // prev pos of ghost before move
			if (p == ghost_prev && p_prev == ghosts[i]->get_pos()) // if ghost is at the prev place of point and point is at prev place of ghost
				return true;
		}
	}
	return false;
}

bool Game::checksAndPreparesGame() {
	if (!b.checksAndPreparesBoard()) // checks valid obj and puts in board
		return false;

	for (int i = 0; i < ghosts.size(); i++) { // check valid ghosts
		if (typeid(*ghosts[i]) == typeid(Climbed_Ghost)) { // if a spacial ghost 
			Point under = Point(ghosts[i]->get_pos().get_x(), ghosts[i]->get_pos().get_y() + 1);// the point under ghost pos
			Point above = Point(ghosts[i]->get_pos().get_x(), ghosts[i]->get_pos().get_y() + 1);// the point above ghost pos
			if (b.IsThereALadder(under) || b.IsThereALadder(above)) {
				b.edit_og_board(ghosts[i]->get_pos(), b.ladder_ch);
				continue; // no need to check floor under
			}
		}
		if (!b.IsThereAFloorUnder(ghosts[i]->get_pos()))
			return false;
	}
	return true;
}

//hammer 
void Game::p_pressed() {
	if (player.Is_Hold_A_Hammer()) { // check's if the 'p'key was pressed and player is holding a hammer 
		Point hammer_pos = { (player.get_pos().get_x() + player.get_dir().x) ,(player.get_pos().get_y() + player.get_dir().y) }; // the hammer pos is one step from the player in it's direction
		Point after_hammer_pos = { hammer_pos.get_x() + player.get_dir().x , hammer_pos.get_y() + player.get_dir().y };
		bool res = hammer_hit_enemy(hammer_pos) || hammer_hit_enemy(after_hammer_pos);// check's if the hammer is hittng one of the barrels (enemy), if so kill it
		if (res) {
			if (!isSilent) {
				hammer_pos.draw(b.hammer_ch);
				Sleep(50);
			}
			score += HAMMER_SCORE;
			b.draw_score(score);
		}
	}
}


//hammer 
bool Game::hammer_hit_enemy(Point hammer) { // check's if hammer hit enemy , if so kill the enemy and return true 
	for (int i = 0; i < NUM_OF_BARRELS; i++) { // goes threw all barrels
		if (barrels[i].get_is_alive()) { // if the curr barrel is alive
			if (barrels[i].get_pos() == hammer) { // if the barrel is in the same point as object
				barrels[i].end_barrel_life(); // then kill the barrel
				return true;
			}
		}
	}
	for (int i = 0; i < ghosts.size(); i++) { // goes threw all ghosts
		if (ghosts[i]->get_is_alive()) { // if the curr ghost is alive
			if (ghosts[i]->get_pos() == hammer) { // if the ghost is in the same point as object
				ghosts[i]->End_Life(); // then kill the ghost
				return true;
			}
		}
	}
	return false;
}
