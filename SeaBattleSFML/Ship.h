#pragma once

#include <iostream>
#include <vector>
#include "Types.h"
#include "Cell.h"

class Ship
{
private:
	bool horizontal;
	bool placed;
	bool selected;
	size_t size;
	std::vector<Cell> cells;
public:
	Ship(Point loc, size_t size);

	Point getLoc();
	size_t getSize();
	std::vector<Cell> getCells();
	bool isHorizontal();
	bool isPlaced();
	bool isSelected();

	bool move(Point newLoc);
	void flip();
	void select();
	void unselect();
	void place();
};

