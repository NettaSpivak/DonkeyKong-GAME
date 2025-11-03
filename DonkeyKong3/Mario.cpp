#include "Mario.h"


bool Mario::IsValidKey(char key) const {
	Point under = Point(pos.get_x(), pos.get_y() + 1); // the point under mario pos
	Point under_under = Point(pos.get_x(), pos.get_y() + 2); // the point under the under mario pos
	if ((!p_b->IsThereAFloorUnder(pos) && !p_b->IsThereALadder(under)) || IsFalling) {// is mario in air 
		return false; // if mario in the air he cant move with keys at all 
	}
	if (p_b->IsThereALadder(under)) { // is mario on a ladder 
		if (key == LEFT || key == RIGHT) { // if mario is on ladder can move only up or down or stay
			//isclimb = true;
			return false;
		}
	}
	if (under_under.get_y() >= Board::Boardsize::MAX_Y || (p_b->IsThereAFloorUnder(pos) && !p_b->IsThereALadder(under_under))) {
		// if mario doesnt has a ladder under the floor that under him or if on first floor
		if (key == DOWN) { // if not mario cant move down
			return false;
		}
	}
	if (pos.get_y() == ZERO) {//the end of the board
		if (key == UP) { // mario cant jump
			return false;
		}
	}
	return true; // else
}


void Mario::EndClimb() {
	Point under(pos.get_x(), pos.get_y() + 1);// the point under mario pos
	Point above(pos.get_x(), pos.get_y() - 1); // the point above mario pos
	Point above_above(pos.get_x(), pos.get_y() - 2); // the point above mario pos
	if (p_b->IsThereAFloor(above) && p_b->IsThereAFloor(above_above))//if at the end of climbing mario cant finish movement because double floor case
		set_dir({ 0,0 });
	else if (p_b->IsThereALadder(under) && p_b->IsThereAFloor(pos)) { // if Mario is at the end of climbing a ladder.
		//If so changes the direction to stay {0,0} as soon as its position is above the floor.
		pos.set_y(-1);
		set_dir({ 0,0 });
		IsClimbing = false;
	}
}

void Mario::OffTheLadder() { // The function checks whether Mario has off a ladder 
	if (p_b->IsThereALadder(pos) && p_b->IsThereAFloorUnder(pos)) { //if mario position is above the floor)
		// changes the direction to stay {0,0};
		set_dir({ 0,0 });
		IsClimbing = false;
	}
}

// function that determent mario dir
bool Mario::Mario_Movement() {
	bool is_mario_alive = true;

	Point under = Point(pos.get_x(), pos.get_y() + 1);// the point under mario pos
	if (Up()) { // if the dir is up
		if ((IsClimbing || p_b->IsThereALadder(pos)) && !IsJumping) { //  if there is ladder and isnt jumping
			IsClimbing = true; // climb the ladder
			EndClimb(); // if reach to end stop the climbing
		}
		else { // up and no ladder
			IsJumping = true;
			jump(); // jump
		}
	}
	else { // not going up
		if (Down() && !IsFalling) { // if dir is down and mario during climbing
			if (!IsClimbing)
				IsClimbing = true;
			else
				OffTheLadder(); // if reach to end stop
		}

		if ((!p_b->IsThereAFloorUnder(pos) && !IsClimbing) || IsFalling) {// if fall or didnt have floor under and not climbing 	
			is_mario_alive = fall(); // if mario lost life after falling
		}

		if (p_b->IsThereAFloor({ pos.get_x() + dir.x, pos.get_y() })) { // if next move is floor only at x
			dir.x = ZERO;
		}
	}
	p_b->ReachToEndOfBoard(pos, dir);
	pos.move(dir); // move the point od mario on board 
	return is_mario_alive;
}

void Mario::jump() {
	Point above_next_move(pos.get_x() + prev_dir.x, pos.get_y() - 2); // the point above that mario goes to 
	if (jump_level < max_jump && !p_b->IsThereAFloorUnder(above_next_move)) { // if is during the lifting at the jump and not reaching floor from above
		set_dir({ prev_dir.x, dir.y }); // dir of jumping at the prev movement dir
		jump_level++; // add to the jump counter
	}
	else {
		IsJumping = false; // when reach to jumping hight of 2 then stop the jumping and fall
		jump_level = ZERO; // resert jump
		fall(); // fall
	}

}

bool Mario::fall() {
	if (dir.y != 1) {//mario didnt start fall yet 
		IsFalling = true; // start falling
		set_dir({ dir.x, 1 });  // set falling dir according to prev movement
	}
	if (!p_b->IsThereAFloorUnder(pos)) { // if there is no floor under mario 
		//IsFalling = true; // start falling
		//set_dir({ dir.x, 1 });  // set falling dir according to prev movement
		fall_counter++; // count the fall floors
		Point above_next_move(pos.get_x() + dir.x, pos.get_y()); //the point that above mario after movement in curr dir
		if (p_b->IsThereAFloorUnder(above_next_move)) { // if there is floor on next move - mario get inside floor
			set_prevdir(dir);
			set_dir({ 0, 1 });  // set falling dir according so mario wand be in floor
		}
	}
	else if (fall_counter >= max_fall) { // if rreach floor and the fall counter is higher then max fall then life count is -1 and mario restart
		set_dir({ 0,0 });
		IsFalling = false;
		return false; // mario lost life
	}
	else { // else, mario keep the movement on floor
		if (dir.x == 0 && prev_dir.x != 0) // if the moving dir changed during falling set to the prev dir
			set_dir({ prev_dir.x, 0 });
		else
			set_dir({ dir.x, 0 });
		fall_counter = ZERO; // fall end and reset
		IsFalling = false;
	}
	return true; // mario didnt fail 
}

void Mario::mario_restart() {
	if (pos != p_b->DEFAULT_POS)
		p_b->restore(pos); //get the original board back to where mario was
	pos = p_b->MARIO_START; //reset mario position
	IsHoldAHammer = false;
	p_b->restartHammer(); // get hammer back to board
	dir = { 0,0 }; // reset dir to stay
	fall_counter = ZERO; // mario fall count reset
	jump_level = ZERO; // mario jump count reset
	// update flags
	IsJumping = false;
	IsFalling = false;
	IsClimbing = false;
}

void Mario::take_life() { //if mario is dead then take one from life count 
	life_count--;
	if (p_b->get_isSilent())
		return;
	for (size_t i = 0; i < 5; i++) { //draw mario dead
		pos.draw(ch);
		Sleep(100);
		pos.draw(' ');
		Sleep(100);
	}
}

//hammer 
bool Mario::Grab_Hammer() {
	if (p_b->IsThereAHammer(pos)) {
		IsHoldAHammer = true;
		if (!p_b->get_isSilent()) {
			p_b->DeleteHammerFromBoard();
			p_b->restore(p_b->HAMMER_POS);
			p_b->HAMMER_CHECK_POS.draw('V');
		}
		return true;
	}
	return false;
}