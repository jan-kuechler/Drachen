#include "pch.h"
#include "MapEditor.h"
#include "FileDlg.h"
#include "../ResourceManager.h"
#include "../Utility.h"
#include "../json_spirit/json_spirit.h"

namespace fs = boost::filesystem;
namespace js = json_spirit;

static Color ColorPath(255, 255, 0, 64);
static Color ColorTransparent(0, 0, 0, 0);

static Color ColorPlaceGood(0, 255, 0, 64);
static Color ColorPlaceBad(255, 0, 0, 64);
static Color ColorPlaceSnapped(255, 0, 0);
static Color ColorPlace(0, 0, 255, 128);
static Color ColorHighRange(255, 0, 255, 128);
static Color ColorSpawn(255, 255, 0, 128);

static Color ColorTarget(0, 255, 255, 128);

static const float MIN_PLACE_DIST = 30;
static const float PLACE_RADIUS = 12.5;

MapEditor::MapEditor(RenderWindow& win)
: window(win), mode(M_NONE), gridSize(25), leftDown(false), rightDown(false), mouseDown(false), showTarget(false)
{ 
	defBgImg.Create(win.GetWidth(), win.GetHeight(), Color::White);
	background.SetImage(defBgImg);

	gridOverlayImg.Create(win.GetWidth(), win.GetHeight(), ColorTransparent);
	gridOverlay.SetImage(gridOverlayImg);

	goodPlace = Shape::Circle(Vector2f(0, 0), PLACE_RADIUS, ColorPlaceGood);
	badPlace = Shape::Circle(Vector2f(0, 0), PLACE_RADIUS, ColorPlaceBad);
	snappedPlace = Shape::Circle(Vector2f(0, 0), PLACE_RADIUS, ColorPlaceSnapped);

	gridInfo.SetPosition(650, 25);
	gridInfo.SetSize(20.f);

	UpdateGrid();
}

void MapEditor::Run()
{
	HandleEvents();
	Draw();
}

void MapEditor::HandleEvents()
{
	Event event;

	while (window.GetEvent(event)) {
		switch (event.Type) {
		case Event::Closed:
			window.Close();
			break;

		case Event::KeyReleased:
			HandleKey(event.Key);
			break;

		case Event::MouseButtonPressed:
		case Event::MouseButtonReleased:
		case Event::MouseMoved:
			HandleMouse(event);
			break;

		case Event::MouseWheelMoved:
			if (mode == M_PATH) {
				int res = gridSize;
				res += event.MouseWheel.Delta;
				if (res < 10)
					res = 10;
				if (res > 100)
					res = 100;

				gridSize = res;
				UpdateGrid();
			}
			break;
		}
	}
}

void MapEditor::HandleKey(Event::KeyEvent key)
{
	switch (key.Code) {
	case Key::Escape:
		mode = M_NONE;
		break;

	case Key::S:
		SaveMap();
		mode = M_NONE;
		break;

	case Key::L:
		LoadBackground();
		mode = M_NONE;
		break;

	case Key::P:
		mode = M_PATH;
		break;

	case Key::E:
		mode = M_SPAWN;
		break;

	case Key::T:
		mode = M_TOWER;
		break;

	case Key::G:
		mode = M_TARGET;
		break;

	case Key::H:
		if (mode == M_TOWER) {
			if (placeSnapped) {
				highRangePlaces.push_back(snappedPlace.GetPosition());
				UpdateMarker(M_TOWER);
			}
		}
	}
}

