#pragma once

#include <random>
#include <iostream>
#include "Board.h"


class Enemy
{
	char ch; // enemy character

	virtual void reset_obj() = 0;

protected:
	Board* p_b = nullptr;
	Point start_pos;
	Point pos;
	Point::Direction dir;// current direction: dir.x, dir.y
	bool is_alive;


public:
	Enemy(char character, Board* _pb = nullptr, Point _pos = Point(0, 0)) : ch(character), p_b(_pb), start_pos(_pos) {
		pos = start_pos;
	}
	void getFirstDir();

	Point get_pos() const {
		return pos;
	}
	char get_ch() const {
		return ch;
	}
	Point::Direction get_dir() const {
		return dir;
	}
	void set_dir(Point::Direction new_dir) {
		dir.x = new_dir.x;
		dir.y = new_dir.y;
	}
	bool get_is_alive() const {
		return is_alive;
	}
	void set_alive(bool new_life) {
		is_alive = new_life;
	}

	bool Is_Enemy_Hit_Player(Point player_p, Point player_p_prev) const { //checks if one of the barrels hit mario
		return (pos == player_p || IsEnemyPrevPosHitPoint(player_p, player_p_prev));
	}
	bool IsEnemyPrevPosHitPoint(Point p, Point p_prev) const;

	void End_Life() {
		pos = start_pos;
		is_alive = false;
	}
	void set_board_p(Board* b) {
		p_b = b;
	}

	void set_start_pos(Point pos) {
		start_pos = pos;
	}

	void reset();

};

