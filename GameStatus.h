#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "GlobalStatus.h"

struct GameStatus
{
	struct Wave
	{
		size_t countdown; // seconds to start the wave
		size_t enemies; // number of enemies to spawn
	};

	size_t lives;

	std::vector<Wave> waves;
	size_t currentWave, enemiesSpawned;
	enum {
		InCountdown, InSpawn, InWave,
	} waveState;

	// Call reset at the begin of Game::Reset before loading the level data
	void Reset(const GlobalStatus& gs)
	{
		lives = gs.startLives;
		waves.clear();
		currentWave = 0;
		waveState = InCountdown;
	}
};

#endif //GAME_STATUS_H
