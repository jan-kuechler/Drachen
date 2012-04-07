#include "pch.h"
#include "Map.h"
#include "Utility.h"

#include "json_spirit/json_spirit.h"

namespace fs = boost::filesystem;
namespace js = json_spirit;

namespace sf {
	bool operator < (const sf::Vector2i& a, const sf::Vector2i& b)
	{
		if (a.x == b.x) {
			return a.y < b.y;
		}
		return a.x < b.x;
	}
}

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

	blockGreen = Shape::Rectangle(0, 0, blockSize, blockSize, Color(0, 255, 0, 64));
	blockRed   = Shape::Rectangle(0, 0, blockSize, blockSize, Color(255, 0, 0, 64));
	blockBlue  = Shape::Rectangle(0, 0, blockSize, blockSize, Color(0, 0, 255, 64));
	UpdateOverlay();

	return true;
}

void Map::PlaceTower(const Vector2i& tpos)
{
	towers.insert(tpos);
	UpdateOverlay();
}

bool Map::MayPlaceTower(const Vector2i& tpos) const
{
	return (towerPlaces.find(tpos) != towerPlaces.end()) && (towers.find(tpos) == towers.end());
}

void Map::UpdateOverlay()
{
	for (size_t x = 0; x < width; ++x) {
		for (size_t y = 0; y < height; ++y) {
			if (grid[x][y])
				overlay[x][y] = &blockGreen;
			else if (MayPlaceTower(BlockToTowerPos(Vector2i(x, y))))
				overlay[x][y] = &blockBlue;
			else
				overlay[x][y] = &blockRed;
		}
	}
}

void Map::Draw(RenderTarget& target)
{
	target.Draw(bg);

	if (drawOverlay) {
		for (size_t x = 0; x < width; ++x) {
			for (size_t y = 0; y < height; ++y) {
				overlay[x][y]->SetPosition(x * blockSize, y * blockSize);
				target.Draw(*overlay[x][y]);
			}
		}
	}
}
