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

static const float MIN_PLACE_DIST = 30;
static const float PLACE_RADIUS = 12.5;

MapEditor::MapEditor(RenderWindow& win)
: window(win), mode(M_NONE), gridSize(25), leftDown(false), rightDown(false)
{ 
	defBgImg.Create(win.GetWidth(), win.GetHeight(), Color::White);
	background.SetImage(defBgImg);

	gridOverlayImg.Create(win.GetWidth(), win.GetHeight(), ColorTransparent);
	gridOverlay.SetImage(gridOverlayImg);

	goodPlace = Shape::Circle(Vector2f(0, 0), PLACE_RADIUS, ColorPlaceGood);
	badPlace = Shape::Circle(Vector2f(0, 0), PLACE_RADIUS, ColorPlaceBad);
	snappedPlace = Shape::Circle(Vector2f(0, 0), PLACE_RADIUS, ColorPlaceSnapped);

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

	case Key::T:
		mode = M_TOWER;
		break;
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
	else if (mode == M_TOWER) {
		if (event.Type == Event::MouseMoved) {
			Vector2f pos(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));

			isGoodPlace = true;
			placeSnapped = false;
			for (auto it = towerPlaces.begin(); it != towerPlaces.end(); ++it) {
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
				towerPlaces.push_back(pos);
				towerPlaceMarker.push_back(Shape::Circle(pos, PLACE_RADIUS, ColorPlace));
			}
			else if (event.MouseButton.Button == Mouse::Right && placeSnapped) {
				towerPlaces.erase(boost::remove(towerPlaces, snappedPlace.GetPosition()), towerPlaces.end());
				towerPlaceMarker.clear();
				boost::for_each(towerPlaces, [&](const Vector2f& pos) {
					towerPlaceMarker.push_back(Shape::Circle(pos, PLACE_RADIUS, ColorPlace));
				});
			}

		}
	}
}

void MapEditor::Draw()
{
	window.Draw(background);

	if (mode == M_PATH) {
		window.Draw(gridOverlay);
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

	js::mArray towers;
	towers.resize(towerPlaces.size());
	for (size_t i=0; i < towerPlaces.size(); ++i) {
		js::mArray pos;
		pos.push_back(towerPlaces[i].x);
		pos.push_back(towerPlaces[i].y);

		towers[i] = pos;
	}
	rootObj["tower-places"] = towers;

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

	towerPlaces.clear();
	towerPlaceMarker.clear();
	js::mArray& towers = rootObj["tower-places"].get_array();
	for (size_t i=0; i < towers.size(); ++i) {
		js::mArray& arr = towers[i].get_array();

		Vector2f pos(static_cast<float>(arr[0].get_real()), static_cast<float>(arr[1].get_real()));
		towerPlaces.push_back(pos);
		towerPlaceMarker.push_back(Shape::Circle(pos, PLACE_RADIUS, ColorPlace));
	}

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
