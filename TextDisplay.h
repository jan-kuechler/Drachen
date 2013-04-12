#ifndef TEXT_DISPLAY_H
#define TEXT_DISPLAY_H

#include "Button.h"
#include "State.h"

class TextDisplay
{
	RenderWindow& window;

	Sprite background;
	String text;

	bool running;

public:
	TextDisplay(RenderWindow& win);

	void Reset();
	void Run();

	bool IsRunning() const
	{
		return running;
	}

	State GetNextState() const;

private:
	void LoadText(const std::string& fn);
};

#endif //TEXT_DISPLAY_H
