#ifndef BUTTON_H
#define BUTTON_H

class Button : public sf::Sprite
{
	bool mouseOver;

	bool clicked;
public:
	Button()
	: sf::Sprite(), clicked(false), mouseOver(false)
	{ }

	bool HandleEvent(sf::Event& event);

	bool WasClicked()
	{
		bool wasClicked = clicked;
		clicked = false;
		return wasClicked;
	}
};

#endif //BUTTON_H
