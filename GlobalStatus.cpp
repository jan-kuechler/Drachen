#include "pch.h"
#include "GlobalStatus.h"
#include "Error.h"

#include <string>

#include "json_spirit/json_spirit.h"
#include "jsex.h"

namespace js = json_spirit;
namespace fs = boost::filesystem;

static const uint64_t GameStatusVersion = 4;

GlobalStatus::Debug::Debug()
: enabled(false)
{ }

void GlobalStatus::Reset()
{
	startLives = 6;
	startMoney = 200;

	moneyPerEnemy = 10;

	lastPack = 0;

	packInfo.clear();

	settings.useShader = true;
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

		startLives = gameStatus["start-lives"].get_int();
		startMoney = gameStatus["start-money"].get_int();
		moneyPerEnemy = gameStatus["money-per-enemy"].get_int();

		lastPack = gameStatus["last-pack"].get_int();

		enabledPacks.clear();
		js::mArray& packs = gameStatus["enabled-packs"].get_array();
		for (size_t i=0; i < packs.size(); ++i) {
			enabledPacks.insert(packs[i].get_str());
		}

		js::mObject& packInfoObj = gameStatus["pack-info"].get_obj();
		for (const auto& it: packInfoObj) {
			const js::mObject& thisInfo = it.second.get_obj();

			PackInfo info;
			info.lastWonLevel = jsex::get<int>(thisInfo.at("last-won-level"));
			info.textsRead = jsex::read_set<std::string>(thisInfo.at("texts-read").get_array());

			packInfo[it.first] = info;
		}

		js::mObject& set = gameStatus["settings"].get_obj();
		settings.useShader = jsex::get<bool>(set["use-shader"]);

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

	gameStatus["start-lives"] = js::mValue(static_cast<uint64_t>(startLives));
	gameStatus["start-money"] = js::mValue(static_cast<uint64_t>(startMoney));
	gameStatus["money-per-enemy"] = js::mValue(static_cast<uint64_t>(moneyPerEnemy));

	gameStatus["last-pack"] = js::mValue(static_cast<uint64_t>(lastPack));

	js::mArray packs;
	for (auto it = enabledPacks.begin(); it != enabledPacks.end(); ++it) {
		packs.push_back(js::mValue(*it));
	}
	gameStatus["enabled-packs"] = packs;

	js::mObject packInfoObj;
	for (const auto& it: packInfo) {
		js::mObject thisPackInfo;
		thisPackInfo["last-won-level"] = js::mValue(it.second.lastWonLevel);
		thisPackInfo["texts-read"] = jsex::write_set(it.second.textsRead);
		packInfoObj[it.first] = thisPackInfo;
	}
	gameStatus["pack-info"] = packInfoObj;

	js::mObject set;

	set["use-shader"] = js::mValue(settings.useShader);

	gameStatus["settings"] = set;

	std::ofstream out(fn);
	js::write_formatted(rootObj, out);
}
