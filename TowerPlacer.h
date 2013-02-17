#ifndef TOWER_PLACER_H
#define TOWER_PLACER_H

#include "AnimSprite.h"

class Map;
struct TowerSettings;

class TowerPlacer : public AnimSprite
{
	const Map* map;
	TowerSettings* settings;

	static Color ColorInvalidPosition, ColorValidPosition, ColorPlaced, ColorRangeCircle;
	Shape rangeCircle;

	bool validPosition;
	bool placed, cancelPlacing;
public:
	TowerPlacer(const Map* map, TowerSettings* settings);

	bool HandleEvent(sf::Event& event);

	void SetPosition(float x, float y)
	{
		AnimSprite::SetPosition(x, y);
		rangeCircle.SetPosition(x, y);
	}

	void SetPosition(Vector2f pos)
	{
		AnimSprite::SetPosition(pos);
		rangeCircle.SetPosition(pos);
	}

	bool IsPlaced()
	{
		return placed;
	}

	bool PlacingCanceled()
	{
		return cancelPlacing;
	}

	void DrawRangeCircle(sf::RenderTarget& target)
	{
		target.Draw(rangeCircle);
	}

	TowerSettings* GetSettings()
	{
		return settings;
	}
};

#endif //TOWER_PLACER_H
