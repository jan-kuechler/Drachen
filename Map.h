#ifndef MAP_H
#define MAP_H

class Map
{
public:
	enum Cell {
		Nothing   = 0,
		Path      = 1,
		Tower     = 2,
		HighTower = 3,
	};

	typedef std::vector<std::vector<Cell> > Grid;

private:
	std::string prevMap;

	Image bgImg;
	Sprite bg;

	size_t width, height;
	size_t blockSize;
	Grid grid;

	// Only access these sets in Map.cpp, the operator < for Vector2i is only defined there.
	std::set<Vector2i> towers;

	std::set<Vector2i> targetPlaces;
	Vector2i defaultTarget;

	Vector2f spawnPosition; // only used as a screen position, no need to keep the block

	bool drawOverlay;

	sf::Shape blockGreen, blockRed, blockBlue;
	std::vector<std::vector<Shape> > overlay;

	bool dbgTowersAnywhere;

public:
	Map();

	bool LoadFromFile(const std::string& map);
	void Reset();

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

	bool IsHighRangeBlock(const Vector2i& tpos) const
	{
		return grid[tpos.x][tpos.y] == HighTower;
	}

	Vector2i GetDefaultTarget() const
	{
		return defaultTarget;
	}

	Vector2f GetSpawnPosition() const
	{
		return spawnPosition;
	}

	bool IsInTargetArea(const Vector2i& blk) const;

	Vector2f BlockToPosition(const Vector2i& blk) const
	{
		return Vector2f((blk.x + 0.5f) * blockSize, (blk.y + 0.5f) * blockSize);
	}

	Vector2i PositionToBlock(const Vector2f& pos) const
	{
		return Vector2i(static_cast<int>(pos.x / blockSize), static_cast<int>(pos.y / blockSize));
	}

	void DebugToggleTowersAnywhere()
	{
		dbgTowersAnywhere = !dbgTowersAnywhere;
	}

private:
	void UpdateOverlay();
};

#endif //MAP_H
