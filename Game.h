#ifndef GAME_H
#define GAME_H

#include "GlobalStatus.h"
#include "State.h"
#include "Enemy.h"
#include "Map.h"

class Game
{
	RenderWindow& window;
	GlobalStatus& status;

	Image imgBg;
	Sprite bg;

	Image imgFoe;
	Enemy foe;

	Map map;

	bool overlay;

public:
	Game(RenderWindow& win, GlobalStatus& gs);

	void Run();
	void Reset();

	bool IsRunning();
	State GetNextState();

private:
	std::string LEVEL(const char *what) const
	{
		return "data/levels/" + status.level + "/" + what;
	}
};

#endif //GAME_H