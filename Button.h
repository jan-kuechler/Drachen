#ifndef BUTTON_H
#define BUTTON_H

class Button : public sf::Sprite
{
	bool mouseOver;

	bool clicked;

	Vector2f activeSize;
public:
	static const Vector2f ImageArea;

	Button()
	: sf::Sprite(), clicked(false), mouseOver(false), activeSize(ImageArea)
	{ }

	bool HandleEvent(sf::Event& event);

	void SetActiveSize(const Vector2f& size)
	{
		activeSize = size;
	}

	bool WasClicked()
	{
		bool wasClicked = clicked;
		clicked = false;
		return wasClicked;
	}

	bool MouseOver()
	{
		return mouseOver;
	}
};

#endif //BUTTON_H