void MapEditor::HandleMouse(Event& event)
{
	if (mode == M_PATH) {
		if (event.Type == Event::MouseButtonPressed) {
			if (event.MouseButton.Button == Mouse::Left)
				leftDown = true;
			else if (event.MouseButton.Button == Mouse::Right)
				rightDown = true;
		}
		else if (event.Type == Event::MouseButtonReleased) {
			if (event.MouseButton.Button == Mouse::Left)
				leftDown = false;
			else if (event.MouseButton.Button == Mouse::Right)
				rightDown = false;
		}

		size_t x, y;
		if (event.Type == Event::MouseMoved) {
			x = event.MouseMove.X / gridSize;
			y = event.MouseMove.Y / gridSize;
		}
		else {
			x = event.MouseButton.X / gridSize;
			y = event.MouseButton.Y / gridSize;
		}

		bool update = false;
		if (leftDown) {
			if (!pathGrid[x + y * gridWidth]) {
				pathGrid[x + y * gridWidth] = true;
				update = true;
			}
		}
		else if (rightDown) {
			if (pathGrid[x + y * gridWidth]) {
				pathGrid[x + y * gridWidth] = false;
				update = true;
			}
		}

		if (update) {
			Color& clr = pathGrid[x + y * gridWidth] ? ColorPath : ColorTransparent;

			// update all pixels but the 1 px grid
			for (size_t col = x * gridSize + 1; col < (x+1)*gridSize - 1; ++col) {
				for (size_t row = y * gridSize + 1; row < (y+1)*gridSize - 1; ++row) {
					gridOverlayImg.SetPixel(col, row, clr);
				}
			}
		}
	}
	else if (mode == M_TOWER || mode == M_SPAWN) {
		auto& places = mode == M_TOWER ? towerPlaces : spawnPlaces;
		auto& placeMarker = mode == M_TOWER ? towerPlaceMarker : spawnPlaceMarker;
		auto& markerColor = mode == M_TOWER ? ColorPlace : ColorSpawn;

		if (event.Type == Event::MouseMoved) {
			Vector2f pos(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));

			isGoodPlace = true;
			placeSnapped = false;
			for (auto it = places.begin(); it != places.end(); ++it) {
				if (dist(pos, *it) < MIN_PLACE_DIST) {
					isGoodPlace = false;

					if (dist(pos, *it) < PLACE_RADIUS) {
						placeSnapped = true;
						pos = *it;
					}

					break;
				}
			}
			goodPlace.SetPosition(pos);
			badPlace.SetPosition(pos);
			snappedPlace.SetPosition(pos);
		}
		else if (event.Type == Event::MouseButtonReleased) {
			Vector2f pos(static_cast<float>(event.MouseButton.X), static_cast<float>(event.MouseButton.Y));
			if (event.MouseButton.Button == Mouse::Left && isGoodPlace) {
				places.push_back(pos);
				placeMarker.push_back(Shape::Circle(pos, PLACE_RADIUS, markerColor));
			}
			else if (event.MouseButton.Button == Mouse::Right && placeSnapped) {
				places.erase(boost::remove(places, snappedPlace.GetPosition()), places.end());
				if (mode == M_TOWER) {
					highRangePlaces.erase(boost::remove(highRangePlaces, snappedPlace.GetPosition()), highRangePlaces.end());
				}
				UpdateMarker(mode);
			}

		}
	}
	else if (mode == M_TARGET) {
		if (event.Type == Event::MouseButtonPressed && event.MouseButton.Button == Mouse::Left) {
			targetFrom = Vector2f(static_cast<float>(event.MouseButton.X), static_cast<float>(event.MouseButton.Y));
			mouseDown = true;
			showTarget = false;
		}
		else if (event.Type == Event::MouseButtonReleased && event.MouseButton.Button == Mouse::Left) {
			targetTo = Vector2f(static_cast<float>(event.MouseButton.X), static_cast<float>(event.MouseButton.Y));
			mouseDown = false;
			showTarget = true;
		}
		else if (event.Type == Event::MouseMoved && mouseDown) {
			targetTo = Vector2f(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));
			showTarget = true;
		}

		if (showTarget) {
			targetAreaMarker = Shape::Rectangle(targetFrom, targetTo, ColorTarget);
		}
	}
}

