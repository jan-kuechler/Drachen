#ifndef ENEMY_H
#define ENEMY_H

#include "AnimSprite.h"
#include "Map.h"
#include "Rectangle.h"
#include "EnemySettings.h"

class Enemy : public AnimSprite
{
	typedef std::stack<Vector2f> Path;

	const Map* map;

	float speed;
	size_t moneyFactor;

	Path path;
	size_t blockSize;

	float life, initialLife;
	sfext::Rectangle hpBarGreen, hpBarRed;

	Vector2i target;
	bool atTarget, striked;

public:
	Enemy(const EnemySettings& settings, const Map* map);

	void SetTarget(const Vector2f& pos);

	void SetSpeed(float v)
	{
		speed = v;
	}

	void Update(float elapsed) /*override*/;

	void Hit(float power)
	{
		life -= power;
		if (life < 0)
			life = 0;

		float greenPct = life / initialLife;

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

	size_t GetMoneyFactor() const
	{
		return moneyFactor;
	}

	void DrawHpBar(RenderTarget& target);

private:
	void FindPath(size_t tgtX, size_t tgtY);
};

#endif //ENEMY_H
