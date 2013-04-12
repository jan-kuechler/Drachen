#include "pch.h"
#include "TextDisplay.h"
#include "Theme.h"
#include "GlobalStatus.h"
#include "ResourceManager.h"
#include "Utility.h"
#include "Button.h"
#include "UiHelper.h"
#include "DataPaths.h"

static const std::string TextPrefixFromFile = "#F:";

TextDisplay::TextDisplay(RenderWindow& win)
: window(win), running(true)
{ }

void TextDisplay::Reset()
{
	running = true;

	InitText(text, "text-display/text");

	if (boost::starts_with(gStatus.runTime.textDisplay.text, TextPrefixFromFile))
		LoadText(gStatus.runTime.textDisplay.text.substr(TextPrefixFromFile.length()));
	else
		text.SetText(gStatus.runTime.textDisplay.text);

	background.SetImage(gImageManager.getResource(gTheme.GetFileName("text-display/background")));
	background.SetPosition(0, 0);
}

void TextDisplay::Run()
{
	// Handle all SFML events
	Event event;
	while (window.GetEvent(event)) {
		// Handle default stuff like window closed etc.
		if (DefaultHandleEvent(window, event))
			continue;

		if (event.Type == Event::MouseButtonReleased) {
			running = false;
		}
	}

	window.Draw(background);
	window.Draw(text);
	window.Display();
}

State TextDisplay::GetNextState() const
{
	return gStatus.runTime.textDisplay.nextState;
}

void TextDisplay::LoadText(const std::string& fn)
{
	std::string str;
	std::ifstream ifs(GetLevelPackFile(fn).string());
	std::string line;

	while (std::getline(ifs, line))
		str += line + "\n";

	text.SetText(str);
}
