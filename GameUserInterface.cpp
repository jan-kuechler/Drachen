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
#include "UiHelper.h"

using boost::lexical_cast;

// do not update the text every frame
static const float TEXT_UPDATE_TIME = 0.2f;

GameUserInterface::GameUserInterface(Game* game, RenderWindow& window, GlobalStatus& globalStatus, GameStatus& gameStatus, const Map* map)
: game(game), window(window), globalStatus(globalStatus), gameStatus(gameStatus), map(map)
{ }

void GameUserInterface::Reset(const LevelMetaInfo& metaInfo)
{
	topPanel.SetImage(gImageManager.getResource(gTheme.GetFileName("top-panel")));
	topPanel.SetPosition(0, 0);

	auto bottomPanelImage = gTheme.GetFileName("bottom-panel");
	bottomPanel.SetImage(gImageManager.getResource(bottomPanelImage));
	bottomPanel.SetPosition(0, 500);

	size_t nTowerButtons = gTheme.GetArrayLength("tower-buttons");
	towerButtons.clear();
	towerButtonTypes.clear();
	for (size_t i=0; i < nTowerButtons; ++i) {
		Button btn;
		InitButton(btn, "tower-buttons[]", i);
		towerButtons.push_back(btn);
		towerButtonTypes.push_back(gTheme.GetInt("tower-buttons[]/tower", i));
	}

	size_t nDeco = gTheme.GetArrayLength("decorations");
	decoration.clear();
	for (size_t i=0; i < nDeco; ++i) {
		Sprite sp;
		InitImage(sp, "decorations[]", i);
		decoration.push_back(sp);
	}

	InitButton(btnUpgrade, "buttons/upgrade");
	InitButton(btnSell, "buttons/sell");
	selectedTower.reset();

	levelName.SetFont(gTheme.GetMainFont());
	levelName.SetText(metaInfo.name);
	auto boundaryBox = levelName.GetRect();
	levelName.SetPosition(gTheme.GetPosition("text/level-name/position") - Vector2f(boundaryBox.GetWidth() / 2, boundaryBox.GetHeight() / 2));
	levelName.SetSize(gTheme.GetFloat("text/level-name/font-size"));

	InitText(lives, "text/lives");
	InitText(countdown, "text/countdown");
	InitText(money, "text/money");

	// release any towerPlacer left from the previous round
	towerPlacer.release();

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
			selectedTower.reset(); // clear selected tower when placing a new one
			StartPlacingTower(i);
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

	if (selectedTower) {
		if (btnUpgrade.WasClicked() && selectedTower->CanUpgrade())
			selectedTower->Upgrade();
		if (btnSell.WasClicked()) {
			gameStatus.money += selectedTower->Sell();
			selectedTower.reset();
		}
	}
}

void GameUserInterface::PreDraw()
{
	if (selectedTower)
		selectedTower->DrawRangeCircle(window);
}

void GameUserInterface::Draw()
{
	window.Draw(topPanel);
	window.Draw(bottomPanel);

	window.Draw(levelName);
	window.Draw(lives);
	window.Draw(money);

	boost::for_each(decoration, [&](const Sprite& sp) {
			window.Draw(sp);
		});

	if (selectedTower) {
		window.Draw(btnUpgrade);
		window.Draw(btnSell);
	}

	for (auto it = towerButtons.begin(); it != towerButtons.end(); ++it)
		window.Draw(*it);

	if (showCountdown)
		window.Draw(countdown);

	if (towerPlacer) {
		towerPlacer->DrawRangeCircle(window, map->IsHighRangeBlock(map->PositionToBlock(towerPlacer->GetPosition())));
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

	if (selectedTower) {
		if (btnUpgrade.HandleEvent(event))
			return true;
		if (btnSell.HandleEvent(event))
			return true;
	}

	return false;
}

void GameUserInterface::UpdateText()
{
	lives.SetText(lexical_cast<std::string>(gameStatus.lives));
	money.SetText(lexical_cast<std::string>(gameStatus.money));

	if (gameStatus.waveState == GameStatus::InCountdown && gameStatus.currentWave < gameStatus.waves.size()) {
		countdown.SetText(lexical_cast<std::string>(gameStatus.waves[gameStatus.currentWave].countdown - static_cast<int>(gameStatus.countdownTimer.GetElapsedTime())));
		showCountdown = true;
	}
	else {
		showCountdown = false;
	}
}

void GameUserInterface::StartPlacingTower(size_t id)
{
	// check if tower placing is already in progress
	if (towerPlacer) 
		return;

	const TowerSettings* settings = gTheme.GetTowerSettings(id);

	if (gameStatus.money < settings->baseCost)
		return;

	assert(towerPlacer == nullptr);
	towerPlacer.reset(new TowerPlacer(map, settings));

	const Input& input = window.GetInput();
	towerPlacer->SetPosition(static_cast<float>(input.GetMouseX()), static_cast<float>(input.GetMouseY()));
}

void GameUserInterface::TowerSelected(std::shared_ptr<Tower> tower)
{
	if (selectedTower)
		selectedTower.reset();

	selectedTower = tower;
}
