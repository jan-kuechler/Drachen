#ifndef GLOBAL_STATUS_H
#define GLOBAL_STATUS_H

struct GlobalStatus
{
	std::string level;
	size_t levelIndex;
	std::string levelPack;

	size_t startLives;
	size_t startMoney;

	size_t moneyPerEnemy;

	std::set<std::string> enabledPacks;
	std::map<std::string,int> lastWonLevel;
};

extern GlobalStatus gStatus;

#endif //GLOBAL_STATUS_H
