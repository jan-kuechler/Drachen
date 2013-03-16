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
#include "EnemySettings.h"
#include "Rectangle.h"

struct TowerSettings;

class Game
{
	RenderWindow& window;
	GlobalStatus& globalStatus;

	Image imgBg;
	Sprite bg;

	Sprite loadingScreenBackground;
	sfext::Rectangle loadingScreenBar;

	std::vector<EnemySettings> enemySettings;
	std::vector<std::shared_ptr<Enemy>> enemies;

	std::vector<Tower> towers;

	std::vector<Projectile> projectiles;

	Map map;
	GameStatus gameStatus;
	GameUserInterface userInterface;
	//Theme theme;
	LevelMetaInfo levelInfo;
public:
	Game(RenderWindow& win, GlobalStatus& gs);

	void Run();
	void Reset();

	bool IsRunning();
	State GetNextState();

	void AddTower(TowerSettings* settings, Vector2f pos);

private:
	bool running;

	void LoadLevel(const std::string& level);

	void UpdateWave();
	void SpawnEnemy(size_t type);

	void LooseLife();

	void LoadEnemySettings();

	void UpdateLoadingScreen(float pct);
};

#endif //GAME_H
