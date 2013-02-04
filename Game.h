#ifndef GAME_H
#define GAME_H

#include "GameUserInterface.h"
#include "GameStatus.h"
#include "GlobalStatus.h"
#include "State.h"
#include "Enemy.h"
#include "Map.h"
#include "Tower.h"
#include "Projectile.h"
#include "Theme.h"
#include "LevelMetaInfo.h"

class Game
{
	RenderWindow& window;
	GlobalStatus& globalStatus;

	Image imgBg;
	Sprite bg;

	Image imgFoe;
	std::vector<std::shared_ptr<Enemy>> enemies;

	Image imgTower;
	Tower* activeTower;
	std::vector<Tower> towers;

	std::vector<Projectile> projectiles;

	Map map;
	GameStatus gameStatus;
	GameUserInterface userInterface;
	Theme theme;
	LevelMetaInfo levelInfo;
public:
	Game(RenderWindow& win, GlobalStatus& gs);

	void Run();
	void Reset();

	bool IsRunning();
	State GetNextState();

private:
	bool running;

	void LoadLevel(const std::string& level);

	void AddTower();
	void AddEnemy();

	void LooseLife();
};

#endif //GAME_H
