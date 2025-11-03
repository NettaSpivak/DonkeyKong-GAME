#pragma once
#include "Ghost.h"

class Climbed_Ghost : public Ghost
{
	static constexpr double prob_climbe_change = 0.15;

	bool is_climbing = false;

public:

	Climbed_Ghost(char character = 'X', Board* _pb = nullptr, Point _pos = Point()) : Ghost(character, _pb, _pos) {}


	void movement() override;
	void EndClimb();
	void OffTheLadder();
	void reset_obj() override {
		is_climbing = false;
	}

	bool Up() const { // if dir is up
		return dir.y == Point::directions[0].y;
	}

	bool Down() const { // if dir is down
		return dir.y == Point::directions[2].y;
	}

};

