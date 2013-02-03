#ifndef THEME_H
#define THEME_H

class Theme
{
public:
	void LoadTheme(const std::string& name);

	const sf::Font& GetMainFont() const
	{
		return mainFont;
	}

	const sf::Image& GetTopPanelImage() const
	{
		return topPanel;
	}

private:
	std::string currentTheme;

	sf::Font mainFont;
	sf::Image topPanel;
};

#endif //THEME_H
