#include "pch.h"
#include "MapEditor.h"
#include "../ResourceManager.h"

static Color ColorPath(255, 255, 0, 64);
static Color ColorTransparent(0, 0, 0, 0);

MapEditor::MapEditor(RenderWindow& win)
: window(win), mode(M_NONE), gridSize(25), leftDown(false), rightDown(false)
{ 
	gridOverlayImg.Create(win.GetWidth(), win.GetHeight(), ColorTransparent);
	gridOverlay.SetImage(gridOverlayImg);

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
	case Key::L:
		LoadBackground();
		mode = M_NONE;
		break;

	case Key::P:
		mode = M_PATH;
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
}

void MapEditor::Draw()
{
	window.Draw(background);

	if (mode == M_PATH)
		window.Draw(gridOverlay);

	window.Display();
}

void MapEditor::LoadBackground()
{
	bgImage = "background.png"; // FIXME: proper load file dialog
	background.SetImage(gImageManager.getResource(bgImage));
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
