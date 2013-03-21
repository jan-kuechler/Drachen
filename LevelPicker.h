#ifndef LEVEL_PICKER_H
#define LEVEL_PICKER_H

#include "Button.h"
#include "State.h"

class LevelPicker : public StateDef
{
	RenderWindow& window;

	Sprite background;

	std::array<Button, 3> buttons;
	std::array<State, 3>  states;

	bool running;
	State nextState;

public:
	LevelPicker(RenderWindow& win);

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

#endif //LEVEL_PICKER_H
