#include "pch.h"
#include "GameUserInterface.h"

GameUserInterface::GameUserInterface(RenderWindow& window, const Theme& theme, GlobalStatus& status)
: window(window), theme(theme), status(status)
{ }

void GameUserInterface::Reset()
{
	topPanel.SetImage(theme.GetTopPanelImage());
	topPanel.SetPosition(0, 0);
}

void GameUserInterface::Draw()
{
	window.Draw(topPanel);
}
