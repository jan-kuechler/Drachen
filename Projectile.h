#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "AnimSprite.h"
#include "Enemy.h"

class Projectile : public AnimSprite
{
protected:
	std::weak_ptr<Enemy> target;

	float speed;
	float power;
	bool hit;
	Vector2f targetPosition;

public:
	Projectile(std::weak_ptr<Enemy> target, float power, float speed);

	void SetImage(const Image& img) /* override */;

	void Update(float elapsed) /* override */;

	bool DidHit() const
	{
		return hit;
	}

protected:
	virtual void Hit(std::shared_ptr<Enemy>& tgt);
};

#endif //PROJECTILE_H
