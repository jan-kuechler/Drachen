#ifndef TOWER_H
#define TOWER_H

#include "AnimSprite.h"
#include "Enemy.h"
#include "Map.h"
#include "Projectile.h"

class Tower : public AnimSprite
{
	const Map* map;
	const std::vector<Enemy>* enemies;
	std::vector<Projectile>* projectiles;

	static Image projectileImg;
	static bool imgLoaded;

	bool placed;
	bool validPosition;

	float range;

	static Color ColorInvalidPosition, ColorValidPosition, ColorPlaced, ColorRangeCircle;
	Shape rangeCircle;

	float cooldown;
public:
	Tower(const Map* map, const std::vector<Enemy>* enemies, std::vector<Projectile>* projectiles);

	bool HandleEvent(Event& event);

	bool IsPlaced() const
	{
		return placed;
	}

	void DrawRangeCircle(RenderTarget& target);
	void Update(float elapsed) /* override */;
};

#endif //TOWER_H
