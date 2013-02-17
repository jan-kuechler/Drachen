#include "pch.h"
#include "Button.h"
#include "Utility.h"

bool Button::HandleEvent(Event& event)
{
	switch (event.Type) {
	case Event::MouseMoved:
		mouseOver = PointInRect(Vector2f(event.MouseMove.X, event.MouseMove.Y), GetPosition(), GetImage()->GetWidth(), GetImage()->GetHeight());
		break;
	case Event::MouseButtonReleased:
		if (mouseOver && event.MouseButton.Button == Mouse::Left) {
			clicked = true;
			return true;
		}
		break;
	}
	return false;
}
