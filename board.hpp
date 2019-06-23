#pragma once
#include "cell.hpp"
#include <vector>
#include <memory>
#include <experimental\filesystem>
#include <SFML\System\Time.hpp>
#include <SFML/Graphics/RenderWindow.hpp>

namespace fs = std::experimental::filesystem;

class Memento;

class Board {
public:
	enum Condition
	{
		DEATH,
		ALIVE
	};

	Board(size_t width, size_t height);
	~Board();
	bool run(std::vector<std::string>);
	void display(sf::RectangleShape&, sf::RectangleShape&, sf::RenderWindow&);
	void displayByPopulation();
	void check_condition();
	void setAliveByMouse();
	void setAlive(); // set cells to being alive
	void killAll();
	std::string getStartingPattern();
	std::string encodeRLE();
	void decodeRLE(const std::string&);
	void decodeBoardRLE(const std::string&);
	void loadPatternFromFile(const fs::path& path);
	std::vector<std::string> listPatternBox();
	const float &getFPS(const sf::Time& time);
	size_t getWidth();
	size_t getHeight();
	void ShowMainMenu(sf::RenderWindow &, bool &);
private:
	size_t width;
	size_t height;
	std::string startingPattern;
	std::vector<Cell> cells;
	std::vector<bool> population;

};