#ifndef TOWER_H
#define TOWER_H

#include "AnimSprite.h"
#include "Enemy.h"
#include "Map.h"
#include "Projectile.h"

class Tower : public AnimSprite
{
	std::vector<std::shared_ptr<Enemy>>* enemies;
	std::vector<Projectile>* projectiles;

	static Image projectileImg;
	static bool imgLoaded;

	float range;
	float cooldown;

public:
	Tower(std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles);

	bool HandleEvent(Event& event);

	void Update(float elapsed) /* override */;
};

#endif //TOWER_H
