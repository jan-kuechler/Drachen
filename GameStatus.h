#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "GlobalStatus.h"

struct GameStatus
{
	size_t lives;
	size_t money;

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
		currentWave = 0;
		waveState = InCountdown;
	}
};

#endif //GAME_STATUS_H
