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

private:
	std::string currentTheme;

	sf::Font mainFont;
};

#endif //THEME_H
