#ifndef BUTTON_H
#define BUTTON_H

#include "sfex.h"

class Button : public sfex::Sprite
{
	const Image *imgNormal, *imgHighlight, *imgDown;
	bool mouseOver, mouseDown;

	bool clicked;

	Vector2f activeSize;
public:
	static const Vector2f ImageArea;

	Button()
	: sfex::Sprite(), clicked(false), mouseOver(false), activeSize(ImageArea), mouseDown(false), 
	  imgNormal(nullptr), imgHighlight(nullptr), imgDown(nullptr)
	{ }

	bool HandleEvent(sf::Event& event);

	void SetImage(const sf::Image& img) /* override */
	{
		imgNormal = &img;
		Sprite::SetImage(img);
	}

	void SetHighlightImage(const sf::Image& img)
	{
		imgHighlight = &img;
	}

	void SetDownImage(const sf::Image& img)
	{
		imgDown = &img;
	}

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
		return visible && mouseOver;
	}

private:
	void UpdateImage();
};

#endif //BUTTON_H
