#include "Ghost.h"

void Ghost::movement() {
	Point::Direction tmp = { 0,0 };
	if (generateDir(prob_dir_change) || p_b->ReachToEndOfBoard(pos, tmp) || p_b->IsThereAFloor({ pos.get_x() + dir.x, pos.get_y() })) // if randomly change dir or ghost reach end of board then cahnge dir
		dir = { -dir.x, dir.y }; // oposite dir
	Point next_pos = { pos.get_x() + dir.x, pos.get_y() };
	if (!p_b->IsThereAFloorUnder(next_pos)) {
		dir = { -dir.x, dir.y }; // oposite dir
		next_pos = { pos.get_x() + dir.x, pos.get_y() };
		if (!p_b->IsThereAFloorUnder(next_pos)) // edge case only one floor
			dir = { 0,0 };
	}
	pos.move(dir);
}

bool Ghost::generateDir(double prob) {
	return (static_cast<double>(std::rand()) / RAND_MAX < prob);
}
