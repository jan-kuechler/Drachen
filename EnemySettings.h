#ifndef ENEMY_SETTINGS_H
#define ENEMY_SETTINGS_H

struct EnemySettings
{
	Image *image;
	size_t width, height;
	size_t offset;
	size_t numFrames;
	float frameTime;

	size_t life;
	float speed;
};

#endif //ENEMY_SETTINGS_H
