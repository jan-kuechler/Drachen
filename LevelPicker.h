#ifndef LEVEL_PICKER_H
#define LEVEL_PICKER_H

#include "Button.h"
#include "State.h"
#include "LevelPack.h"

class LevelPicker : public StateDef
{
	RenderWindow& window;

	Sprite background;

	std::map<std::string,LevelPack> levelPacks;

	//std::array<Button, 3> buttons;
	//std::array<State, 3>  states;

	String strName;
	String strDesc;

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
		return ST_GAME;
	}

private:
	void LoadLevelPacks();
};

#endif //LEVEL_PICKER_H
