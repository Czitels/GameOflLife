#include "board.hpp"
#include "settings.h"
#include "imgui.h"
#include "imgui-SFML.h"
#include <iostream>
#include <algorithm>
#include <chrono>
#include <SFML/Graphics/RenderWindow.hpp>
#include <SFML/System/Clock.hpp>
#include <SFML/Window/Event.hpp>

void start();

int main()
{	start();
	// Create the main window
	std::chrono::time_point<std::chrono::system_clock> start, end;

	/*start = std::chrono::system_clock::now();
	end = std::chrono::system_clock::now();

	std::chrono::duration<double> elapsed_seconds = end - start;
	std::time_t end_time = std::chrono::system_clock::to_time_t(end);

	std::cout << "finished computation at " << std::ctime(&end_time)
		<< "elapsed time: " << elapsed_seconds.count() << "s\n";*/
	/*sf::RenderWindow window(sf::VideoMode(800, 800), "SFML window");
	const size_t width = 170;
	const size_t height = 170;

	start = std::chrono::system_clock::now();
	sf::VertexArray piramida;
	piramida.setPrimitiveType(sf::Quads);
	piramida.resize(width*height * 4);

	for (unsigned int i = 0; i < width; ++i)
		for (unsigned int j = 0; j < height; ++j)
		{
			// get a pointer to the current tile's quad
			sf::Vertex* quad = &piramida[(i + j * width) * 4];

			// define its 4 corners
			quad[0].position = sf::Vector2f(i * 16, j * 16);
			quad[1].position = sf::Vector2f((i + 1) * 16, j * 16);
			quad[2].position = sf::Vector2f((i + 1) * 16, (j + 1) * 16);
			quad[3].position = sf::Vector2f(i * 16, (j + 1) * 16);

			// define its 4 texture coordinates
			quad[0].color = sf::Color::White;
			quad[1].color = sf::Color::White;
			quad[2].color = sf::Color::White;
			quad[3].color = sf::Color::White;
		}

	end = std::chrono::system_clock::now();
	std::chrono::duration<double> elapsed_seconds = end - start;

	std::cout << "elapsed timexcxcxc: " << elapsed_seconds.count() << "s\n"; 
	// Start the game loop
	while (window.isOpen())
	{
		// Process events
		sf::Event event;
		while (window.pollEvent(event))
		{
			// Close window : exit
			if (event.type == sf::Event::Closed)
				window.close();
		}

		// Clear screen
		window.clear();
		start = std::chrono::system_clock::now();
		window.draw(piramida);
		end = std::chrono::system_clock::now();
		elapsed_seconds = end - start;
		std::cout << "elapsed time: " << elapsed_seconds.count() << "s\n";

		// Update the window

		window.display();
	}*/
	system("pause");
	return 0;
}

void start() {
	bool isEnd = false;
	size_t NewWidth = 30;
	size_t NewHeight = 30;
	Cell idDestroyer(20, 20, NULL, NULL, NULL);
	idDestroyer.destroyID(); //idDestroyer is only for destroying ID so it isn't necessary to have his ID
	while (!isEnd) {
		Board GameOfLive(NewWidth, NewHeight);
		GameOfLive.loadPatternFromFile(R"(C:\Users\bitel\Documents\Visual Studio 2017\Projects\GameOfLife\GameOfLife\Patterns\glider.rle)");
		GameOfLive.decodeRLE(GameOfLive.getStartingPattern());
		GameOfLive.setAlive();
		isEnd = GameOfLive.run(GameOfLive.listPatternBox());
		NewWidth = GameOfLive.getWidth();
		NewHeight = GameOfLive.getHeight();
		idDestroyer.destroyID(); // really necesery functionality, becaouse ID is static and while we make NEW GAME and destroy all objects
								//ID isn't destroied so we need to make it manually
	}
}