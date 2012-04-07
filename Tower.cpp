#include "pch.h"
#include "Tower.h"

Tower::Tower(const Map* map)
: map(map), placed(false)
{ 
	SetColor(Color(255, 255, 255, 128));
}

bool Tower::HandleEvent(Event& event)
{
	if (placed)
		return false;

	if (event.Type == Event::MouseMoved) {
		SetPosition(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));
	}
	else if (event.Type == Event::MouseButtonPressed && event.MouseButton.Button == Mouse::Left) {
		placed = true;
		SetColor(Color::White);
	}
	else {
		return false;
	}
	return true;
}
