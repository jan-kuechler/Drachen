#include "pch.h"
#include "CanonTower.h"
#include "CanonBall.h"
#include "Utility.h"

CanonTower::CanonTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles, bool highRange)
: Tower(settings, enemies, projectiles, highRange)
{ }

void CanonTower::Attack()
{
	if (currentTarget.expired() || dist(*currentTarget.lock(), *this) > range)
		ChooseTarget();

	auto target = currentTarget.lock();
	if (!target)
		return;

	Vector2f offs; // per default use last valid offset
	for (size_t i=0; i < attacks; ++i) {
		const TowerSettings::Stage& st = settings->stage[stage];

		if (i < st.attackPosition.size())
			offs = st.attackPosition[i];


		std::unique_ptr<CanonBall> p(new CanonBall(target, enemies, st.power, st.speed, st.splashRange, st.splashPower));
		p->SetImage(*st.projectile);
		p->SetPosition(GetPosition() - GetCenter() + offs);
		projectiles.emplace_back(std::move(p));
	}
}

static bool CmpByDist(const std::shared_ptr<Enemy>& a, const std::shared_ptr<Enemy>& b, const Drawable* ref)
{
	return dist(*a, *ref) < dist(*b, *ref);
}

void CanonTower::ChooseTarget()
{
	std::vector<std::shared_ptr<Enemy>> relevantEnemies;
	for (const auto &e : enemies) {
		if(!e->IsIrrelevant()) {
			relevantEnemies.push_back(e);
		}
	}
	auto  it = boost::min_element(relevantEnemies, boost::bind(CmpByDist, _1, _2, this));
	if (it == relevantEnemies.end() || dist(*(*it), *this) > range) //|| dist(*(*it), *this) > range
		currentTarget.reset();
	else
		currentTarget = *it;
}
