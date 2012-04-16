#include "pch.h"
#include "Game.h"
#include "Utility.h"
#include "Tower.h"

Game::Game(RenderWindow& win, GlobalStatus& gs)
: window(win), status(gs), activeTower(0)
{ }

void Game::Reset()
{
	enemies.clear();
	towers.clear();
	projectiles.clear();
	treasures.clear();

	LoadFromFile(map, LEVEL("level.js"));
	LoadFromFile(imgFoe, "data/models/test.png");
	LoadFromFile(imgTower, "data/models/archer_level1.png");
	LoadFromFile(imgTreasure, "data/models/gem.png");

	size_t numTreasures = std::min(status.numTreasures, map.GetNumTreasurePlaces());
	for (size_t i=0; i < numTreasures; ++i) {
		Treasure t;
		t.SetPosition(map.BlockToPosition(map.GetTreasurePlace(i)));
		t.SetImage(imgTreasure);
		t.SetCenter(imgTreasure.GetWidth() / 2.0f, imgTreasure.GetHeight() / 2.0f);
		treasures.push_back(t);
	}
}

// Compare towers by their y position, to ensure lower towers (= higher y pos) are drawn
// later, so the overlap is displayed correctly.
static bool CompTowerY(const Tower& a, const Tower& b)
{
	return a.GetPosition().y < b.GetPosition().y;
}

static bool ShouldRemoveEnemy(const std::shared_ptr<Enemy>& e)
{
	return e->IsDead() && e->ProjectileCount() == 0;
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
			else if (activeTower->StopPlace()) {
				towers.pop_back();
				activeTower = 0;
			}
			continue;
		}

		if (event.Type == Event::KeyReleased) {
			switch (event.Key.Code) {
			case Key::G:
				AddEnemy();
				break;
			case Key::T:
				AddTower();
				break;
			case Key::F2:
				map.ToggleOverlay();
				break;
			case Key::F3:
				map.DebugToggleTowersAnywhere();
				break;
			}
		}
	}

	float elapsed = window.GetFrameTime();

	for (auto it = enemies.begin(); it != enemies.end(); ++it)
		(*it)->Update(elapsed);
	for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
		it->Update(elapsed);
	for (auto it = towers.begin(); it != towers.end(); ++it)
		it->Update(elapsed);

	projectiles.erase(boost::remove_if(projectiles, boost::bind(&Projectile::DidHit, _1)), projectiles.end());
	enemies.erase(boost::remove_if(enemies, ShouldRemoveEnemy), enemies.end());

	UpdateEnemyTargets();

	window.Clear();
	map.Draw(window);
	for (auto it = towers.begin(); it != towers.end(); ++it) {
		it->DrawRangeCircle(window);
		window.Draw(*it);
	}
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		(*it)->DrawHpBar(window);
		window.Draw(*(*it));
	}
	for (auto it = treasures.begin(); it != treasures.end(); ++it) 
		window.Draw(*it);
	
	for (auto it = projectiles.begin(); it != projectiles.end(); ++it)
		window.Draw(*it);

	window.Display();
}

void Game::UpdateEnemyTargets()
{
	for (auto it = enemies.begin(); it != enemies.end(); ++it) {
		std::shared_ptr<Enemy> e = *it;
		if (e->IsDead())
			continue;

		auto target = NearestTarget(e->GetPosition());
		if (target != e->GetTarget())
			e->SetTarget(target);
	}
}

Vector2i Game::NearestTarget(const Vector2f& pos)
{
	int minIdx = -1;
	float minDist = std::numeric_limits<float>::max();

	for (size_t i=0; i < treasures.size(); ++i) {
		if (!treasures[i].Available())
			continue;

		float dist = abs(treasures[i].GetPosition() - pos);
		if (dist < minDist) {
			minIdx = i;
			minDist = dist;
		}
	}
	if (minIdx >= 0)
		return map.PositionToBlock(treasures[minIdx].GetPosition());
	return map.GetDefaultTarget();
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

	Tower t(&map, &enemies, &projectiles);
	t.SetImage(imgTower);

	const Input& input = window.GetInput();
	t.SetPosition(static_cast<float>(input.GetMouseX()), static_cast<float>(input.GetMouseY()));
	t.SetSize(imgTower.GetWidth(), imgTower.GetHeight());

	towers.push_back(t);
	activeTower = &towers.back();
}

void Game::AddEnemy()
{
	std::shared_ptr<Enemy>e(new Enemy(&map));
	e->SetImage(imgFoe);
	e->SetOffset(1);
	e->SetSize(50, 50);
	e->SetFrameTime(.2f);
	e->SetNumFrames(4);

	e->SetPosition(map.BlockToPosition(Vector2i(0, 7)));

	e->SetSpeed(50);
	e->SetTarget(Vector2i(24, 17));

	enemies.push_back(e);
}
