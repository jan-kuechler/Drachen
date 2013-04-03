#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

class MapEditor
{
	enum Mode
	{
		M_NONE, M_PATH, M_TOWER, M_SPAWN, M_TARGET, M_FIRE,
	};

	RenderWindow& window;

	Image defBgImg;
	Sprite background;

	Image gridOverlayImg;
	Sprite gridOverlay;
	String gridInfo;

	Mode mode;

	// path mode
	std::vector<bool> pathGrid;
	size_t gridWidth, gridHeight;
	size_t gridSize;

	bool leftDown, rightDown;

	// tower mode
	std::vector<Vector2f> towerPlaces;
	std::vector<Vector2f> highRangePlaces;
	std::vector<Shape> towerPlaceMarker;

	bool isGoodPlace, placeSnapped;
	Shape goodPlace, badPlace, snappedPlace;

	// spawn mode
	std::vector<Vector2f> spawnPlaces;
	std::vector<Shape> spawnPlaceMarker;

	// fire mode
	std::vector<Vector2f> firePlaces;
	std::vector<Shape> firePlaceMarker;

	// target mode
	Vector2f targetFrom, targetTo;
	Shape targetAreaMarker;
	bool showTarget;
	bool mouseDown;

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
	void UpdatePath();

	void UpdateMarker(Mode md);

	void LoadMap(boost::filesystem::path path);
	void SaveMap();
};

#endif //MAP_EDITOR_H
