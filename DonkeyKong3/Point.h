#pragma once
#include "Global.h"

class Point
{
	int x, y;

	void draw_private(const char c) const { // generic draw char to the point on screen
		gotoxy();
		std::cout << c;
	}


public:

	struct Direction { int x, y; }; // inner struct
	static constexpr Direction directions[] = { {0, -1}, {-1, 0}, {0, 1}, {1, 0}, {0, 0} }; // directions options for example {0,-1} = up
	bool operator==(const Direction& other) const { // checks if 2 points are equal
		return (x == other.x && y == other.y);
	}

	constexpr Point(int _x = 0, int _y = 0) :x(_x), y(_y) {} // constractor, with an default option

	bool operator==(const Point& other) const { // checks if 2 points are equal
		return (x == other.x && y == other.y);
	}

	void gotoxy() const;

	void draw(const char ch) const { // draw at point
		draw_private(ch);
	}
	int get_x() const {
		return x;
	}
	int get_y() const {
		return y;
	}
	void set_x(int dir_x) {
		x += dir_x;
	}
	void set_y(int dir_y) {
		y += dir_y;
	}

	void move(Direction& dir); // moves point according to direction

};
