#include "pch.h"
#include "GameUserInterface.h"

using boost::lexical_cast;

// do not update the text every frame
static const float TEXT_UPDATE_TIME = 0.2f;

GameUserInterface::GameUserInterface(RenderWindow& window, const Theme& theme, GlobalStatus& globalStatus, GameStatus& gameStatus)
: window(window), theme(theme), globalStatus(globalStatus), gameStatus(gameStatus)
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
}

void GameUserInterface::Draw()
{
	window.Draw(topPanel);

	window.Draw(levelName);
	window.Draw(lives);

	if (showCountdown)
		window.Draw(countdown);
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
