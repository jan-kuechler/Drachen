#ifndef ARROW_TOWER_H
#define ARROW_TOWER_H

#include "Tower.h"

class ArrowTower : public Tower
{
public:
	ArrowTower(const TowerSettings* settings, std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles);
};

#endif //ARROW_TOWER_H
