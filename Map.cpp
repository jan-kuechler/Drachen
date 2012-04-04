#include "pch.h"
#include "Map.h"
#include "Utility.h"

#include "json_spirit/json_spirit.h"

namespace fs = boost::filesystem;
namespace js = json_spirit;

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
	for (size_t i=0; i < width; ++i) {
		grid[i].resize(height);
	}

	js::mArray& gridData = rootObj["grid"].get_array();

	// NOTE: x/y reversed due to JSON layout
	for (size_t y=0; y < gridData.size(); ++y) {
		js::mArray& row = gridData[y].get_array();
		for (size_t x=0; x < row.size(); ++x) {
			grid[x][y] = row[x].get_bool(); 
		}
	}

	blockGreen = Shape::Rectangle(0, 0, blockSize, blockSize, Color(0, 255, 0, 64));
	blockRed   = Shape::Rectangle(0, 0, blockSize, blockSize, Color(255, 0, 0, 64));

	return true;
}

void Map::Draw(RenderTarget& target)
{
	target.Draw(bg);

	if (drawOverlay) {
		for (size_t x = 0; x < width; ++x) {
			for (size_t y = 0; y < height; ++y) {
				Shape& s = grid[x][y] ? blockGreen : blockRed;
				s.SetPosition(x * blockSize, y * blockSize);
				target.Draw(s);
			}
		}
	}
}
