#ifndef ARROW_TOWER_H
#define ARROW_TOWER_H

#include "Tower.h"

class ArrowTower : public Tower
{
	std::weak_ptr<Enemy> currentTarget;

public:
	ArrowTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles, bool highRange);

protected:
	void Attack();

private:
	void ChooseTarget();
};

#endif //ARROW_TOWER_H
