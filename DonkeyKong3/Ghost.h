#pragma once

#include <random>
#include <iostream>
#include "Enemy.h"

class Board; // forward declerasion

class Ghost : public Enemy
{
	static constexpr double prob_dir_change = 0.05;

public:
	Ghost(char character = 'x', Board* _pb = nullptr, Point _pos = Point()) : Enemy(character, _pb, _pos) {
		is_alive = true;
		getFirstDir();
	}

	virtual void movement();
	bool generateDir(double prob);
	void reset_obj() override {
		getFirstDir();
	}
};

