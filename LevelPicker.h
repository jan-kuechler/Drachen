#ifndef LEVEL_PICKER_H
#define LEVEL_PICKER_H

#include "Button.h"
#include "State.h"
#include "LevelPack.h"

class LevelPicker : public StateDef
{
	RenderWindow& window;

	Sprite background;

	std::vector<std::string> levelPackOrder;
	std::map<std::string,LevelPack> levelPacks;

	std::vector<Button> levelButtons;
	std::vector<String> levelStrings;
	std::vector<bool>   levelEnabled;

	String strName;
	String strDesc;
	Sprite previewImage;

	Button backButton;

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

private:
	void LoadLevelPacks();

	bool ShouldDisplayText(const std::string& id);
	void DisplayText(const std::string& id, State nextState);
};

#endif //LEVEL_PICKER_H
