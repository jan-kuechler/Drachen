#ifndef TOWER_SETTINGS_H
#define TOWER_SETTINGS_H

static const float HIGH_RANGE_FACTOR = 1.5f;

struct TowerSettings
{
	struct Stage
	{
		float range, cooldown;
		Image *image, *projectile;
		Vector2f center;

		int attacks;
		float power;
		float speed;

		float splashPower;
		float splashRange;

		std::vector<Vector2f> attackPosition;

		Stage()
		: range(25.f), cooldown(2.f), attacks(1), power(1.f), speed(100.f), splashPower(0), splashRange(0)
		{ }
	};

	std::string type;
	std::string name;
	size_t baseCost;

	std::vector<Stage> stage;
};

#endif //TOWER_SETTINGS_H
