#pragma once

#include <random>
#include <iostream>
#include "Enemy.h"

class Board; // forward declerasion

class Barrel : public Enemy
{
	static constexpr int max_fall = 8; // how many floors can fall until expload
	static constexpr int expload_lengh = 5; // explotion rate
	Point::Direction start_dir;
	int fall_counter = 0;
	bool IsFalling = false;
	bool IsExploded = false;

public:

	Barrel(char character = 'O', Board* _pb = nullptr, Point _pos = Point()) : Enemy(character, _pb, _pos) {
		is_alive = false;
		//start_dir = dir;
	}

	bool get_IsExploded() const {
		return IsExploded;
	}
	void barrel_movement();
	void fall();
	void end_barrel_life();
	bool barrel_explosion_hit_something(Point mario_p) const;
	void reset_obj() override;

};

