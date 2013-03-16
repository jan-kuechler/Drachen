#ifndef MAIN_MENU_H
#define MAIN_MENU_H

#include "Button.h"
#include "State.h"

class MainMenu
{
	RenderWindow& window;

	Sprite background;

	std::array<Button, 3> buttons;
	std::array<State, 3>  states;

	bool running;
	State nextState;

public:
	MainMenu(RenderWindow& win);

	void Reset();
	void Run();

	bool IsRunning()
	{
		return running;
	}

	State GetNextState()
	{
		return nextState;
	}
};

#endif //MAIN_MENU_H
