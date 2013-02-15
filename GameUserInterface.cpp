#include "pch.h"
#include "GameUserInterface.h"
#include "Map.h"
#include "TowerPlacer.h"
#include "TowerSettings.h"
#include "Utility.h"

using boost::lexical_cast;

// do not update the text every frame
static const float TEXT_UPDATE_TIME = 0.2f;

GameUserInterface::GameUserInterface(RenderWindow& window, const Theme& theme, GlobalStatus& globalStatus, GameStatus& gameStatus, const Map* map)
: window(window), theme(theme), globalStatus(globalStatus), gameStatus(gameStatus), map(map)
{ }

void GameUserInterface::Reset(const LevelMetaInfo& metaInfo)
{
	topPanel.SetImage(theme.GetTopPanelImage());
	topPanel.SetPosition(0, 0);

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

	if (towerPlacer) {
		if (towerPlacer->IsPlaced()) {
			// TODO: Tell the game the tower that there is a tower
			towerPlacer.release();
		}
		else if (towerPlacer->PlacingCanceld()) {
			towerPlacer.release();
		}
	}
}

void GameUserInterface::Draw()
{
	window.Draw(topPanel);

	window.Draw(levelName);
	window.Draw(lives);

	if (showCountdown)
		window.Draw(countdown);

	if (towerPlacer)
		window.Draw(*towerPlacer);
}

bool GameUserInterface::HandleEvent(Event& event)
{
	if (towerPlacer && towerPlacer->HandleEvent(event))
		return true;

	if (event.Type == Event::KeyReleased) {
		switch (event.Key.Code) {
		case Key::T:
			if (!towerPlacer) {
				StartPlacingTower();
			}
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
