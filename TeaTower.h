#ifndef TEA_TOWER_H
#define TEA_TOWER_H

#include "Tower.h"

class TeaTower : public Tower
{
public:
	TeaTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles, bool highRange);

protected:
	void Attack();
};

#endif //TEA_TOWER_H
