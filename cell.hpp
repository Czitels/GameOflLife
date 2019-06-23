#pragma once
#include <SFML\Graphics\RectangleShape.hpp>
class Cell: public sf::RectangleShape
{
public:
	Cell(size_t width, size_t height, size_t row, size_t column, bool state);
	~Cell();
	void set_neighborhood(size_t width, size_t height);
	const std::vector<size_t>& get_neighborhood() const noexcept;
	const bool &isAlive() const;
	const bool &isNewAlive() const noexcept;
	void setNewState(const bool &state) noexcept;
	void setState(const bool &state) noexcept;
	void setHaveNeighborhood(const bool &hNeigh) noexcept;
	const bool &getHaveNeighborhood() const noexcept;
	void updateState();
	static void destroyID();
private:
	static size_t ID;
	size_t number;
	size_t row;
	size_t column;
	bool state;
	bool newState;
	bool haveNeighborhood; // it is using only for optimalization, to avoid looping over only death cell neighborhood vector
	std::vector<size_t> neighborhood;
};