#ifndef TOWER_H
#define TOWER_H

#include "AnimSprite.h"
#include "Map.h"

class Tower : public AnimSprite
{
	const Map* map;
	
	bool placed;
	bool validPosition;

	static Color ColorInvalidPosition, ColorValidPosition, ColorPlaced;

public:
	Tower(const Map* map);

	bool HandleEvent(Event& event);

	bool IsPlaced() const
	{
		return placed;
	}
};

#endif //TOWER_H
