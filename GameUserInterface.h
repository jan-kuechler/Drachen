#ifndef GAME_USER_INTERFACE_H
#define GAME_USER_INTERFACE_H

#include "GlobalStatus.h"
#include "Theme.h"

class GameUserInterface
{
	const Theme& theme;
	RenderWindow& window;
	GlobalStatus& status;

	Sprite topPanel;

public:
	GameUserInterface(RenderWindow& window, const Theme& theme, GlobalStatus& status);

	void Draw();
	void Reset();
};

#endif //GAME_USER_INTERFACE_H
