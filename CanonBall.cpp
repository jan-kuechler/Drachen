#include "pch.h"
#include "CanonBall.h"
#include "Utility.h"

static const float HIT_DISTANCE = 10.f;

CanonBall::CanonBall(std::weak_ptr<Enemy> target, const std::vector<std::shared_ptr<Enemy>>& enemies, float power, float speed, float range, float splash)
: Projectile(target, power, speed), enemies(enemies), range(range), splash(splash)
{ }

void CanonBall::Hit(std::shared_ptr<Enemy>& tgt)
{
	if (tgt)
		tgt->Hit(power);

	boost::for_each(enemies, [&](const std::shared_ptr<Enemy>& e) {
		if (e != tgt && dist(*e, *this) < range) {
			e->Hit(splash);
		}
	});
}
