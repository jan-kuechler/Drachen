#include "pch.h"
#include "GameUserInterface.h"

using boost::lexical_cast;

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
}

void GameUserInterface::Update()
{
	lives.SetText(lexical_cast<std::string>(gameStatus.lives));
}

void GameUserInterface::Draw()
{
	window.Draw(topPanel);

	window.Draw(levelName);
	window.Draw(lives);
}
