#pragma once
#include "cell.hpp"
#include <vector>
class Board {
public:
	using mtrx = std::vector<std::vector<Cell>>;

	enum Condition
	{
		DEATH,
		ALIVE
	};

	Board(size_t width, size_t height);
	void run();
	void display();
	void check_condition();
	void setAlive(size_t count);
private:
	size_t width;
	size_t height;
	std::vector<Cell> cells;
	std::vector<size_t> alive_cells;
};