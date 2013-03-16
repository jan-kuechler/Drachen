#include "pch.h"
#include "TowerPlacer.h"
#include "Map.h"
#include "TowerSettings.h"

Color TowerPlacer::ColorInvalidPosition(255, 0, 0, 128);
Color TowerPlacer::ColorValidPosition(0, 255, 0, 128);
Color TowerPlacer::ColorPlaced(255, 255, 255);
Color TowerPlacer::ColorRangeCircle(64, 255, 64, 128);

TowerPlacer::TowerPlacer(const Map* map, TowerSettings* settings)
: map(map), settings(settings), placed(false), cancelPlacing(false)
{
	SetColor(ColorInvalidPosition);
	SetImage(*settings->baseImage);
	SetSize(settings->baseImage->GetWidth(), settings->baseImage->GetHeight());
	SetCenter(GetWidth() / 2.f, GetHeight() - GetWidth() / 3.f);

	rangeCircle = Shape::Circle(GetPosition(), settings->range, ColorRangeCircle);
	highRangeCircle = Shape::Circle(GetPosition(), settings->range * 1.5f, ColorRangeCircle);
}

bool TowerPlacer::HandleEvent(Event& event)
{
	if (placed || cancelPlacing)
		return false;

	if (event.Type == Event::MouseMoved) {
		Vector2f pos(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));
		Vector2i tpos = map->PositionToBlock(pos);

		if (map->MayPlaceTower(tpos)) {
			SetPosition(map->BlockToPosition(tpos));
			validPosition = true;
			SetColor(ColorValidPosition);
		}
		else {
			SetPosition(pos);
			validPosition = false;
			SetColor(ColorInvalidPosition);
		}
	}
	else if (event.Type == Event::MouseButtonPressed) {
		if (event.MouseButton.Button == Mouse::Left && validPosition) {
			placed = true;
			SetColor(ColorPlaced);
		}
		else {
			cancelPlacing = true;
		}
		return true;
	}

	return false;
}
