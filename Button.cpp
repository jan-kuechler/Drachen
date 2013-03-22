#include "pch.h"
#include "Button.h"
#include "Utility.h"

const Vector2f Button::ImageArea(-1, -1);

bool Button::HandleEvent(Event& event)
{
	bool handled = false;
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
		UpdateImage();
		break;
	case Event::MouseButtonPressed:
		if (mouseOver && event.MouseButton.Button == Mouse::Left) {
			mouseDown = true;
			handled = true;
		}
		UpdateImage();
		break;
	case Event::MouseButtonReleased:
		mouseDown = false;

		if (mouseOver && event.MouseButton.Button == Mouse::Left) {
			clicked = true;
			handled = true;
		}
		UpdateImage();
		break;
	}

	return handled;
}

void Button::UpdateImage()
{
	if (imgDown && mouseDown && mouseOver)
		Sprite::SetImage(*imgDown);
	else if (imgHighlight && mouseOver)
		Sprite::SetImage(*imgHighlight);
	else
		Sprite::SetImage(*imgNormal);
}
