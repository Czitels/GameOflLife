#include <SFML/Graphics.hpp>
#include "board.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.

Board::Board(size_t width, size_t height) :width(width), height(height) {
	size_t x = 40;
	size_t y = 40;

	for (size_t i = 0; i < height; i++)
	{
		for (size_t j = 0; j < width; j++)
		{
			Cell tmp(width, height, i, j, DEATH);
			tmp.setSize(sf::Vector2f(x, y));
			tmp.setFillColor(sf::Color::White);
			tmp.setPosition(sf::Vector2f(x*j + 1, y * i + 1));
			tmp.setOutlineThickness(2.f);
			tmp.setOutlineColor(sf::Color::Black);
			cells.push_back(tmp);
		}
	}
}

void Board::run()
{
	sf::RenderWindow window(sf::VideoMode(400, 400), "Game of live");

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			if (event.type == sf::Event::Closed)
				window.close();
		}

		window.clear();
		for (auto &x : cells) {
			window.draw(x);
		}
		window.display();
		Board::check_condition();
		Board::display(); //TODO: create board namespace !

		sleep_for(1s);
	}
}

void Board::display() {
	for (auto &x : cells) {
		switch (x.isNewAlive())
		{
		case DEATH: {
			x.setFillColor(sf::Color::White);
			x.setOutlineThickness(2.f);
			x.setOutlineColor(sf::Color::Black);
			break;
		}
		case ALIVE: {
			x.setFillColor(sf::Color::Black);
			break;
		}
		default:
			break;
		}
		x.updateState();
	}
}

void Board::check_condition()
{
	std::vector<int> neighborhood;
	size_t alives=0;
	for(auto &x:cells){
		neighborhood = x.get_neighborhood();
		for (auto &z : neighborhood) {
			if (cells[z].isAlive()) {
				++alives;
			}
		}
		if (x.isAlive()) {
			if (alives < 2 || alives > 3)
				x.setNewState(DEATH);
			else
				x.setNewState(ALIVE);
		}
		else {
			if (alives == 3)
				x.setNewState(ALIVE);
		}
		alives = 0;
	}
}

void Board::setAlive(size_t count) {
	size_t squaresID;
	for (size_t i = 0; i < count; i++)
	{
		std::cin >> squaresID;
		cells[squaresID].setState(ALIVE);
		cells[squaresID].setFillColor(sf::Color::Black);
	}
}