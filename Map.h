#ifndef MAP_H
#define MAP_H

class Map
{
public:
	typedef std::vector<std::vector<bool> > Grid;

private:
	Image bgImg;
	Sprite bg;

	size_t width, height;
	size_t blockSize;
	Grid grid;

	// Only access these sets in Map.cpp, the operator < for Vector2i is only defined there.
	std::set<Vector2i> towerPlaces;
	std::set<Vector2i> towers;

	bool drawOverlay;

	sf::Shape blockGreen, blockRed, blockBlue;
	std::vector<std::vector<Shape> > overlay;

	bool dbgTowersAnywhere;
public:
	Map();

	bool LoadFromFile(const std::string& fileName);

	const Grid& GetGrid() const
	{
		return grid;
	}

	size_t GetBlockSize() const
	{
		return blockSize;
	}

	size_t GetWidth() const
	{
		return width;
	}

	size_t GetHeight() const
	{
		return height;
	}

	void Draw(RenderTarget& target);

	void ToggleOverlay()
	{
		drawOverlay = !drawOverlay;
	}

	bool MayPlaceTower(const Vector2i& tpos) const;
	void PlaceTower(const Vector2i& tpos);

	Vector2f BlockToPosition(const Vector2i& blk) const
	{
		return Vector2f((blk.x + 0.5f) * blockSize, (blk.y + 0.5f) * blockSize);
	}

	Vector2i BlockToTowerPos(const Vector2i& blk) const
	{
		return blk / 2;
	}

	Vector2f TowerPosToPosition(const Vector2i& tp) const
	{
		return Vector2f((tp.x + 0.5f) * 2 * blockSize, (tp.y + 0.5f) * 2 * blockSize);
	}

	Vector2i PositionToBlock(const Vector2f& pos) const
	{
		return Vector2i(static_cast<int>(pos.x / blockSize), static_cast<int>(pos.y / blockSize));
	}

	Vector2i PostionToTowerPos(const Vector2f& pos) const
	{
		return Vector2i(static_cast<int>(pos.x / (2*blockSize)), static_cast<int>(pos.y / (2*blockSize)));
	}

	void DebugToggleTowersAnywhere()
	{
		dbgTowersAnywhere = !dbgTowersAnywhere;
	}

private:
	void UpdateOverlay();
};

#endif //MAP_H
