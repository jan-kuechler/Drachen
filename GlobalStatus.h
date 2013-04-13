#ifndef GLOBAL_STATUS_H
#define GLOBAL_STATUS_H

#include "State.h"

struct GlobalStatus
{
	size_t startLives;
	size_t startMoney;

	size_t moneyPerEnemy;

	size_t lastPack;

	struct PackInfo
	{
		int lastWonLevel;
		std::set<std::string> textsRead;

		PackInfo()
		: lastWonLevel(-1)
		{ }
	};
	std::map<std::string,PackInfo> packInfo;

	struct Settings
	{
		bool useShader;
	
	} settings;

	struct Debug
	{
		bool enabled;

		Debug();
	} debug; // does not get saved

	struct RunTime
	{
		std::string level;
		size_t levelIndex;
		std::string levelPack;

		struct 
		{
			std::string text;
			State nextState;
		} textDisplay;

		struct LevelPicker
		{
			int commingFromLevel;
			bool didWin;

			LevelPicker()
			: commingFromLevel(-1), didWin(false)
			{ }
		} levelPicker;

	} runTime; // does not get saved

	void Reset();
	void LoadFromFile(const std::string& fn);
	void WriteToFile(const std::string& fn);
};

extern GlobalStatus gStatus;

#endif //GLOBAL_STATUS_H