void MapEditor::Draw()
{
	window.Draw(background);

	if (mode == M_PATH) {
		window.Draw(gridOverlay);
		window.Draw(gridInfo);
	}
	else if (mode == M_TOWER) {
		boost::for_each(towerPlaceMarker, [&](Shape& s) {
			window.Draw(s);
		});

		if (isGoodPlace)
			window.Draw(goodPlace);
		else if (placeSnapped)
			window.Draw(snappedPlace);
		else
			window.Draw(badPlace);
	}
	else if (mode == M_SPAWN) {
		boost::for_each(spawnPlaceMarker, [&](Shape& s) {
			window.Draw(s);
		});

		if (isGoodPlace)
			window.Draw(goodPlace);
		else if (placeSnapped)
			window.Draw(snappedPlace);
		else
			window.Draw(badPlace);
	}
	else if (mode == M_TARGET) {
		if (showTarget)
			window.Draw(targetAreaMarker);
	}

	window.Display();
}

void MapEditor::LoadBackground()
{
	auto path = FileOpenDlg();
	if (path) {
		if (path.get().filename() == "map.js") {
			LoadMap(path.get());
		}
		else {
			background.SetImage(gImageManager.getResource(path.get().string()));
		}
	}
}

void MapEditor::UpdateGrid()
{
	gridOverlayImg.Create(window.GetWidth(), window.GetHeight(), ColorTransparent);

	for (size_t col = gridSize; col < gridOverlayImg.GetWidth(); col += gridSize) {
		for (size_t row = 0; row < gridOverlayImg.GetHeight(); ++row) {
			gridOverlayImg.SetPixel(col, row, Color::Black);
		}
	}

	for (size_t row = gridSize; row < gridOverlayImg.GetHeight(); row += gridSize) {
		for (size_t col = 0; col < gridOverlayImg.GetWidth(); ++col) {
			gridOverlayImg.SetPixel(col, row, Color::Black);
		}
	}

	gridWidth = gridOverlayImg.GetWidth() / gridSize;
	gridHeight = gridOverlayImg.GetHeight() / gridSize;

	gridInfo.SetText("Grid size: " + boost::lexical_cast<std::string>(gridSize));

	pathGrid.clear();
	pathGrid.resize(gridWidth * gridHeight, false);
}

void MapEditor::UpdatePath()
{
	for (size_t x=0; x < gridWidth; ++x) {
		for (size_t y = 0; y < gridHeight; ++y) {
			Color& clr = pathGrid.at(x + y * gridWidth) ? ColorPath : ColorTransparent;

			// update all pixels but the 1 px grid
			for (size_t col = x * gridSize + 1; col < (x+1)*gridSize - 1; ++col) {
				for (size_t row = y * gridSize + 1; row < (y+1)*gridSize - 1; ++row) {
					gridOverlayImg.SetPixel(col, row, clr);
				}
			}
		}
	}
}

void MapEditor::UpdateMarker(Mode md)
{
	assert(md == M_TOWER || md == M_SPAWN);

	auto& places = md == M_TOWER ? towerPlaces : spawnPlaces;
	auto& placeMarker = md == M_TOWER ? towerPlaceMarker : spawnPlaceMarker;
	auto& markerColor = md == M_TOWER ? ColorPlace : ColorSpawn;

	placeMarker.clear();
	boost::for_each(places, [&](const Vector2f& pos) {
		Color clr = markerColor;

		if (mode == M_TOWER && boost::range::find(highRangePlaces, pos) != highRangePlaces.end()) {
			clr = ColorHighRange;
		}

		placeMarker.push_back(Shape::Circle(pos, PLACE_RADIUS, clr));
	});
}

js::mArray PosToJsArray(Vector2f pos)
{
	js::mArray arr;
	arr.push_back(pos.x);
	arr.push_back(pos.y);
	return arr;
}

Vector2f JsArrayToPos(const js::mArray& arr)
{
	return Vector2f(static_cast<float>(arr[0].get_real()), static_cast<float>(arr[1].get_real()));
}

js::mArray PosVectorToJsArray(const std::vector<Vector2f>& positions)
{
	js::mArray arr;
	arr.resize(positions.size());
	for (size_t i=0; i < positions.size(); ++i) {
		js::mArray pos;
		pos.push_back(positions[i].x);
		pos.push_back(positions[i].y);

		arr[i] = pos;
	}
	return arr;
}

