#include "Point.h"


void Point::gotoxy() const {
    std::cout.flush();
    COORD coord; // Create a COORD structure 
    coord.X = x; // Set the X coordinate 
    coord.Y = y; // Set the Y coordinate 
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord); // Move the cursor 
}


void Point::move(Direction& dir) { // move the point according to given direction
    set_x(dir.x);
    set_y(dir.y);
}
