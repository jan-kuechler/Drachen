#include "pch.h"
#include "Win.h"
#include "Theme.h"
#include "GlobalStatus.h"
#include "ResourceManager.h"
#include "Utility.h"
#include "Button.h"

Win::Win(RenderWindow& win)
: window(win), running(true)
{ }

void Win::Reset()
{
	running = true;

	int level = static_cast<int>(gStatus.levelIndex);
	if (level > gStatus.packInfo[gStatus.levelPack].lastWonLevel)
		gStatus.packInfo[gStatus.levelPack].lastWonLevel = level;

	gStatus.runTime.levelPicker.commingFromLevel = level;
	gStatus.runTime.levelPicker.didWin = true;

	background.SetImage(gImageManager.getResource(gTheme.GetFileName("win/background")));
	background.SetPosition(0, 0);
}

void Win::Run()
{
	// Handle all SFML events
	Event event;
	while (window.GetEvent(event)) {
		// Handle default stuff like window closed etc.
		if (DefaultHandleEvent(window, event))
			continue;

		if (event.Type == Event::MouseButtonReleased) {
			running = false;
		}
	}

	window.Draw(background);
	window.Display();
}
