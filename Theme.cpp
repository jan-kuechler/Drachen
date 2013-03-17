#include "pch.h"
#include "DataPaths.h"
#include "Theme.h"
#include "Utility.h"
#include "DataPaths.h"
#include "ResourceManager.h"

namespace fs = boost::filesystem;
namespace js = json_spirit;

void Theme::LoadTheme(const std::string& name)
{
	if (currentTheme == name)
		return;

	fs::path themePath = GetThemePath(name);
	fs::path themeDef = themePath / ThemeDefinitionFile;

	std::ifstream in(themeDef.string());
	js::mValue rootValue;
	try {
		js::read_or_throw(in, rootValue);
	}
	catch (js::Error_position err) {
		throw GameError() << ErrorInfo::Desc("Invalid json file") << ErrorInfo::Note(err.reason_) << boost::errinfo_at_line(err.line_) << boost::errinfo_file_name(themeDef.string());
	}
	if (rootValue.type() != js::obj_type)
		throw GameError() << ErrorInfo::Desc("Root value is not an object");

	rootObj = rootValue.get_obj();
	currentTheme = name;
	try {
		LoadFromFile(mainFont, (themePath / rootObj["main-font"].get_str()).string());
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(themeDef.string());
	}

	LoadTowerSettings();
}

void Theme::LoadTowerSettings()
{
	fs::path themePath = GetThemePath(currentTheme);
	fs::path towerDef = themePath / TowerDefinitionFile;

	std::ifstream in(towerDef.string());
	js::mValue rootValue;
	try {
		js::read_or_throw(in, rootValue);
	}
	catch (js::Error_position err) {
		throw GameError() << ErrorInfo::Desc("Invalid json file") << ErrorInfo::Note(err.reason_) << boost::errinfo_at_line(err.line_) << boost::errinfo_file_name(towerDef.string());
	}
	if (rootValue.type() != js::obj_type)
		throw GameError() << ErrorInfo::Desc("Root value is not an object");

	js::mObject& rootObject = rootValue.get_obj();

	try {
		js::mArray& towers = rootObject["towers"].get_array();

		towerSettings.clear();
		towerSettings.resize(towers.size());
		for (size_t i=0; i < towers.size(); ++i) {
			js::mObject& def = towers[i].get_obj();

			TowerSettings* settings = &towerSettings[i];

			settings->name = def["name"].get_str();
			settings->type = def["type"].get_str();

			js::mArray& stages = def["stages"].get_array();
			settings->stages.resize(stages.size());
			for (size_t j=0; j < stages.size(); ++j) {
				js::mObject& stage = stages[j].get_obj();

				settings->stages[j].image = &gImageManager.getResource((themePath / stage["base"].get_str()).string());
				settings->stages[j].projectile = &gImageManager.getResource((themePath / stage["projectile"].get_str()).string());

				settings->stages[j].range = static_cast<float>(stage["range"].get_real());
				settings->stages[j].cooldown = static_cast<float>(stage["cooldown"].get_real());
				settings->stages[j].attacks = stage["attacks"].get_int();
				settings->stages[j].power = static_cast<float>(stage["power"].get_real());
			}
		}
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(towerDef.string());
	}
}

std::string Theme::GetFileName(const std::string& path, int idx) const
{
	std::string fileName = TraversePath(path, idx).get_str();
	return (GetThemePath(currentTheme) / fileName).string();
}

std::tuple<std::string, bool, int> GetArrayAccess(const std::string& part, int idx)
{
	size_t bracketPos = part.find('[');
	if (bracketPos != part.npos) {
		std::string name = part.substr(0, bracketPos);
		std::string index = part.substr(bracketPos+1);
		index.pop_back(); // remove the closing ]

		int arrayIndex = idx;
		if (!index.empty())
			arrayIndex = boost::lexical_cast<int>(index);

		return std::make_tuple(name, true, arrayIndex);
	}

	return std::make_tuple(part, false, 0);
}

const js::mValue& Theme::TraversePath(const std::string& path, int idx) const
{
	std::vector<std::string> parts;
	boost::split(parts, path, boost::is_any_of("/"));
	std::string last = parts.back();
	parts.pop_back();

	const js::mObject* parent = &rootObj;
	for (auto it = parts.begin(); it != parts.end(); ++it) {
		bool arrayAccess = false;
		int arrayIndex = 0;
		std::string name = *it;

		std::tie(name, arrayAccess, arrayIndex) = GetArrayAccess(*it, idx);

		if (!arrayAccess) {
			parent = &parent->at(name).get_obj();
		}
		else {
			const js::mArray& arr = parent->at(name).get_array();
			parent = &arr.at(arrayIndex).get_obj();
		}
	}

	bool arrayAccess = false;
	int arrayIndex = 0;
	std::tie(last, arrayAccess, arrayIndex) = GetArrayAccess(last, idx);

	if (!arrayAccess)
		return parent->at(last);
	else
		return parent->at(last).get_array().at(arrayIndex);
}
