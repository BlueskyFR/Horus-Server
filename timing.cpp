#include "Structures.h"
#include <SFML/Graphics.hpp>

void waitForNextFrame(int &framesElapsed, Clock &clock)
{
	framesElapsed++;

	while (clock.getElapsedTime().asMilliseconds() < framesElapsed * 1000 / FPS)
	{
		sleep(milliseconds(1));
	}

}


void initRandom() {
	srand(time(NULL));
}