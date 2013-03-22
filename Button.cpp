#include "pch.h"
#include "Button.h"
#include "Utility.h"

const Vector2f Button::ImageArea(-1, -1);

bool Button::HandleEvent(Event& event)
{
	switch (event.Type) {
	case Event::MouseMoved:
		{
			Vector2f mousePos(static_cast<float>(event.MouseMove.X), static_cast<float>(event.MouseMove.Y));

			if (activeSize == ImageArea)
				mouseOver = PointInRect(mousePos, GetPosition(), 
					static_cast<float>(GetImage()->GetWidth()), static_cast<float>(GetImage()->GetHeight()));
			else
				mouseOver = PointInRect(mousePos, GetPosition(), activeSize.x, activeSize.y);
		}
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
