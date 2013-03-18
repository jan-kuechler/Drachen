#ifndef CANON_BALL_H
#define CANON_BALL_H

#include "Projectile.h"

class CanonBall : public Projectile
{
	const std::vector<std::shared_ptr<Enemy>>& enemies;

	float range;
	float splash;

public:
	CanonBall(std::weak_ptr<Enemy> target, const std::vector<std::shared_ptr<Enemy>>& enemies, float power, float speed, float range, float splash);

protected:
	virtual void Hit(std::shared_ptr<Enemy>& tgt);
};

#endif //CANON_BALL_H
