#include "pch.h"
#include "Tower.h"
#include "Utility.h"

Color Tower::ColorInvalidPosition(255, 0, 0, 128);
Color Tower::ColorValidPosition(0, 255, 0, 128);
Color Tower::ColorPlaced(255, 255, 255);
Color Tower::ColorRangeCircle(64, 255, 64, 128);

Tower::Tower(const Map* map, const std::vector<Enemy>* enemies)
: map(map), enemies(enemies), placed(false), validPosition(false), range(4.0f * map->GetBlockSize()), cooldown(1.0f)
{ 
	SetColor(ColorInvalidPosition);

	rangeCircle = Shape::Circle(GetPosition(), range, ColorRangeCircle);
}
bool Tower::HandleEvent(Event& event)
{
	if (placed)
		return false;

	if (event.Type == Event::MouseMoved) {
		Vector2f pos(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));
		Vector2i tpos = map->PostionToTowerPos(pos);

		if (map->MayPlaceTower(tpos)) {
			SetPosition(map->TowerPosToPosition(tpos));
			validPosition = true;
			SetColor(ColorValidPosition);
		}
		else {
			SetPosition(pos);
			validPosition = false;
			SetColor(ColorInvalidPosition);
		}
		rangeCircle.SetPosition(GetPosition());
	}
	else if (event.Type == Event::MouseButtonPressed && validPosition && event.MouseButton.Button == Mouse::Left) {
		placed = true;
		SetColor(ColorPlaced);
	}
	else {
		return false;
	}
	return true;
}

void Tower::DrawRangeCircle(RenderTarget& target)
{
	//if (!placed)
		target.Draw(rangeCircle);
}

void Tower::Update(float elapsed)
{
	if (cooldown > 0)
		cooldown -= elapsed;

	if (cooldown <= 0) {
		for (auto it = enemies->begin(); it != enemies->end(); ++it) {
			if (norm(it->GetPosition() - GetPosition()) <= range) {
				SetColor(ColorInvalidPosition);
				cooldown = 1.0f;
			}
		}
	}

	AnimSprite::Update(elapsed);
}
