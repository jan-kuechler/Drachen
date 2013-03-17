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
	const std::vector<std::shared_ptr<Enemy>>& enemies;
	std::vector<Projectile>& projectiles;

	bool hasHighRange;

	float  range;
	float  cooldown;
	size_t attacks;
	float  power;
	
	float cooldownTimer;

	const TowerSettings* settings;
	size_t stage;

public:
	static std::unique_ptr<Tower> CreateTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, bool highRange);

	void Update(float elapsed) /* override */;

	bool CanUpgrade()
	{
		return stage < (settings->stage.size() - 1);
	}

	void Upgrade();

protected:
	Tower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, bool highRange);

	virtual void ApplyStage();
	virtual void Attack();
};

#endif //TOWER_H
