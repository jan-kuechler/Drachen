#ifndef TOWER_SETTINGS_H
#define TOWER_SETTINGS_H

struct TowerSettings
{
	sf::Image *baseImage, *projectileImage;

	float range;
	float cooldown;

	struct Stage
	{
		float range, cooldown;
		Image *image, *projectile;

		int attacs;
		float power;
	};

	std::string type;
	std::string name;

	std::vector<Stage> stages;
};

#endif //TOWER_SETTINGS_H
