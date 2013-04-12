#ifndef LEVEL_PACK_H
#define LEVEL_PACK_H

struct LevelPack
{
	std::string name;
	std::string desc;
	std::string image;
	std::vector<std::tuple<std::string,std::string>> levels;
	std::map<std::string,std::string> texts;
};

#endif //LEVEL_PACK_H
