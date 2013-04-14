#ifndef TOWER_H
#define TOWER_H

#include "AnimSprite.h"
#include "Enemy.h"
#include "Map.h"
#include "Projectile.h"
#include "TowerSettings.h"

class Tower : public AnimSprite
{
protected:
	const std::vector<std::shared_ptr<Enemy>>& enemies;
	std::vector<std::unique_ptr<Projectile>>& projectiles;

	bool hasHighRange;

	Shape rangeCircle;

	float  range;
	float  cooldown;
	size_t attacks;
	float  power;
	
	float cooldownTimer;

	const TowerSettings* settings;
	size_t stage;

	bool isSold;

public:
	static std::unique_ptr<Tower> CreateTower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles, bool highRange);

	void Update(float elapsed) /* override */;

	bool CanUpgrade()
	{
		return stage < (settings->stage.size() - 1);
	}

	void Upgrade();

	size_t Sell()
	{
		isSold = true;
		return settings->baseCost;
	}

	bool IsSold()
	{
		return isSold;
	}

	void DrawRangeCircle(RenderWindow& tgt)
	{
		tgt.Draw(rangeCircle);
	}

	void SetPosition(float x, float y)
	{
		AnimSprite::SetPosition(x, y);
		rangeCircle.SetPosition(x, y);
	}

	void SetPosition(Vector2f pos)
	{
		AnimSprite::SetPosition(pos);
		rangeCircle.SetPosition(pos);
	}

	const TowerSettings* GetSettings() const
	{
		return settings;
	}

protected:
	Tower(const TowerSettings* settings, const std::vector<std::shared_ptr<Enemy>>& enemies, std::vector<std::unique_ptr<Projectile>>& projectiles, bool highRange);

	virtual void ApplyStage();
	virtual void Attack();
};

#endif //TOWER_H
