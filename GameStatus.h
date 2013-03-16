#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "GlobalStatus.h"

struct GameStatus
{
	struct Wave
	{
		size_t countdown; // seconds to start the wave
		size_t maxTime; // maximal time for this wave
		std::queue<size_t> enemies; // enemies to spawn
	};

	size_t lives;
	size_t money;

	std::vector<Wave> waves;
	size_t currentWave;
	enum {
		InCountdown, InSpawn, InWave,
	} waveState;

	Clock spawnTimer, waveTimer, countdownTimer;

	// Call reset at the begin of Game::Reset before loading the level data
	void Reset(const GlobalStatus& gs)
	{
		lives = gs.startLives;
		money = gs.startMoney;
		waves.clear();
		currentWave = 0;
		waveState = InCountdown;
	}
};

#endif //GAME_STATUS_H
