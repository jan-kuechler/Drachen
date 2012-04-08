#include "pch.h"
#include "Tower.h"

Color Tower::ColorInvalidPosition(255, 0, 0, 128);
Color Tower::ColorValidPosition(0, 255, 0, 128);
Color Tower::ColorPlaced(255, 255, 255);

Tower::Tower(const Map* map)
: map(map), placed(false), validPosition(false)
{ 
	SetColor(ColorInvalidPosition);
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
