#include "Cell.h"

Cell::Cell() : loc(Point{ 0, 0 }), cursor(false), ship(false), hit(false) {}
Cell::Cell(Point loc, bool ship, bool cursor) : loc(loc), cursor(cursor), ship(ship), hit(false) {};

void Cell::setLoc(Point loc) {
	this->loc = loc;
}

void Cell::setCursor(bool isCursor) {
	this->cursor = isCursor;
}

void Cell::setShip(bool isShip) {
	this->ship = isShip;
}

void Cell::setHit(bool hit)
{
	this->hit = hit;
}

Point Cell::getLoc() {
	return this->loc;
}
bool Cell::isCursor() {
	return this->cursor;
}
bool Cell::isShip() {
	return this->ship;
}

bool Cell::isHit() {
	return this->hit;
}