#include "pch.h"
#include "GlobalStatus.h"
#include "Error.h"

#include <string>

#include "json_spirit\json_spirit.h"

namespace js = json_spirit;
namespace fs = boost::filesystem;

static const uint64_t GameStatusVersion = 1;

void GlobalStatus::Reset()
{
	level = "";
	levelIndex = 0;
	levelPack = "pack1";

	startLives = 6;
	startMoney = 200;

	moneyPerEnemy = 10;

	enabledPacks.insert(levelPack);
	lastWonLevel[levelPack] = -1;
}

void GlobalStatus::LoadFromFile(const std::string& fn)
{
	std::ifstream in(fn);
	assert(in.is_open());
	js::mValue rootValue;

	try {
		js::read_or_throw(in, rootValue);

		js::mObject& rootObj = rootValue.get_obj();

		uint64_t version = rootObj["version"].get_uint64();
		if (version != GameStatusVersion) {
			Reset();
			return;
		}

		js::mObject& gameStatus = rootObj["game-status"].get_obj();

		level = gameStatus["level"].get_str();
		levelIndex = gameStatus["level-index"].get_int();
		levelPack = gameStatus["level-pack"].get_str();

		startLives = gameStatus["start-lives"].get_int();
		startMoney = gameStatus["start-money"].get_int();
		moneyPerEnemy = gameStatus["money-per-enemy"].get_int();

		enabledPacks.clear();
		js::mArray& packs = gameStatus["enabled-packs"].get_array();
		for (size_t i=0; i < packs.size(); ++i) {
			enabledPacks.insert(packs[i].get_str());
		}

		lastWonLevel.clear();
		js::mObject& lastWon = gameStatus["last-won-level"].get_obj();
		for (auto it = lastWon.cbegin(); it != lastWon.cend(); ++it) {
			lastWonLevel.insert(std::make_pair<std::string, int>(it->first, it->second.get_int()));
		}

	}
	catch (js::Error_position err) {
		throw GameError() << ErrorInfo::Desc("Json format error") << ErrorInfo::Note(err.reason_) << boost::errinfo_at_line(err.line_) << boost::errinfo_file_name(fn);
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(fn);
	}

}

void GlobalStatus::WriteToFile(const std::string& fn)
{
	js::mObject rootObj;
	js::mObject gameStatusObj;

	rootObj["version"] = js::mValue(GameStatusVersion);

	rootObj["game-status"] = gameStatusObj;
	js::mObject& gameStatus = rootObj["game-status"].get_obj();

	gameStatus["level"] = js::mValue(level);
	gameStatus["level-index"] = js::mValue(static_cast<uint64_t>(levelIndex));
	gameStatus["level-pack"] = js::mValue(levelPack);

	gameStatus["start-lives"] = js::mValue(static_cast<uint64_t>(startLives));
	gameStatus["start-money"] = js::mValue(static_cast<uint64_t>(startMoney));
	gameStatus["money-per-enemy"] = js::mValue(static_cast<uint64_t>(moneyPerEnemy));

	js::mArray packs;
	for (auto it = enabledPacks.begin(); it != enabledPacks.end(); ++it) {
		packs.push_back(js::mValue(*it));
	}
	gameStatus["enabled-packs"] = packs;

	js::mObject lastWon;
	for (auto it = lastWonLevel.cbegin(); it != lastWonLevel.cend(); ++it) {
		lastWon[it->first] = js::mValue(it->second);
	}
	gameStatus["last-won-level"] = lastWon;

	std::ofstream out(fn);
	js::write_formatted(rootObj, out);
}