void JsArrayToPosVector(const js::mArray& arr, std::vector<Vector2f>& positions)
{
	positions.clear();
	positions.resize(arr.size());
	for (size_t i=0; i < arr.size(); ++i) {
		const js::mArray& parr = arr[i].get_array();

		Vector2f pos(static_cast<float>(parr[0].get_real()), static_cast<float>(parr[1].get_real()));
		positions[i] = pos;
	}
}

void MapEditor::SaveMap()
{
	auto outPathOpt = FileSaveDlg();
	if (!outPathOpt)
		return;
	auto outPath = outPathOpt.get();


	js::mObject rootObj;

	rootObj["name"] = js::mValue("Editor Map");
	rootObj["theme"] = js::mValue("default");

	js::mObject path;
	path["block-size"] = js::mValue(static_cast<int>(gridSize));
	
	js::mArray grid;
	grid.resize(pathGrid.size());
	for (size_t i=0; i < pathGrid.size(); ++i) {
		grid[i] = js::mValue(pathGrid[i]);
	}
	path["grid"] = grid;

	rootObj["path"] = path;

	rootObj["tower-places"] = PosVectorToJsArray(towerPlaces);
	rootObj["high-range"]   = PosVectorToJsArray(highRangePlaces);
	rootObj["spawn-places"] = PosVectorToJsArray(spawnPlaces);

	FloatRect targetArea;
	targetArea.Left = std::min(targetFrom.x, targetTo.x);
	targetArea.Right = std::max(targetFrom.x, targetTo.x);
	targetArea.Top = std::min(targetFrom.y, targetTo.y);
	targetArea.Bottom = std::max(targetFrom.y, targetTo.y);

	js::mObject target;
	js::mArray targetTopLeft;
	target["width"] = targetArea.GetWidth();
	target["height"] = targetArea.GetHeight();
	target["top-left"] = PosToJsArray(Vector2f(targetArea.Left, targetArea.Top));
	rootObj["target-area"] = target;
	rootObj["default-target"] = PosToJsArray(Vector2f(targetArea.Left + targetArea.GetWidth() / 2.f, targetArea.Top + targetArea.GetHeight() / 2.f));

	std::ofstream out(outPath.string());
	js::write_formatted(rootObj, out);

	auto basePath = outPath.parent_path();
	background.GetImage()->SaveToFile((basePath / "background.png").string());
}

void MapEditor::LoadMap(fs::path inPath)
{
	std::ifstream in(inPath.string());
	js::mValue rootValue;
	js::read(in, rootValue);

	js::mObject& rootObj = rootValue.get_obj();

	js::mArray& towers = rootObj["tower-places"].get_array();

	js::mArray& spawns = rootObj["spawn-places"].get_array();
	JsArrayToPosVector(towers, towerPlaces);
	JsArrayToPosVector(spawns, spawnPlaces);
	if (rootObj.count("high-range")) {
		js::mArray& highRange = rootObj["high-range"].get_array();
		JsArrayToPosVector(highRange, highRangePlaces);
	}
	else {
		highRangePlaces.clear();
	}
	UpdateMarker(M_TOWER);
	UpdateMarker(M_SPAWN);

	js::mObject& target = rootObj["target-area"].get_obj();
	js::mArray& targetTopLeft = target["top-left"].get_array();
	targetFrom = JsArrayToPos(targetTopLeft);
	targetTo = Vector2f(targetTopLeft[0].get_real() + target["width"].get_real(), targetTopLeft[1].get_real() + target["height"].get_real());

	js::mObject& path = rootObj["path"].get_obj();
	gridSize = path["block-size"].get_int();
	gridWidth = window.GetWidth() / gridSize;
	gridHeight = window.GetHeight() / gridSize;
	UpdateGrid();

	pathGrid.clear();
	js::mArray& grid = path["grid"].get_array();
	assert(grid.size() == gridWidth * gridHeight);
	pathGrid.resize(grid.size());
	for (size_t i=0; i < grid.size(); ++i) {
		pathGrid[i] = grid[i].get_bool();
	}
	UpdatePath();

	auto imgPath = inPath.parent_path() / "background.png";
	background.SetImage(gImageManager.getResource(imgPath.string()));
}
