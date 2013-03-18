#include "pch.h"
#include "ArrowTower.h"
#include "Utility.h"

ArrowTower::ArrowTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles, bool highRange)
: Tower(settings, enemies, projectiles, highRange)
{ }

void ArrowTower::Attack()
{
	if (currentTarget.expired() || dist(*currentTarget.lock(), *this) > range)
		ChooseTarget();

	auto target = currentTarget.lock();
	if (!target)
		return;

	Vector2f offs; // per default use last valid offset
	for (size_t i=0; i < attacks; ++i) {
		if (i < settings->stage[stage].attackPosition.size())
			offs = settings->stage[stage].attackPosition[i];

		std::unique_ptr<Projectile> p(new Projectile(target, settings->stage[stage].power, settings->stage[stage].speed));
		p->SetImage(*settings->stage[stage].projectile);
		p->SetPosition(GetPosition() - GetCenter() + offs);
		projectiles.emplace_back(std::move(p));
	}
}

static bool CmpByDist(const std::shared_ptr<Enemy>& a, const std::shared_ptr<Enemy>& b, const Drawable* ref)
{
	return dist(*a, *ref) < dist(*b, *ref);
}

void ArrowTower::ChooseTarget()
{
	auto it = boost::min_element(enemies | boost::adaptors::filtered([](const std::shared_ptr<Enemy>& e){
			return !e->IsIrrelevant();
		}), boost::bind(CmpByDist, _1, _2, this)).base();
	if (it == enemies.end() || dist(*(*it), *this) > range)
		currentTarget.reset();
	else
		currentTarget = *it;
}
