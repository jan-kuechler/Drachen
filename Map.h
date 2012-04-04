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

	bool drawOverlay;

	sf::Shape blockGreen, blockRed;
public:
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

	void DrawOverlay(bool doDraw)
	{
		drawOverlay = doDraw;
	}

	Vector2f BlockToPosition(Vector2i blk) const
	{
		return Vector2f((blk.x + 0.5f) * blockSize, (blk.y + 0.5f) * blockSize);
	}

	Vector2i PositionToBlock(Vector2f pos) const
	{
		return Vector2i(static_cast<int>(pos.x / blockSize), static_cast<int>(pos.y / blockSize));
	}

private:
};

#endif //MAP_H
