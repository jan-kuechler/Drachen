#include "pch.h"
#include "Utility.h"
#include "GlobalStatus.h"

namespace fs = boost::filesystem;

void MakeScreenshot(RenderWindow& win);

bool DefaultHandleEvent(RenderWindow& win, Event& event)
{
	switch (event.Type) {
	case Event::Closed:
		win.Close();
		return true;

	case Event::KeyPressed:
		if (event.Key.Code == Key::Escape) {
			win.Close();
			return true;
		}
		// enable/disable debug with F12/Shift-F12
		else if (event.Key.Code == Key::F12) {
			gStatus.debug.enabled = !event.Key.Shift;
		}
		else if (event.Key.Code == Key::R) {
			if (gStatus.debug.enabled) {
				gStatus.Reset();
			}
		}
		else if (event.Key.Code == Key::P) {
			MakeScreenshot(win);
		}



	default:
		return false;
	}
}

fs::path GetScreenshotPath()
{
	fs::path def = "screenshot.png";
	if (!fs::exists(def))
		return def;

	size_t i=0;
	for (size_t i=1; i < 10000; ++i) {
		fs::path p = "screenshot-" + boost::lexical_cast<std::string>(i) + ".png";
		if (!fs::exists(p))
			return p;
	}

	return def;
}

void MakeScreenshot(RenderWindow& win)
{
	win.Capture().SaveToFile(GetScreenshotPath().string());
}
