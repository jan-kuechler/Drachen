#include "pch.h"
#include "Loose.h"
#include "Theme.h"
#include "GlobalStatus.h"
#include "ResourceManager.h"
#include "Utility.h"
#include "Button.h"

Loose::Loose(RenderWindow& Loose)
: window(Loose), running(true)
{ }

void Loose::Reset()
{
	running = true;

	background.SetImage(gImageManager.getResource(gTheme.GetFileName("loose/background")));
	background.SetPosition(0, 0);
}

void Loose::Run()
{
	// Handle all SFML events
	Event event;
	while (window.GetEvent(event)) {
		// Handle default stuff like Loosedow closed etc.
		if (DefaultHandleEvent(window, event))
			continue;

		if (event.Type == Event::MouseButtonReleased) {
			running = false;
		}
	}

	window.Draw(background);
	window.Display();
}
