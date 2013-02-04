#ifndef GAME_STATUS_H
#define GAME_STATUS_H

#include "GlobalStatus.h"

struct GameStatus
{
	size_t lives;

	void Reset(const GlobalStatus& gs)
	{
		lives = gs.startLives;
	}
};

#endif //GAME_STATUS_H
