#ifndef CANON_TOWER_H
#define CANON_TOWER_H

#include "Tower.h"

class CanonTower : public Tower
{
	std::weak_ptr<Enemy> currentTarget;

public:
	CanonTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles, bool highRange);

protected:
	void Attack();

private:
	void ChooseTarget();
};

#endif //CANON_TOWER_H
