#ifndef GAME_USER_INTERFACE_H
#define GAME_USER_INTERFACE_H

#include "GlobalStatus.h"
#include "GameStatus.h"
#include "LevelMetaInfo.h"
#include "Theme.h"
#include "TowerPlacer.h"

class Map;

class GameUserInterface
{
	const Theme& theme;
	RenderWindow& window;

	GlobalStatus& globalStatus;
	GameStatus& gameStatus;
	const Map* map;

	Sprite topPanel;

	sf::String levelName;
	sf::String lives;
	sf::String countdown;

	bool showCountdown;

	sf::Clock textUpdateClock;

	std::unique_ptr<TowerPlacer> towerPlacer;

public:
	GameUserInterface(RenderWindow& window, const Theme& theme, GlobalStatus& globalStatus, GameStatus& gameStatus, const Map *map);

	void Update();
	void Draw();
	void Reset(const LevelMetaInfo& metaInfo);

	bool HandleEvent(sf::Event& event);

private:
	void UpdateText();

	void StartPlacingTower();
};

#endif //GAME_USER_INTERFACE_H
