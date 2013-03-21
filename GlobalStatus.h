#ifndef GLOBAL_STATUS_H
#define GLOBAL_STATUS_H

struct GlobalStatus
{
	std::string level;
	std::string levelPack;

	size_t startLives;
	size_t startMoney;

	size_t moneyPerEnemy;
};

extern GlobalStatus gStatus;

#endif //GLOBAL_STATUS_H
