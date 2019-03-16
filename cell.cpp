#include "cell.hpp"
#include <vector>
#include <iostream>

size_t Cell::ID = 0;

Cell::Cell(size_t width, size_t height, size_t row, size_t column, bool state) :state(state), row(row), column(column){ 
	number = this->ID;
	++ID;
	set_neighborhood(width, height);
	newState = state;
}

void Cell::set_neighborhood(size_t width, size_t height) {
	if (row == 0 && column == 0) {
		neighborhood.push_back(number + 1);
		neighborhood.push_back(number + width);
		neighborhood.push_back(number + (width + 1));

	}
	else if (row == 0 && column == width-1) { //-1 becaouse we iterate from 0
		neighborhood.push_back(number - 1);
		neighborhood.push_back(number + (width-1));
		neighborhood.push_back(number + width);
	}
	else if (row == height - 1 && column == 0) {
		neighborhood.push_back(number - width);
		neighborhood.push_back(number - (width-1));
		neighborhood.push_back(number + 1);
	}
	else if (row == height - 1 && column == width-1) {
		neighborhood.push_back(number - (width+1));
		neighborhood.push_back(number - width);
		neighborhood.push_back(number - 1);
	}
	else if (row == 0) {
		neighborhood.push_back(number - 1);
		neighborhood.push_back(number + 1);
		neighborhood.push_back(number + (width - 1));
		neighborhood.push_back(number + width);
		neighborhood.push_back(number + (width + 1));
	}
	else if (column == 0) {
		neighborhood.push_back(number - width);
		neighborhood.push_back(number - (width - 1));
		neighborhood.push_back(number + 1);
		neighborhood.push_back(number + width);
		neighborhood.push_back(number + (width + 1));
	}
	else if (column == height-1) {
		neighborhood.push_back(number - (width + 1));
		neighborhood.push_back(number - width);
		neighborhood.push_back(number - 1);
		neighborhood.push_back(number + (width - 1));
		neighborhood.push_back(number + width);
	}
	else if (row == width - 1) {
		neighborhood.push_back(number - (width + 1));
		neighborhood.push_back(number - width);
		neighborhood.push_back(number - (width - 1));
		neighborhood.push_back(number - 1);
		neighborhood.push_back(number + 1);
	}
	else {
		neighborhood.push_back(number - (width + 1));
		neighborhood.push_back(number - width);
		neighborhood.push_back(number - (width - 1));
		neighborhood.push_back(number - 1);
		neighborhood.push_back(number + 1);
		neighborhood.push_back(number + (width - 1));
		neighborhood.push_back(number + width);
		neighborhood.push_back(number + (width + 1));
	}
}

const std::vector<int> Cell::get_neighborhood() { return this->neighborhood; }
bool Cell::isAlive() { return this->state; }
bool Cell::isNewAlive() { return this->newState; }
void Cell::setNewState(bool state) { this->newState = state; }
void Cell::setState(bool state) { this->state = state; }
void Cell::updateState() { this->state = this->newState; }