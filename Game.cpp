#include "pch.h"
#include "Game.h"
#include "Utility.h"

Game::Game(RenderWindow& win, GlobalStatus& gs)
: window(win), status(gs)
{ }

void Game::Reset()
{
	LoadFromFile(map, LEVEL("level.js"));
	overlay = false;
	map.DrawOverlay(false);

	LoadFromFile(imgFoe, "data/models/test.png");
	foe.SetImage(imgFoe);
	foe.SetOffset(1);
	foe.SetSize(50, 50);
	foe.SetFrameTime(.2f);
	foe.SetNumFrames(4);

	foe.SetMap(&map);
	foe.SetPosition(map.BlockToPosition(Vector2i(0, 7)));

	foe.SetSpeed(50);
}

void Game::Run()
{
	Event event;
	while (window.GetEvent(event)) {
		if (DefaultHandleEvent(window, event))
			continue;

		if (event.Type == Event::KeyReleased) {
			if (event.Key.Code == Key::G)
				foe.SetTarget(24, 17);
			if (event.Key.Code == Key::O)
				map.DrawOverlay(overlay = !overlay);
		}
	}

	foe.Update(window.GetFrameTime());

	window.Clear();
	map.Draw(window);
	window.Draw(foe);
	window.Display();
}

bool Game::IsRunning()
{
	return true;
}

State Game::GetNextState()
{
	return ST_QUIT;
}
