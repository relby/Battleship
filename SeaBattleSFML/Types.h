#pragma once

const size_t WIDTH = 10;
const size_t HEIGHT = 10;

const size_t CELL_WIDTH = 50;
const size_t CELL_HEIGHT = 50;

struct Point {
	size_t x;
	size_t y;
};

enum Direction {
    Up,
    Down,
    Left,
    Right
};