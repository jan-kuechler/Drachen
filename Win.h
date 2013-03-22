#ifndef WIN_H
#define WIN_H

#include "Button.h"
#include "State.h"

class Win
{
	RenderWindow& window;

	Sprite background;

	bool running;

public:
	Win(RenderWindow& win);

	void Reset();
	void Run();

	bool IsRunning()
	{
		return running;
	}

	State GetNextState()
	{
		return ST_LEVEL_PICKER;
	}
};

#endif //WIN_H
