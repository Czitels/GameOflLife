#include "cell.hpp"
#include <vector>
#include <iostream>

size_t Cell::ID = 0;

Cell::Cell(size_t width, size_t height, size_t row, size_t column, bool state) :state(state), row(row), column(column){ 
	number = this->ID;
	++ID;
	set_neighborhood(width, height);
	newState = state;
	haveNeighborhood = false;
}
Cell::~Cell() {
	this->column = NULL;
	this->row = NULL;
	this->number = NULL;
	this->row = NULL;
	this->column = NULL;
	this->state = NULL;
	this->newState = NULL;
	this->haveNeighborhood = NULL;
	neighborhood.clear();
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

	
	if (number < width)
	{
		this->neighborhood.push_back(number + (width*(height - 1)));
		if (number%width != 0)
			this->neighborhood.push_back(number + (width*(height - 1)) - 1);
		if ((number + 1) % width != 0)
			this->neighborhood.push_back(number + (width*(height - 1)) + 1);
	}

	if (number >= width * (height - 1))
	{
		this->neighborhood.push_back(number - (width*(height - 1)));
		if (number%width != 0)
			this->neighborhood.push_back(number - (width*(height - 1)) - 1);
		if ((number + 1) % width != 0)
			this->neighborhood.push_back(number - (width*(height - 1)) + 1);
	}
}

const std::vector<size_t>& Cell::get_neighborhood() const noexcept { return this->neighborhood; }
const bool &Cell::isAlive() const { return this->state; }
const bool &Cell::isNewAlive() const noexcept{ return this->newState; }
const bool &Cell::getHaveNeighborhood() const noexcept{ return this->haveNeighborhood; }
void Cell::setNewState(const bool &state) noexcept{ this->newState = state; }
void Cell::setState(const bool &state) noexcept{ this->state = state; }
void Cell::setHaveNeighborhood(const bool &hNeigh) noexcept{ this->haveNeighborhood = hNeigh; }
void Cell::updateState() { this->state = this->newState; }
void Cell::destroyID() { ID = 0; }