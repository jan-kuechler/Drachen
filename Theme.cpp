#include "pch.h"
#include "Theme.h"
#include "Utility.h"
#include "DataPaths.h"

namespace fs = boost::filesystem;
namespace js = json_spirit;

void Theme::LoadTheme(const std::string& name)
{
	if (currentTheme == name)
		return;

	fs::path themePath = GetThemePath(name);
	fs::path themeDef = themePath / "theme.js";

	std::ifstream in(themeDef.string());
	js::mValue rootValue;
	if (!js::read(in, rootValue)) 
		throw GameError() << ErrorInfo::Desc("Root value not found") << ErrorInfo::Note("The json file may be invalid");
	if (rootValue.type() != js::obj_type)
		throw GameError() << ErrorInfo::Desc("Root value is not an object");

	rootObj = rootValue.get_obj();

	LoadFromFile(mainFont, (themePath / rootObj["main-font"].get_str()).string());
	LoadFromFile(topPanel, (themePath / rootObj["top-panel"].get_str()).string());

	currentTheme = name;
}
