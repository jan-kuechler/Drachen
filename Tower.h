#ifndef TOWER_H
#define TOWER_H

#include "AnimSprite.h"
#include "Map.h"

class Tower : public AnimSprite
{
	const Map* map;
	
	bool placed;

public:
	Tower(const Map* map);

	bool HandleEvent(Event& event);

	bool IsPlaced() const
	{
		return placed;
	}
};

#endif //TOWER_H
