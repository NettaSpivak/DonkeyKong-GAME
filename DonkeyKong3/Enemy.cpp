#include "Enemy.h"

void Enemy::getFirstDir() {
	if (std::rand() % 2 == 1) { // randomly chose start dir
		dir = { 1,0 };
	}
	else {
		dir = { -1,0 };
	}
}


bool Enemy::IsEnemyPrevPosHitPoint(Point p, Point p_prev) const {
	Point enemy_prev = Point((pos.get_x() - dir.x), (pos.get_y() - dir.y)); // prev pos of ghost before move
	if (p == enemy_prev && p_prev == pos) // if enemy is at the prev place of point and point is at prev place of enemy
		return true;
	else
		return false;
}

void Enemy::reset() {
	p_b->restore(pos); //get the original board back to where the char was
	pos = start_pos;
	is_alive = true;
	reset_obj();
}
