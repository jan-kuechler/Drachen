#ifndef ENEMY_H
#define ENEMY_H

#include "AnimSprite.h"
#include "Map.h"

class Enemy : public AnimSprite
{
	typedef std::stack<Vector2f> Path;

	const Map* map;

	float speed;

	Path path;
	size_t blockSize;

	size_t numProjectiles;
public:
	Enemy(const Map* map);

	void SetTarget(size_t x, size_t y);
	void SetSpeed(float v)
	{
		speed = v;
	}

	void Update(float elapsed) /*override*/;

	void AddProjectile()
	{
		numProjectiles++;
	}

	void ReleaseProjectile()
	{
#ifdef _DEBUG
		if (numProjectiles == 0)
			throw std::runtime_error("ReleaseProjectiles() called, but numProjectiles is 0");
#endif
		numProjectiles--;
	}

	size_t ProjectileCount() const
	{
		return numProjectiles;
	}

private:
	void FindPath(size_t tgtX, size_t tgtY);
};

#endif //ENEMY_H
