#ifndef ENEMY_H
#define ENEMY_H

#include "AnimSprite.h"
#include "Map.h"
#include "Rectangle.h"

class Enemy : public AnimSprite
{
	typedef std::stack<Vector2f> Path;

	const Map* map;

	float speed;

	Path path;
	size_t blockSize;

	size_t numProjectiles;

	int life, initialLife;
	sfext::Rectangle hpBarGreen, hpBarRed;

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

	void Hit(int strength)
	{
		life -= strength;
		hpBarGreen.SetWidth(life * 3.0f);
		hpBarRed.SetWidth((initialLife - life) * 3.0f);
	}

	bool IsDead() const
	{
		return life <= 0;
	}

	void DrawHpBar(RenderTarget& target);

private:
	void FindPath(size_t tgtX, size_t tgtY);
};

#endif //ENEMY_H
