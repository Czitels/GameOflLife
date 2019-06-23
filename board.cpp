#include <SFML/Graphics.hpp>
#include "board.hpp"
#include "cell.hpp"
#include <vector>
#include <iostream>
#include <chrono>
#include <thread>
#include <experimental\filesystem>
#include "imgui.h"
#include "imgui-SFML.h"
#include <memory>
#include <execution>
#include <fstream>
#include <algorithm>
#include "Originator.h"
#include "Caretaker.h"
#include <mutex>
#include <thread>
#include <windows.h>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

#define LIMIT_FOR_ITERATION 500 //in bytes
#define LIST_DESTROYER 10000

using namespace std::this_thread;     // sleep_for, sleep_until
using namespace std::chrono_literals; // ns, us, ms, s, h, etc.
namespace fs = std::experimental::filesystem;
namespace ImGui
{
	static auto vector_getter = [](void* vec, int idx, const char** out_text)
	{
		auto& vector = *static_cast<std::vector<std::string>*>(vec);
		if (idx < 0 || idx >= static_cast<int>(vector.size())) { return false; }
		*out_text = vector.at(idx).c_str();
		return true;
	};

	bool Combo(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return Combo(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

	bool ListBox(const char* label, int* currIndex, std::vector<std::string>& values)
	{
		if (values.empty()) { return false; }
		return ListBox(label, currIndex, vector_getter,
			static_cast<void*>(&values), values.size());
	}

}

Board::Board(size_t width, size_t height) :width(width), height(height) {
	size_t x = 800/width;
	size_t y = 800/height;

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
			population.push_back(0); 
		}
	}
}
Board::~Board() {
	this->cells.clear();
	this->population.clear();
	startingPattern.clear();

}
bool Board::run(std::vector<std::string> listbox_items)
{
	bool isEnd = false;
	std::cout << std::thread::hardware_concurrency() << " concurrent threads are supported.\n";
	size_t flag = 1;
	unsigned long long int amountOfIterations;
	static int listbox_item_current = 0;
	float fps;
	float myfps=150;
	auto originator = std::make_unique<Originator>();
	auto caretaker = std::make_unique<Caretaker>();
	sf::RectangleShape white;
	sf::RectangleShape black;
	sf::RenderWindow window(sf::VideoMode(1200, 800), "Game of live");
	sf::Clock deltaClock;
	sf::Clock FPSClock;
	///////
	MEMORYSTATUSEX statex;

	statex.dwLength = sizeof(statex);

	GlobalMemoryStatusEx(&statex);

	std::cout << statex.ullAvailPhys << "\n"; // information about free memory (KB)
	amountOfIterations = 1024 * statex.ullAvailPhys / LIMIT_FOR_ITERATION; // we need to convert KB to B so we multiply it by 1024

	///////
	ImGui::SFML::Init(window);
	window.resetGLStates();

	size_t x = 800 / width;
	size_t y = 800 / height;
	white.setSize(sf::Vector2f(x, y));
	white.setFillColor(sf::Color::White);
	white.setOutlineThickness(2.f);
	white.setOutlineColor(sf::Color::Black);

	black.setSize(sf::Vector2f(x, y));
	black.setFillColor(sf::Color::Black);
	black.setOutlineThickness(2.f);
	black.setOutlineColor(sf::Color::Black);

	while (window.isOpen())
	{
		sf::Event event;
		while (window.pollEvent(event))
		{
			ImGui::SFML::ProcessEvent(event);
			if (event.type == sf::Event::Closed)
				window.close();
		}
		ImGui::SFML::Update(window, deltaClock.restart());

		ImGui::Begin("Options: ");

		if (ImGui::BeginMenu("Main Menu"))
		{
			ShowMainMenu(window, isEnd);
			ImGui::EndMenu();
		}

		if (ImGui::Button("Play")) {
			flag = 1;
		}
		else if (ImGui::Button("Stop")) {
			flag = 0;
		}
		else if (ImGui::Button("Next iteration")) {
			flag = 2;
		}
		else if (ImGui::Button("Previous iteration")) {
			originator->restoreToMemento(caretaker->getMemento());
			flag = 3;
		}
		else if (ImGui::Button("Reset")) {
			flag = 4;
		}

		ImGui::ListBox("listbox\n(single select)", &listbox_item_current, listbox_items);
		ImGui::InputFloat("Set FPS", &myfps);
		window.clear();

		if (flag == 1)
		{
			--amountOfIterations;
			Board::check_condition();
			Board::display(white, black, window);
			originator->setRLE(encodeRLE());
			caretaker->setMemento(originator->createMemento());
		}
		else if (flag == 2) {
			--amountOfIterations;
			flag = 0;
			Board::check_condition();
			Board::display(white, black, window);
			originator->setRLE(encodeRLE());
			caretaker->setMemento(originator->createMemento());
		}
		else if (flag == 3) {
			++amountOfIterations;
			flag = 0;
			Board::decodeBoardRLE(originator->getRLE());
			Board::setAlive();
			Board::displayByPopulation();
			Board::check_condition();
			Board::display(white, black, window);
		}
		else if (flag == 4) {
			std::string choosenPattern = {};
			choosenPattern += R"(C:\Users\bitel\Documents\Visual Studio 2017\Projects\GameOfLife\GameOfLife\Patterns\)";
			choosenPattern += listbox_items[listbox_item_current];
			choosenPattern += ".rle";

			flag = 0;
			Board::killAll();
			Board::loadPatternFromFile(choosenPattern);
			Board::decodeRLE(getStartingPattern());
			Board::setAlive();
			Board::check_condition();
			Board::display(white, black, window);
			caretaker->clearList();
			amountOfIterations = 1024 * statex.ullAvailPhys / LIMIT_FOR_ITERATION;
		}
		else if (flag == 0) {
			display(white, black, window);
		}

		fps = getFPS(FPSClock.restart());
		window.setFramerateLimit(myfps);

		ImGui::End(); // end window

		ImGui::Begin("FPS");

		if(fps>=45)
			ImGui::TextColored(sf::Color::Green,"%f", fps);
		else if(fps>=20&&fps<45)
			ImGui::TextColored(sf::Color::Yellow, "%f", fps);
		else
			ImGui::TextColored(sf::Color::Red, "%f", fps);
		
		ImGui::End();

		ImGui::Begin("Iterations");

		if (amountOfIterations >= 1000)
			ImGui::TextColored(sf::Color::White, "%u", amountOfIterations);
		else if(amountOfIterations < 1000 && amountOfIterations > 1)
			ImGui::TextColored(sf::Color::Red, "%u", amountOfIterations);
		else {
			ImGui::TextColored(sf::Color::Red, "%s", "Destroying first 10k iterations");
			caretaker->eraseList(LIST_DESTROYER);
			amountOfIterations += LIST_DESTROYER;
		}

		ImGui::End();

		ImGui::SFML::Render(window);
		window.display();
		sleep_for(50ns);
	}
	return isEnd;
}
void Board::display(sf::RectangleShape& white, sf::RectangleShape& black, sf::RenderWindow& window) {
	std::for_each(std::execution::seq, cells.begin(), cells.end(), [&](Cell& x) {
		switch (x.isNewAlive())
		{
		case DEATH: {
			white.setPosition(x.getPosition());
			window.draw(white);
			break;
		}
		case ALIVE: {
			black.setPosition(x.getPosition());
			window.draw(black);
			break;
		}
		}
		x.updateState();
	});
}
void Board::displayByPopulation() {
	for (size_t i = 0; i < population.size(); i++)
	{
		switch (population[i])
		{
		case DEATH: {
			cells[i].setFillColor(sf::Color::White);
			cells[i].setOutlineColor(sf::Color::Black); 
		}
		case ALIVE: {
			cells[i].setFillColor(sf::Color::Black);
			break;
		}
		}
		cells[i].setState(population[i]);
		cells[i].setNewState(population[i]);
	}
}
void Board::check_condition()
{
	thread_local int alives = 0;
	std::for_each(std::execution::par, cells.begin(), cells.end(), [&](Cell& x) {
		for (auto &z : x.get_neighborhood()) {
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
	});
}
void Board::setAlive() {
	for (size_t i = 0; i < population.size(); i++)
	{
		if (population[i] == ALIVE) {
			cells[i].setState(population[i]);
			cells[i].setFillColor(sf::Color::Black);
		}
	}
}
void Board::killAll() {
	for (size_t i = 0; i < cells.size(); i++)
	{
		population[i] = DEATH;
		cells[i].setState(DEATH);
		cells[i].setNewState(DEATH);
	}
}
std::string Board::getStartingPattern() {
	return this->startingPattern;
}
std::string Board::encodeRLE() {
	std::string sRLE = {};
	size_t counter=0;
	bool check = cells[0].isAlive();
		for (auto &x : cells) {
		if (!x.isAlive()) {
			if (check == ALIVE) {
				if (counter > 1) {
					sRLE += std::to_string(counter);
				}
				sRLE += 'o';
				check = DEATH;
				counter = 0;
			}
			++counter;
		}
		else {
			if (check == DEATH) {
				if (counter > 1) {
					sRLE += std::to_string(counter);
				}
				sRLE += 'b';
				check = ALIVE;
				counter = 0;
			}
			++counter;
		}
	}
	sRLE += std::to_string(counter);
	sRLE += 'b';
	sRLE += '!';
	return sRLE;
}
void Board::decodeRLE(const std::string& position) {
	size_t start = 5 + 2 * width;//2 and 2* - becaouse we want have starting point in (3,3)
	size_t inumbOfCells = 1;
	std::string snumbOfCells = {};
	std::string::const_iterator it = position.cbegin();
	std::vector<bool>::iterator startPos = population.begin()+start;
	
	for (it; *it != '!'; ++it){
		if (*it == 'b') {
			if (!snumbOfCells.empty()) {
				inumbOfCells = std::stoi(snumbOfCells);

				for (size_t i = 0; i < inumbOfCells; i++)
				{
					*startPos = DEATH;
					++startPos;
				}
				inumbOfCells = 1;
				snumbOfCells.clear();
			}
			else{
				*startPos = DEATH;
				++startPos;
			}
		}
		else if (*it == 'o') {
			if (!snumbOfCells.empty()) {
				inumbOfCells = std::stoi(snumbOfCells);

				for (size_t i = 0; i < inumbOfCells; i++)
				{
					*startPos = ALIVE;
					++startPos;
				}
				inumbOfCells = 1;
				snumbOfCells.clear();
			}
			else {
				*startPos = ALIVE;
				++startPos;
			}
		}
		else if (*it == '$') {
			if (!snumbOfCells.empty()) {
				inumbOfCells = std::stoi(snumbOfCells);

				start += (width*inumbOfCells); //it is important to multiply becaouse we can have also a few empty lines
				startPos = population.begin() + start;

				inumbOfCells = 1;
				snumbOfCells.clear();
			}
			else {
				start += (width);
				startPos = population.begin() + start;
			}
		}
		else {
			snumbOfCells.push_back(*it);
		}
	}
}
void Board::decodeBoardRLE(const std::string& position) {
	size_t inumbOfCells = 1;
	std::string snumbOfCells = {};

	std::string::const_iterator it = position.cbegin();
	std::vector<bool>::iterator startPos = population.begin();

	for (it; *it != '!'; ++it) {
		if (*it == 'b') {
			if (!snumbOfCells.empty()) {
				inumbOfCells = std::stoi(snumbOfCells);

				for (size_t i = 0; i < inumbOfCells; i++)
				{
					*startPos = DEATH;
					++startPos;
				}
				inumbOfCells = 1;
				snumbOfCells.clear();
			}
			else {
				*startPos = DEATH;
				++startPos;
			}
		}
		else if (*it == 'o') {
			if (!snumbOfCells.empty()) {
				inumbOfCells = std::stoi(snumbOfCells);

				for (size_t i = 0; i < inumbOfCells; i++)
				{
					*startPos = ALIVE;
					++startPos;
				}
				inumbOfCells = 1;
				snumbOfCells.clear();
			}
			else {
				*startPos = ALIVE;
				++startPos;
			}
		}
		else {
			snumbOfCells.push_back(*it);
		}
	}
}
void Board::loadPatternFromFile(const fs::path& path) {
	if (fs::exists(path)) {
		std::string line;
		startingPattern.clear();
		std::ifstream myRLEFile(path);
		while (getline(myRLEFile, line)) {
			if (line[0] != '#'&&line[0] != 'x') {
				this->startingPattern += line;
			}
		}
	}
}std::vector<std::string> Board::listPatternBox() {
	std::vector<std::string> vecOfPatterns;
	for (auto& p : fs::directory_iterator(R"(C:\Users\bitel\Documents\Visual Studio 2017\Projects\GameOfLife\GameOfLife\Patterns)")){
		fs::path p1(p.path());
		vecOfPatterns.push_back(p1.stem().string());
	}
	return vecOfPatterns;
}
const float &Board::getFPS(const sf::Time& time) {
	return (1000000.0f / time.asMicroseconds());
}
void Board::ShowMainMenu(sf::RenderWindow &window, bool &isEnd) {
	ImGui::MenuItem("Main menu", NULL, false, false);
	if (ImGui::BeginMenu("New Game")) {
		static int newWidth = 30;
		static int newHeight = 30;
		ImGui::InputInt("width", &newWidth, 10);
		ImGui::InputInt("height", &newHeight, 10);
		
		if (ImGui::MenuItem("Start!")) {
			this->width = newWidth;
			this->height = newHeight;
			window.close();
			isEnd = false;
		}
		ImGui::EndMenu();
	}
	if (ImGui::MenuItem("Quit", "Alt+F4")) { 
		window.close();
		isEnd = true;
	}

}
size_t Board::getWidth() { return this->width; }
size_t Board::getHeight() { return this->height; }