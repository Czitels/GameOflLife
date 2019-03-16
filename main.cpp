#include "board.hpp"

int main()
{
	Board GameOfLive(10,10);
	GameOfLive.setAlive(3);
	GameOfLive.run();
	return 0;
}