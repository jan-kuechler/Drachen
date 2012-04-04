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

public:
	void SetMap(const Map* m)
	{
		map = m;
	}

	void SetTarget(size_t x, size_t y);
	void SetSpeed(float v)
	{
		speed = v;
	}

	void Update(float elapsed) /*override*/;

private:
	void FindPath(size_t tgtX, size_t tgtY);
};

#endif //ENEMY_H
