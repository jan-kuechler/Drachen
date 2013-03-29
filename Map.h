#ifndef MAP_H
#define MAP_H

class Map
{
	std::string prevMap;

	Sprite bg;

	size_t blockSize;
	std::vector<bool> pathGrid;

	// Only access these sets in Map.cpp, the operator < for Vector2i is only defined there.
	std::set<Vector2i> towers;
	std::vector<Vector2f> towerPlaces, origTowerPlaces;

	FloatRect targetArea;
	Vector2f defaultTarget;

	std::vector<Vector2f> spawnPlaces;

public:
	bool LoadFromFile(const std::string& map);
	void Reset();

	void Draw(RenderTarget& target);

	void PlaceTower(Vector2f pos);
	void RemoveTower(Vector2f pos);

	const std::vector<Vector2f>& GetTowerPlaces() const
	{
		return towerPlaces;
	}

	bool IsHighRange(const Vector2f& pos) const
	{
		return false; // FIXME
	}



	Vector2f GetSpawnPosition() const
	{
		return spawnPlaces[0];
	}



	Vector2f GetDefaultTarget() const
	{
		return defaultTarget;
	}

	bool IsInTargetArea(const Vector2f& pos) const
	{
		return targetArea.Contains(pos.x, pos.y);
	}



	Vector2f BlockToPosition(const Vector2i& blk) const
	{
		return Vector2f((blk.x + 0.5f) * blockSize, (blk.y + 0.5f) * blockSize);
	}

	Vector2i PositionToBlock(const Vector2f& pos) const
	{
		return Vector2i(static_cast<int>(pos.x / blockSize), static_cast<int>(pos.y / blockSize));
	}

	const std::vector<bool>& GetPathGrid() const
	{
		return pathGrid;
	}

	size_t GetBlockSize() const
	{
		return blockSize;
	}

	size_t GetWidthBlocks() const
	{
		return bg.GetImage()->GetWidth() / blockSize;
	}

	size_t GetHeightBlocks() const
	{
		return bg.GetImage()->GetHeight() / blockSize;
	}
};

#endif //MAP_H
