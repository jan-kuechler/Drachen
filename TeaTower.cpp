#include "pch.h"
#include "TeaTower.h"
#include "Utility.h"

TeaTower::TeaTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles, bool highRange)
: Tower(settings, enemies, projectiles, highRange)
{ }

void TeaTower::Attack()
{
	boost::for_each(enemies, [&](const std::shared_ptr<Enemy>& e) {
			if (dist(*e, *this) < range) {
				e->Hit(power);
			}
		});
}
