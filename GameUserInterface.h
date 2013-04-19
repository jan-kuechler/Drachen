#ifndef GAME_USER_INTERFACE_H
#define GAME_USER_INTERFACE_H

#include "Button.h"
#include "Theme.h"
#include "TowerPlacer.h"

class Map;
class Game;
class Theme;
class Tower;
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

	Button btnUpgrade, btnSell;

	String levelName;
	String lives;
	String countdown;
	String money;

	struct Tooltip
	{
		enum Mode {
			Hidden, Preview, Selected, Upgrade, Sell,
		};

		void Initialize(const std::string& prefix = "tooltip");

		void SetTower(const TowerSettings* settings, Mode md);
		void Clear();

		void Draw(RenderTarget& target);

		Tooltip()
		: mode(Hidden)
		{}
	private:
		Mode mode;

		Color buyColor, sellColor;

		String title;
		String subtitle;
		String cost;
		Sprite coin;
	};
	Tooltip tooltip;

	std::vector<Sprite> decoration;

	std::vector<Button> towerButtons;
	std::vector<int> towerButtonTypes;

	bool showCountdown;

	sf::Clock textUpdateClock;

	std::unique_ptr<TowerPlacer> towerPlacer;
	std::vector<Shape> towerMarkers;

	std::shared_ptr<Tower> selectedTower;

public:
	GameUserInterface(Game* game, RenderWindow& window, GlobalStatus& globalStatus, GameStatus& gameStatus, const Map *map);

	void Update();

	void PreDraw();
	void Draw();
	void Reset(const LevelMetaInfo& metaInfo);

	void TowerSelected(std::shared_ptr<Tower> tower);

	bool HandleEvent(sf::Event& event);

private:
	void UpdateText();

	void StartPlacingTower(size_t id);

	void LoadDefinition();
};

#endif //GAME_USER_INTERFACE_H
