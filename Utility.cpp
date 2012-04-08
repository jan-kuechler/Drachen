#include "pch.h"
#include "Utility.h"

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

	default:
		return false;
	}
}

