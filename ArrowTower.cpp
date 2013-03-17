#include "pch.h"
#include "ArrowTower.h"

ArrowTower::ArrowTower(const TowerSettings* settings, std::vector<std::shared_ptr<Enemy>>* enemies, std::vector<Projectile>* projectiles)
: Tower(settings, enemies, projectiles)
{ }
