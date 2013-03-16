#ifndef ENEMY_H
#define ENEMY_H

#include "AnimSprite.h"
#include "Map.h"
#include "Rectangle.h"
#include "EnemySettings.h"

class Enemy : public AnimSprite
{
	typedef std::stack<Vector2f> Path;

	const EnemySettings& settings;
	const Map* map;

	float speed;

	Path path;
	size_t blockSize;

	int life, initialLife;
	sfext::Rectangle hpBarGreen, hpBarRed;

	Vector2i target;
	bool atTarget, striked;

public:
	Enemy(const EnemySettings& settings, const Map* map);

	void SetTarget(const Vector2i& pos);
	Vector2i GetTarget() const
	{
		return target;
	}

	void SetSpeed(float v)
	{
		speed = v;
	}

	void Update(float elapsed) /*override*/;

	void Hit(int strength)
	{
		life -= strength;
		if (life < 0)
			life = 0;

		float greenPct = static_cast<float>(life) / initialLife;

		hpBarGreen.SetWidth(greenPct * 30);
		hpBarRed.SetWidth((1 - greenPct) * 30);
	}

	bool IsDead() const
	{
		return life <= 0;
	}

	bool IsAtTarget() const
	{
		return atTarget;
	}

	void Strike()
	{
		assert(atTarget);
		striked = true;
	}

	bool DidStrike() const
	{
		return striked;
	}

	bool IsIrrelevant() const
	{
		return IsDead() || DidStrike();
	}

	void DrawHpBar(RenderTarget& target);

private:
	void FindPath(size_t tgtX, size_t tgtY);
};

#endif //ENEMY_H
