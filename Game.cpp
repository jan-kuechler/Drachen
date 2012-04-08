#include "pch.h"
#include "Game.h"
#include "Utility.h"
#include "Tower.h"


Game::Game(RenderWindow& win, GlobalStatus& gs)
: window(win), status(gs), activeTower(0)
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

	LoadFromFile(imgTower, "data/models/archer_level1.png");
}

// Compare towers by their y position, to ensure lower towers (= higher y pos) are drawn
// later, so the overlap is displayed correctly.
static bool CompTowerY(const Tower& a, const Tower& b)
{
	return a.GetPosition().y < b.GetPosition().y;
}

void Game::Run()
{
	Event event;
	while (window.GetEvent(event)) {
		if (DefaultHandleEvent(window, event))
			continue;
		if (activeTower && activeTower->HandleEvent(event)) {
			if (activeTower->IsPlaced()) {
				map.PlaceTower(map.PostionToTowerPos(activeTower->GetPosition()));
				activeTower = 0;
				boost::sort(towers, CompTowerY);
			}
			continue;
		}

		if (event.Type == Event::KeyReleased) {
			if (event.Key.Code == Key::G)
				foe.SetTarget(24, 17);
			if (event.Key.Code == Key::O)
				map.DrawOverlay(overlay = !overlay);
			if (event.Key.Code == Key::T)
				AddTower();
		}
	}

	foe.Update(window.GetFrameTime());

	window.Clear();
	map.Draw(window);
	for (auto it = towers.begin(); it != towers.end(); ++it)
		window.Draw(*it);
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

void Game::AddTower()
{
	if (activeTower)
		return;

	Tower t(&map);
	t.SetImage(imgTower);

	const Input& input = window.GetInput();
	t.SetPosition(static_cast<float>(input.GetMouseX()), static_cast<float>(input.GetMouseY()));
	t.SetSize(imgTower.GetWidth(), imgTower.GetHeight());

	towers.push_back(t);
	activeTower = &towers.back();
}
