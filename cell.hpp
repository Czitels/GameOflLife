#pragma once
#include <SFML\Graphics\RectangleShape.hpp>
class Cell: public sf::RectangleShape
{
public:
	Cell(size_t width, size_t height, size_t row, size_t column, bool state);
	void set_neighborhood(size_t width, size_t height);
	const std::vector<int> get_neighborhood();
	bool isAlive();
	bool isNewAlive();
	void setNewState(bool state);
	void setState(bool state);
	void updateState();
private:
	static size_t ID;
	size_t number;
	size_t row;
	size_t column;
	bool state;
	bool newState;
	std::vector<int> neighborhood;
};