#include "pch.h"
#include "Error.h"
#include "Map.h"
#include "Utility.h"
#include "DataPaths.h"
#include "ResourceManager.h"

#include "json_spirit/json_spirit.h"
#include "jsex.h"

namespace fs = boost::filesystem;
namespace js = json_spirit;

namespace sf {

	// Compare operator for the Map::towerPlaces and Map::towers sets.
	bool operator < (const sf::Vector2i& a, const sf::Vector2i& b)
	{
		if (a.x == b.x) {
			return a.y < b.y;
		}
		return a.x < b.x;
	}
}

bool Map::LoadFromFile(const std::string& map)
{
	if (map == prevMap)
		return true; // everything already loaded

	fs::path base = GetMapPath(map);
	fs::path filePath = base / MapDefinitionFile;

	bg.SetImage(gImageManager.getResource((base/"background.png").string()));

	std::ifstream in(filePath.string());
	js::mValue rootValue;
	try {
		js::read_or_throw(in, rootValue);
	}
	catch (js::Error_position err) {
		throw GameError() << ErrorInfo::Desc("Invalid json file") << ErrorInfo::Note(err.reason_) << boost::errinfo_at_line(err.line_) << boost::errinfo_file_name(filePath.string());
	}

	if (rootValue.type() != js::obj_type)
		throw GameError() << ErrorInfo::Desc("Root value is not an object");

	js::mObject rootObj = rootValue.get_obj();

	try {
		towerPlaces = jsex::read_vector<Vector2f>(rootObj["tower-places"].get_array());
		origTowerPlaces = towerPlaces;
		spawnPlaces = jsex::read_vector<Vector2f>(rootObj["spawn-places"].get_array());

		targetArea = jsex::get<FloatRect>(rootObj["target-area"]);
		defaultTarget = GetCenter(targetArea);

		js::mObject& path = rootObj["path"].get_obj();
		blockSize = jsex::get<size_t>(path["block-size"]);

		pathGrid = jsex::read_vector<bool>(path["grid"].get_array());
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(filePath.string());
	}

	prevMap = map;
	
	return true;
}

void Map::Reset()
{
	towerPlaces = origTowerPlaces;
}

void Map::PlaceTower(Vector2f pos)
{
	assert(boost::range::find(towerPlaces, pos) != towerPlaces.end());
	towerPlaces.erase(boost::remove(towerPlaces, pos), towerPlaces.end());
}

void Map::RemoveTower(Vector2f pos)
{
	towerPlaces.push_back(pos);
}

void Map::Draw(RenderTarget& target)
{
	target.Draw(bg);
}
