#pragma once

#include "Types.h"

class Cell
{
private:
	Point loc;
	bool cursor;
	bool ship;
	bool hit;
public:
	Cell();
	Cell(Point loc, bool ship = false, bool cursor = false);

	void setLoc(Point loc);
	void setCursor(bool cursor);
	void setShip(bool ship);
	void setHit(bool hit);

	Point getLoc();
	bool isCursor();
	bool isShip();
	bool isHit();
};

