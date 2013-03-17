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

		std::vector<Vector2f> attackPosition;
	};

	std::string type;
	std::string name;
	size_t baseCost;

	std::vector<Stage> stage;
};

#endif //TOWER_SETTINGS_H
