#ifndef THEME_H
#define THEME_H

#include "json_spirit/json_spirit.h"

// TODO ?!
// Allow for composite names to be queried. A query (GetInt, GetFloat, etc) for
// "title/font/size" would the json file entry at
//  { ...
//    "title" : {
//      "font" : {
//        "size" : 25,
//        ...
//      },
//      ...
//    },
//    ...
//  }
//


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

	const sf::Vector2f GetPosition(const std::string& name) const
	{
		const json_spirit::mArray& arr = rootObj.at(name).get_array();
		return sf::Vector2f(static_cast<float>(arr[0].get_real()), static_cast<float>(arr[1].get_real()));
	}

	int GetInt(const std::string& name) const
	{
		return rootObj.at(name).get_int();
	}

	float GetFloat(const std::string& name) const
	{
		return static_cast<float>(rootObj.at(name).get_real());
	}

	std::string GetString(const std::string& name) const
	{
		return rootObj.at(name).get_str();
	}

private:
	json_spirit::mObject rootObj;

	std::string currentTheme;

	sf::Font mainFont;
	sf::Image topPanel;
};

#endif //THEME_H
