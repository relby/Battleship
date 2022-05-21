#include "Ship.h"

Ship::Ship(Point loc, size_t size) : horizontal(true), selected(false), placed(false), size(size), cells(std::vector<Cell>(size, Cell(loc, true)))
{
	for (size_t i = 0; i < size; i++) {
		if (this->horizontal) {
			cells[i].setLoc(Point{ loc.x + i, loc.y });
		}
		else {
			cells[i].setLoc(Point{ loc.x, loc.y + i });
		}
	}
}

Point Ship::getLoc()
{
	return this->cells[0].getLoc();
}

size_t Ship::getSize()
{
	return this->cells.size();
}

std::vector<Cell> Ship::getCells()
{
	return this->cells;
}

bool Ship::isHorizontal()
{
	return this->horizontal;
}

bool Ship::isPlaced()
{
	return this->placed;
}

bool Ship::isSelected()
{
	return this->selected;
}

bool Ship::move(Point newLoc)
{
	if (this->horizontal && this->size + newLoc.x > WIDTH) return false;
	if (!this->horizontal && this->size + newLoc.y > HEIGHT) return false;

	for (size_t i = 0; i < this->size; i++) {
		if (this->horizontal) {
			cells[i].setLoc(Point{ newLoc.x + i, newLoc.y });
		}
		else {
			cells[i].setLoc(Point{ newLoc.x, newLoc.y + i });
		}
	}
	return true;
}

void Ship::flip() {
	bool ok = true;
	Point shipLoc = this->getLoc();
	std::vector<Point> newLocs;
	for (Cell cell : cells) {
		Point cellLoc = cell.getLoc();
		size_t newY = shipLoc.y + cellLoc.x - shipLoc.x;
		size_t newX = shipLoc.x + cellLoc.y - shipLoc.y;
		if (newY >= HEIGHT || newX >= WIDTH) {
			ok = false;
			break;
		}
		newLocs.push_back(Point{ newX, newY });
	}
	if (!ok) return;
	for (size_t i = 0; i < this->size; i++) {
		cells[i].setLoc(newLocs[i]);
	}
	this->horizontal = !this->horizontal;
}

void Ship::select()
{
	this->selected = true;
}

void Ship::unselect()
{
	this->selected = false;
}

void Ship::place()
{
	this->placed = true;
	this->unselect();
}


