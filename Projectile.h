#ifndef PROJECTILE_H
#define PROJECTILE_H

#include "AnimSprite.h"

class Projectile : public AnimSprite
{
	const AnimSprite* target;

	float speed;
	bool hit;
public:
	Projectile(const AnimSprite* target);

	void SetImage(const Image& img) /* override */;

	void Update(float elapsed) /* override */;

	bool DidHit() const
	{
		return hit;
	}
};

#endif //PROJECTILE_H
