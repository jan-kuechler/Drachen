#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "AnimSprite.h"
#include "Enemy.h"

class Projectile : public AnimSprite
{
	std::weak_ptr<Enemy> target;

	float speed;
	bool hit;
	Vector2f targetPosition;

public:
	Projectile(std::weak_ptr<Enemy> target);

	void SetImage(const Image& img) /* override */;

	void Update(float elapsed) /* override */;

	bool DidHit() const
	{
		return hit;
	}
};

#endif //PROJECTILE_H
