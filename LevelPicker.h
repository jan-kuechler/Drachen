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

	std::vector<Button> levelButtons;
	std::vector<String> levelStrings;

	String strName;
	String strDesc;
	Sprite previewImage;

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
