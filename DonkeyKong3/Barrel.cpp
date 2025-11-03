#include "Barrel.h"

void Barrel::barrel_movement() {
	Point under(pos.get_x(), pos.get_y() + 1);// the point under mario pos
	if (IsExploded) {//if the barrel is after explotion
		end_barrel_life();
	}
	if (p_b->IsThereAFloorUnder(pos)) { // if there is floor under the barrel
		if (p_b->getChar(under) == Board::right_dir_floor) {
			set_dir({ 1,0 });
		}
		else if (p_b->getChar(under) == Board::left_dir_floor) {
			set_dir({ -1,0 });
		}
	}
	if (!p_b->IsThereAFloorUnder(pos) || IsFalling) {// if there is no floor or barrel during falling
		fall();
	}
	bool Is_Reach_End_Of_Board = p_b->ReachToEndOfBoard(pos, dir);
	pos.move(dir);
	if (!IsExploded && (Is_Reach_End_Of_Board || p_b->IsThereAFloor({ pos.get_x() + dir.x, pos.get_y() }))) {//if the barrel reach the end of the board
		end_barrel_life();
	}
}

void Barrel::fall() {
	if (!p_b->IsThereAFloorUnder(pos)) { // if there is no floor under barrel 
		IsFalling = true; // start falling
		set_dir({ dir.x, 1 });  // set falling dir according to prev movement
		fall_counter++; // count the fall floors
		Point above_next_move(pos.get_x() + dir.x, pos.get_y()); //the point that above barrel after movement in curr dir
		if (p_b->IsThereAFloorUnder(above_next_move)) { // if there is floor on next move - barrel get inside floor
			set_dir({ 0, 1 });  // set falling dir according so mario wand be in floor
		}
	}
	else {
		if (fall_counter >= max_fall) { // if reach floor and the fall counter is higher then max fall then barrel expload
			set_dir({ 0,0 });
			IsExploded = true;
		}
		else {
			getFirstDir(); // if not exploded then continue with a new dir
		}
		fall_counter = 0; // fall end and reset
		IsFalling = false;
	}
}

void Barrel::end_barrel_life() { // after explosion or reach to the end of board barrel eand her life at game until a wanted reset
	pos = start_pos;
	is_alive = false;
	IsExploded = false;
	dir = start_dir;
}


bool Barrel::barrel_explosion_hit_something(Point mario_p) const { // if a barrel is exploded checks if it hit mario
	Point point_to_compar = { pos.get_x(), pos.get_y() }; // get barrel position and check radius of 2
	point_to_compar.set_x(-2);
	point_to_compar.set_y(-2);
	for (size_t i = 0; i < 5; i++) {
		for (size_t j = 0; j < 5; j++) {
			if (point_to_compar == mario_p) {
				return true;
			}
			point_to_compar.set_x(1);
		}
		point_to_compar.set_x(-5);
		point_to_compar.set_y(1);
	}
	return false;
}

void Barrel::reset_obj() { // reset all the barrel to not on game and get back to original start pos
	fall_counter = 0;
	is_alive = false;
	IsFalling = false;
	IsExploded = false;
}