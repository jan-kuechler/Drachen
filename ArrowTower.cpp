#include "pch.h"
#include "ArrowTower.h"
#include "Utility.h"

ArrowTower::ArrowTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<Projectile>& projectiles)
: Tower(settings, enemies, projectiles)
{ }

void ArrowTower::Attack()
{
	if (currentTarget.expired() || dist(*currentTarget.lock(), *this) > range)
		ChooseTarget();

	auto target = currentTarget.lock();
	if (!target)
		return;

	Projectile p(target);
	p.SetImage(*settings->stage[stage].projectile);
	p.SetPosition(GetPosition());
	projectiles.push_back(p);
}

bool CmpByDist(const std::shared_ptr<Enemy>& a, const std::shared_ptr<Enemy>& b, const Drawable* ref)
{
	return dist(*a, *ref) < dist(*b, *ref);
}

void ArrowTower::ChooseTarget()
{
	auto it = boost::min_element(enemies, boost::bind(CmpByDist, _1, _2, this));
	if (it == enemies.end() || dist(*(*it), *this) > range)
		currentTarget.reset();
	else
		currentTarget = *it;
}
