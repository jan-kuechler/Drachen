#ifndef LEVEL_H
#define LEVEL_H

struct Level
{
	struct Wave
	{
		size_t countdown; // seconds to start the wave
		size_t maxTime; // maximal time for this wave
		std::vector<std::vector<size_t>> enemies; // enemies to spawn per spawn location
	};

	std::string name;
	std::string map;
	std::string theme;

	bool nightMode;

	std::vector<Wave> waves;

	void LoadFromFile(const boost::filesystem::path& path);
};

#endif //LEVEL_H
