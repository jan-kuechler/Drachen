#ifndef DATA_PATHS_H
#define DATA_PATHS_H

static inline boost::filesystem::path GetLevelPath(const std::string& level)
{
	return boost::filesystem::path("data/levels") / level;
}

static inline boost::filesystem::path GetThemePath(const std::string& theme)
{
	return boost::filesystem::path("data/themes") / theme;
}

#endif //DATA_PATHS_H
