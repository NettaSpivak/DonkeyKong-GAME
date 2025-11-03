#pragma once
#include "Board.h"

class Board; // forward declerasion

class Mario
{
	char ch; // mario character
	Point pos; // current position
	Point::Direction dir{ 0, 0 }; // current direction: dir.x, dir.y
	Point::Direction prev_dir{ 0,0 };// save the previous key direction
	Board* p_b;
	const int Life_Max;
	int life_count;	// mario life count
	int fall_counter = ZERO;
	static constexpr int max_jump = 2; // jump hight
	static constexpr int max_fall = 5; // how many floors can fall until dead
	int jump_level = ZERO;
	bool IsJumping = false;
	bool IsFalling = false;
	bool IsClimbing = false;
	bool IsHoldAHammer = false;

	const enum KEYS { UP = 'w', LEFT = 'a', DOWN = 'x', RIGHT = 'd', STAY = 's' }; // enum Keys

public:
	Mario(Board* _pb, int life) :p_b(_pb), Life_Max(life) {
		ch = p_b->mario_ch;
		pos = p_b->MARIO_START;
		life_count = Life_Max;
	}

	Point get_pos() const {
		return pos;
	}
	char get_ch() const {
		return ch;
	}
	Point::Direction get_dir() const {
		return dir;
	}
	Point::Direction get_prevdir() const {
		return prev_dir;
	}
	int get_life_count() const {
		return life_count;
	}
	void reset_life_count() {
		life_count = Life_Max;
	}
	void set_dir(Point::Direction new_dir) {
		dir.x = new_dir.x;
		dir.y = new_dir.y;
	}
	void set_prevdir(Point::Direction new_dir) {
		prev_dir.x = new_dir.x;
		prev_dir.y = new_dir.y;
	}

	bool Up() const { // if mario dir is up
		return dir.y == Point::directions[0].y;
	}

	bool Down() const { // if mario dir is down
		return dir.y == Point::directions[2].y;
	}
	bool Is_Hold_A_Hammer() const {
		return IsHoldAHammer;
	}

	void set_Is_Hold_A_Hammer() {
		IsHoldAHammer = !IsHoldAHammer;
	}
	void mario_restart();
	bool IsValidKey(char key) const; // checks if the movement at the key direction is valid
	void EndClimb();// when move up a ladder helper to get end the climb and get on floor 
	void OffTheLadder(); //when move down a ladder helper to get off  
	bool Mario_Movement();
	void jump();
	bool fall();
	void take_life();
	bool Grab_Hammer();
};

