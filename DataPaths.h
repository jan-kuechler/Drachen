#ifndef DATA_PATHS_H
#define DATA_PATHS_H

static const std::string LevelDefinitionFile = "level.js";
static const std::string MapDefinitionFile = "map.js";
static const std::string ThemeDefinitionFile = "theme.js";
static const std::string EnemyDefinitionFile = "enemies.js";
static const std::string TowerDefinitionFile = "towers.js";
static const boost::filesystem::path LevelPacksFile = "data/levels/packs.js";

static inline boost::filesystem::path GetLevelPath(const std::string& level)
{
	auto pos = level.find_last_of('/');
	if (pos != level.npos) {
		auto levelDir = level.substr(0, pos);
		return boost::filesystem::path("data/levels") / levelDir;
	}
	return boost::filesystem::path("data/levels");
}

static inline boost::filesystem::path GetLevelFile(const std::string& level)
{
	auto pos = level.find_last_of('/');
	if (pos != level.npos) {
		auto levelFile = level.substr(pos+1);
		return levelFile;
	}
	return level;
}

static inline boost::filesystem::path GetLevelPackFile(const std::string& path)
{
	return boost::filesystem::path("data/levels") / path;
}

static inline boost::filesystem::path GetMapPath(const std::string& map)
{
	return boost::filesystem::path("data/maps") / map;
}

static inline boost::filesystem::path GetThemePath(const std::string& theme)
{
	return boost::filesystem::path("data/themes") / theme;
}

static inline boost::filesystem::path GetStatusFile()
{
	return boost::filesystem::path("drachen.st");
}

#endif //DATA_PATHS_H
