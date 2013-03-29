#include "pch.h"
#include "TowerPlacer.h"
#include "Map.h"
#include "TowerSettings.h"
#include "Utility.h"

static const float PLACE_RANGE = 25.f;

Color TowerPlacer::ColorInvalidPosition(255, 0, 0, 128);
Color TowerPlacer::ColorValidPosition(0, 255, 0, 128);
Color TowerPlacer::ColorPlaced(255, 255, 255);
Color TowerPlacer::ColorRangeCircle(64, 255, 64, 128);

TowerPlacer::TowerPlacer(const Map* map, const TowerSettings* settings)
: map(map), settings(settings), placed(false), cancelPlacing(false)
{
	SetColor(ColorInvalidPosition);
	Image *img = settings->stage.at(0).image;
	SetImage(*img);
	SetSize(img->GetWidth(), img->GetHeight());
	SetCenter(settings->stage.at(0).center);

	rangeCircle = Shape::Circle(GetPosition(), settings->stage[0].range, ColorRangeCircle);
	highRangeCircle = Shape::Circle(GetPosition(), settings->stage[0].range * HIGH_RANGE_FACTOR, ColorRangeCircle);
}

static bool CmpByDist(const Vector2f& a, const Vector2f& b, const Vector2f& ref)
{
	return dist(a, ref) < dist(b, ref);
}

bool TowerPlacer::HandleEvent(Event& event)
{
	if (placed || cancelPlacing)
		return false;

	if (event.Type == Event::MouseMoved) {
		Vector2f pos(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));

		auto towerPlaces = map->GetTowerPlaces();
		auto nearestPlaceIt = boost::min_element(towerPlaces, boost::bind(CmpByDist, _1, _2, pos));
		assert(nearestPlaceIt != towerPlaces.end());
		auto nearestPlace = *nearestPlaceIt;
		auto distToNearest = dist(nearestPlace, pos);

		if (distToNearest < PLACE_RANGE) {
			SetPosition(nearestPlace);
			validPosition = true;
			SetColor(ColorValidPosition);
		}
		else {
			SetPosition(pos);
			validPosition = false;
			SetColor(ColorInvalidPosition);
		}
	}
	else if (event.Type == Event::MouseButtonReleased) {
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
