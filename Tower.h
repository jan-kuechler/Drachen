#ifndef TOWER_H
#define TOWER_H

#include "AnimSprite.h"
#include "Enemy.h"
#include "Map.h"
#include "Projectile.h"
#include "TowerSettings.h"

class Tower : public AnimSprite
{
protected:
	std::vector<std::shared_ptr<Enemy>>* enemies;
	std::vector<Projectile>* projectiles;

	float range;
	float cooldown;
	
	float cooldownTimer;

	const TowerSettings* settings;
	size_t stage;

public:
	static std::unique_ptr<Tower> CreateTower(const TowerSettings* settings, std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles);

	void Update(float elapsed) /* override */;

protected:
	Tower(const TowerSettings* settings, std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles);

	virtual void ApplyStage();
	virtual void Attack();
};

#endif //TOWER_H
