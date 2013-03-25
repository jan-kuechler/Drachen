#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

class MapEditor
{
	enum Mode
	{
		M_NONE, M_PATH,
	};

	RenderWindow& window;

	Sprite background;

	Image gridOverlayImg;
	Sprite gridOverlay;

	std::string bgImage;

	Mode mode;

	std::vector<bool> pathGrid;
	size_t gridWidth, gridHeight;
	size_t gridSize;

	bool leftDown, rightDown;

public:
	MapEditor(RenderWindow& win);

	void Run();

private:
	void HandleEvents();
	void Draw();

	void HandleKey(Event::KeyEvent key);
	void HandleMouse(Event& event);

	void LoadBackground();
	void UpdateGrid();
};

#endif //MAP_EDITOR_H
