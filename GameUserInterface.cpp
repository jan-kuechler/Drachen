#include "pch.h"
#include "GameUserInterface.h"
#include "Map.h"
#include "Game.h"
#include "GlobalStatus.h"
#include "GameStatus.h"
#include "LevelMetaInfo.h"
#include "TowerPlacer.h"
#include "TowerSettings.h"
#include "ResourceManager.h"
#include "Utility.h"

using boost::lexical_cast;

// do not update the text every frame
static const float TEXT_UPDATE_TIME = 0.2f;

GameUserInterface::GameUserInterface(Game* game, RenderWindow& window, const Theme& theme, GlobalStatus& globalStatus, GameStatus& gameStatus, const Map* map)
: game(game), window(window), theme(theme), globalStatus(globalStatus), gameStatus(gameStatus), map(map)
{ }

void GameUserInterface::Reset(const LevelMetaInfo& metaInfo)
{
	topPanel.SetImage(gImageManager.getResource(theme.GetFileName("top-panel")));
	topPanel.SetPosition(0, 0);

	auto bottomPanelImage = theme.GetFileName("bottom-panel");
	bottomPanel.SetImage(gImageManager.getResource(bottomPanelImage));
	bottomPanel.SetPosition(0, 500);

	size_t nTowerButtons = theme.GetArrayLength("tower-buttons");
	towerButtons.clear();
	towerButtonTypes.clear();
	for (size_t i=0; i < nTowerButtons; ++i) {
		Button btn;
		btn.SetImage(gImageManager.getResource(theme.GetFileName("tower-buttons[]/image", i)));
		btn.SetPosition(theme.GetPosition("tower-buttons[]/position", i));
		towerButtons.push_back(btn);
		towerButtonTypes.push_back(theme.GetInt("tower-buttons[]/tower", i));
	}

	levelName.SetFont(theme.GetMainFont());
	levelName.SetText(metaInfo.name);
	levelName.SetPosition(theme.GetPosition("level-name-position"));
	levelName.SetSize(theme.GetFloat("level-name-font-size"));

	lives.SetFont(theme.GetMainFont());
	lives.SetText(lexical_cast<std::string>(gameStatus.lives));
	lives.SetPosition(theme.GetPosition("lives-position"));
	lives.SetSize(theme.GetFloat("lives-font-size"));

	countdown.SetFont(theme.GetMainFont());
	countdown.SetPosition(theme.GetPosition("countdown-position"));
	countdown.SetSize(theme.GetFloat("countdown-font-size"));

	UpdateText();
	textUpdateClock.Reset();
}

void GameUserInterface::Update()
{
	if (textUpdateClock.GetElapsedTime() > TEXT_UPDATE_TIME) {
		UpdateText();
		textUpdateClock.Reset();
	}

	for (size_t i=0; i < towerButtons.size(); ++i) {
		if (towerButtons[i].WasClicked()) {
			StartPlacingTower();
		}
	}

	if (towerPlacer) {
		if (towerPlacer->IsPlaced()) {
			game->AddTower(towerPlacer->GetSettings(), towerPlacer->GetPosition());
			towerPlacer.release();
		}
		else if (towerPlacer->PlacingCanceled()) {
			towerPlacer.release();
		}
	}
}

void GameUserInterface::Draw()
{
	window.Draw(topPanel);
	window.Draw(bottomPanel);

	window.Draw(levelName);
	window.Draw(lives);

	for (auto it = towerButtons.begin(); it != towerButtons.end(); ++it)
		window.Draw(*it);

	if (showCountdown)
		window.Draw(countdown);

	if (towerPlacer) {
		towerPlacer->DrawRangeCircle(window);
		window.Draw(*towerPlacer);
	}
}

bool GameUserInterface::HandleEvent(Event& event)
{
	if (towerPlacer && towerPlacer->HandleEvent(event))
		return true;

	for (auto it = towerButtons.begin(); it != towerButtons.end(); ++it)
		if (it->HandleEvent(event))
			return true;

	if (event.Type == Event::KeyReleased) {
		switch (event.Key.Code) {
		case Key::T:
			StartPlacingTower();
			return true;
		}
	}

	return false;
}

void GameUserInterface::UpdateText()
{
	lives.SetText(lexical_cast<std::string>(gameStatus.lives));

	if (gameStatus.waveState == GameStatus::InCountdown && gameStatus.currentWave < gameStatus.waves.size()) {
		countdown.SetText(lexical_cast<std::string>(gameStatus.waves[gameStatus.currentWave].countdown - static_cast<int>(gameStatus.countdownTimer.GetElapsedTime())));
		showCountdown = true;
	}
	else {
		showCountdown = false;
	}
}

void GameUserInterface::StartPlacingTower()
{
	// check if tower placing is already in progress
	if (towerPlacer) 
		return;

	// TODO: Fix hard coded tower cost
	if (gameStatus.money < 100)
		return;

	gameStatus.money -= 100;

	static TowerSettings testSettings;
	if (!testSettings.baseImage) {
		testSettings.baseImage = new sf::Image;
		LoadFromFile(*testSettings.baseImage, "data/models/archer_level1.png");
	}
	testSettings.range = 4.0f * map->GetBlockSize();
	testSettings.cooldown = 1.0f;

	assert(towerPlacer == nullptr);

	towerPlacer.reset(new TowerPlacer(map, &testSettings));

	const Input& input = window.GetInput();
	towerPlacer->SetPosition(static_cast<float>(input.GetMouseX()), static_cast<float>(input.GetMouseY()));
}
