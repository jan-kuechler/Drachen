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

Vector2f GetVector2f(js::mArray& arr)
{
	return Vector2f(static_cast<float>(arr[0].get_real()), static_cast<float>(arr[1].get_real()));
}

template <typename T>
void GetOptInt(T& var, js::mObject& obj, const std::string& name)
{
	if (obj.count(name))
		var = obj[name].get_int();
}

template <typename T>
void GetOptFloat(T& var, js::mObject& obj, const std::string& name)
{
	if (obj.count(name))
		var = static_cast<float>(obj[name].get_real());
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
			settings->baseCost = def["base-cost"].get_int();

			js::mArray& stages = def["stages"].get_array();
			settings->stage.resize(stages.size());
			for (size_t j=0; j < stages.size(); ++j) {
				js::mObject& stage = stages[j].get_obj();

				settings->stage[j].image = &gImageManager.getResource((themePath / stage["base"].get_str()).string());
				settings->stage[j].projectile = &gImageManager.getResource((themePath / stage["projectile"].get_str()).string());
				settings->stage[j].center = GetVector2f(stage["center"].get_array());

				GetOptFloat(settings->stage[j].range, stage, "range");
				GetOptFloat(settings->stage[j].cooldown, stage, "cooldown");
				GetOptInt(settings->stage[j].attacks, stage, "attacks");
				GetOptFloat(settings->stage[j].power, stage, "power");
				GetOptFloat(settings->stage[j].speed, stage, "speed");
				GetOptFloat(settings->stage[j].splashPower, stage, "splash-power");
				GetOptFloat(settings->stage[j].splashRange, stage, "splash-range");

				js::mArray& attackPosition = stage["attack-position"].get_array();
				settings->stage[j].attackPosition.resize(attackPosition.size());
				for (size_t k=0; k < attackPosition.size(); ++k) {
					settings->stage[j].attackPosition[k] = GetVector2f(attackPosition[k].get_array());
				}

				
			}
		}
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(towerDef.string());
	}
}

std::string Theme::GetFileName(const std::string& path, int idx) const
{
	auto val = TraversePath(path, idx);
	assert(std::get<0>(val));
	std::string fileName = std::get<1>(val).get_str();
	return (GetThemePath(currentTheme) / fileName).string();
}


sf::Color Theme::GetColor(const std::string& path, int idx) const
{
	auto value = TraversePath(path, idx);
	assert(std::get<0>(value));
	js::mValue& val = std::get<1>(value);
	if (val.type() == js::array_type) {
		js::mArray arr = val.get_array();
		if (arr.size() == 4)
			return Color(arr.at(0).get_int(), arr.at(1).get_int(), arr.at(2).get_int(), arr.at(3).get_int());
		else
			return Color(arr.at(0).get_int(), arr.at(1).get_int(), arr.at(2).get_int());
	}
	auto str = val.get_str();
	boost::to_lower(str);
	if (str == "black")
		return Color::Black;
	if (str == "blue")
		return Color::Blue;
	if (str == "cyan")
		return Color::Cyan;
	if (str == "green")
		return Color::Green;
	if (str == "magenta")
		return Color::Magenta;
	if (str == "red")
		return Color::Red;
	if (str == "white")
		return Color::White;
	if (str == "yellow")
		return Color::Yellow;
	return Color::Black;
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

static const js::mValue DUMMY_VALUE;

bool CheckExistsAndType(const std::string& name, const js::mObject* p, js::Value_type type)
{
	return p->count(name) && p->at(name).type() == type;
}

std::tuple<bool, json_spirit::mValue> Theme::TraversePath(const std::string& path, int idx) const
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
			if (!CheckExistsAndType(name, parent, js::obj_type))
				return std::make_tuple(false, DUMMY_VALUE);
			parent = &parent->at(name).get_obj();
		}
		else {
			if (!CheckExistsAndType(name, parent, js::array_type))
				return std::make_tuple(false, DUMMY_VALUE);
			const js::mArray& arr = parent->at(name).get_array();
			if (arrayIndex >= static_cast<int>(arr.size()))
				return std::make_tuple(false, DUMMY_VALUE);
			parent = &arr.at(arrayIndex).get_obj();
		}
	}

	bool arrayAccess = false;
	int arrayIndex = 0;
	std::tie(last, arrayAccess, arrayIndex) = GetArrayAccess(last, idx);

	if (!parent->count(last))
		return std::make_tuple(false, DUMMY_VALUE);
	if (!arrayAccess) {
		return std::make_tuple(true, parent->at(last));
	}
	else {
		if (parent->at(last).type() != js::array_type || arrayIndex >= static_cast<int>(parent->at(last).get_array().size()))
			return std::make_tuple(false, DUMMY_VALUE);
		return std::make_tuple(true, parent->at(last).get_array().at(arrayIndex));
	}
}
