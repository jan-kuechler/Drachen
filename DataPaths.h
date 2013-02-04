#ifndef DATA_PATHS_H
#define DATA_PATHS_H

static const std::string LevelDefinitionFile = "level.js";
static const std::string MapDefinitionFile = "map.js";
static const std::string ThemeDefinitionFile = "theme.js";

static inline boost::filesystem::path GetLevelPath(const std::string& level)
{
	return boost::filesystem::path("data/levels") / level;
}

static inline boost::filesystem::path GetMapPath(const std::string& map)
{
	return boost::filesystem::path("data/maps") / map;
}

static inline boost::filesystem::path GetThemePath(const std::string& theme)
{
	return boost::filesystem::path("data/themes") / theme;
}

#endif //DATA_PATHS_H
