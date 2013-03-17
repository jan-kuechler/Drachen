#ifndef GAME_USER_INTERFACE_H
#define GAME_USER_INTERFACE_H

#include "Button.h"
#include "Theme.h"
#include "TowerPlacer.h"

class Map;
class Game;
class Theme;
struct GlobalStatus;
struct GameStatus;
struct LevelMetaInfo;

class GameUserInterface
{
	RenderWindow& window;

	GlobalStatus& globalStatus;
	GameStatus& gameStatus;
	const Map* map;
	Game* game;

	Sprite topPanel, bottomPanel;

	sf::String levelName;
	sf::String lives;
	sf::String countdown;
	sf::String money;

	std::vector<Button> towerButtons;
	std::vector<int> towerButtonTypes;

	bool showCountdown;

	sf::Clock textUpdateClock;

	std::unique_ptr<TowerPlacer> towerPlacer;

public:
	GameUserInterface(Game* game, RenderWindow& window, GlobalStatus& globalStatus, GameStatus& gameStatus, const Map *map);

	void Update();
	void Draw();
	void Reset(const LevelMetaInfo& metaInfo);

	bool HandleEvent(sf::Event& event);

private:
	void UpdateText();

	void StartPlacingTower(size_t id);

	void LoadDefinition();
};

#endif //GAME_USER_INTERFACE_H
