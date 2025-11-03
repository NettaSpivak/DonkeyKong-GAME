#include "Climbed_Ghost.h"

void Climbed_Ghost::movement() {
	if (p_b->IsThereALadder(pos) && generateDir(prob_climbe_change)) {
		is_climbing = true;
		dir = { 0,-1 };
	}
	Point under = Point(pos.get_x(), pos.get_y() + 1); // the point under the ghost pos
	Point under_under = Point(pos.get_x(), pos.get_y() + 2); // the point under the under ghost pos
	if ((p_b->IsThereALadder(under_under) || p_b->IsThereALadder(under)) && generateDir(prob_climbe_change)) { 
		dir = { 0,1 };
		is_climbing = true;
	}
	if (Up()) { // going up
		EndClimb();
	}
	if (Down()) { // going down
		OffTheLadder();
	}
	if (!is_climbing) {
		Ghost::movement();
	}
	else {
		pos.move(dir);
	}
}



void Climbed_Ghost::EndClimb() {
	Point under(pos.get_x(), pos.get_y() + 1);// the point under pos
	Point above(pos.get_x(), pos.get_y() - 1); // the point above pos
	Point above_above(pos.get_x(), pos.get_y() - 2); // the point above pos
	if (p_b->IsThereAFloor(above) && p_b->IsThereAFloor(above_above))//if at the end of climbing ghost cant finish movement because double floor case
		set_dir({ 1,0 });
	else if (p_b->IsThereALadder(under) && p_b->IsThereAFloor(pos)) { // if ghost is at the end of climbing a ladder.
		//If so changes the direction to random left or right as soon as its position is above the floor.
		pos.set_y(-1);
		getFirstDir();
		is_climbing = false;
	}
}

void Climbed_Ghost::OffTheLadder() { // The function checks whether ghost has off a ladder 
	if (p_b->IsThereALadder(pos) && p_b->IsThereAFloorUnder(pos)) { //if position is above the floor)
		// changes the direction randomly left or right
		getFirstDir();
		is_climbing = false;
	}
}