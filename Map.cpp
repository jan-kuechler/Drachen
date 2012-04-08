#include "pch.h"
#include "Map.h"
#include "Utility.h"

#include "json_spirit/json_spirit.h"

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

Map::Map()
: drawOverlay(false), dbgTowersAnywhere(false)
{ }

bool Map::LoadFromFile(const std::string& fileName)
{
	fs::path filePath = fileName;
	fs::path base = filePath.parent_path();

	::LoadFromFile(bgImg, (base / "Background.png").string());
	bg.SetImage(bgImg);

	std::ifstream in(fileName.c_str());
	js::mValue rootValue;
	if (!js::read(in, rootValue))
		return false;

	if (rootValue.type() != js::obj_type)
		return false;

	js::mObject rootObj = rootValue.get_obj();

	width = rootObj["width"].get_int();
	height = rootObj["height"].get_int();
	blockSize = rootObj["block-size"].get_int();

	grid.resize(width);
	overlay.resize(width);
	for (size_t i=0; i < width; ++i) {
		grid[i].resize(height);
		overlay[i].resize(height);
	}

	js::mArray& gridData = rootObj["grid"].get_array();

	// NOTE: x/y reversed due to JSON layout
	for (size_t y=0; y < gridData.size(); ++y) {
		js::mArray& row = gridData[y].get_array();
		for (size_t x=0; x < row.size(); ++x) {
			grid[x][y] = row[x].get_bool(); 
		}
	}

	js::mArray& tp = rootObj["tower-places"].get_array();
	for (size_t i = 0; i < tp.size(); ++i) {
		js::mArray& p = tp[i].get_array();
		towerPlaces.insert(Vector2i(p[0].get_int(), p[1].get_int()));
	}

	UpdateOverlay();

	return true;
}

void Map::PlaceTower(const Vector2i& tpos)
{
	towers.insert(tpos);
}

bool Map::MayPlaceTower(const Vector2i& tpos) const
{
	return dbgTowersAnywhere || ((towerPlaces.find(tpos) != towerPlaces.end()) && (towers.find(tpos) == towers.end()));
}

void Map::UpdateOverlay()
{
	const Vector2f p1(0, 0);
	const Vector2f p2(static_cast<float>(blockSize), static_cast<float>(blockSize));
	for (size_t x = 0; x < width; ++x) {
		for (size_t y = 0; y < height; ++y) {
			if (grid[x][y])
				overlay[x][y] = Shape::Rectangle(p1, p2, Color(0, 255, 0, 64));
			else if (MayPlaceTower(BlockToTowerPos(Vector2i(x, y))))
				overlay[x][y] = Shape::Rectangle(p1, p2, Color(0, 0, 255, 64));
			else
				overlay[x][y] = Shape::Rectangle(p1, p2, Color(255, 0, 0, 64));

			overlay[x][y].SetPosition(static_cast<float>(x * blockSize), static_cast<float>(y * blockSize));
		}
	}
}

void Map::Draw(RenderTarget& target)
{
	target.Draw(bg);

	if (drawOverlay) {
		for (size_t x = 0; x < width; ++x) {
			for (size_t y = 0; y < height; ++y) {
				target.Draw(overlay[x][y]);
			}
		}
	}
}
