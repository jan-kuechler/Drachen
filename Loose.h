#ifndef LOOSE_H
#define LOOSE_H

#include "Button.h"
#include "State.h"

class Loose
{
	RenderWindow& window;

	Sprite background;

	bool running;

public:
	Loose(RenderWindow& Loose);

	void Reset();
	void Run();

	bool IsRunning()
	{
		return running;
	}

	State GetNextState()
	{
		return ST_MAIN_MENU;
	}
};

#endif //LOOSE_H
