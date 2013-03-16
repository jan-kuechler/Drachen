#include "pch.h"
#include "MainMenu.h"
#include "Theme.h"
#include "GlobalStatus.h"
#include "ResourceManager.h"
#include "Utility.h"
#include "Button.h"

MainMenu::MainMenu(RenderWindow& win)
: window(win), running(true), nextState(ST_QUIT)
{ 
	states[0] = ST_GAME;
	states[1] = ST_OPTIONS_MENU;
	states[2] = ST_QUIT;
}

void MainMenu::Reset()
{
	running = true;

	background.SetImage(gImageManager.getResource(gTheme.GetFileName("main-menu/background")));
	background.SetPosition(0, 0);

	for (size_t i=0; i < buttons.size(); ++i) {
		buttons[i].SetImage(gImageManager.getResource(gTheme.GetFileName("main-menu/buttons[]", i)));
		buttons[i].SetPosition(550.f, 150.f + i * 100.f);
	}
}

void MainMenu::Run()
{
	// Handle all SFML events
	Event event;
	while (window.GetEvent(event)) {
		// Handle default stuff like window closed etc.
		if (DefaultHandleEvent(window, event))
			continue;

		bool handled = false;
		for (size_t i=0; i < buttons.size(); ++i) {
			if (buttons[i].HandleEvent(event)) {
				handled = true;
				break;
			}
		}
		if (handled)
			continue;
	}

	for (size_t i=0; i < buttons.size(); ++i) {
		if (buttons[i].WasClicked()) {
			running = false;
			nextState = states[i];
		}
	}

	window.Draw(background);

	for (size_t i=0; i < buttons.size(); ++i) {
		window.Draw(buttons[i]);
	}

	window.Display();
}
