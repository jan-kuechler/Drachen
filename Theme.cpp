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
	try {
		js::read_or_throw(in, rootValue);
	}
	catch (js::Error_position err) {
		throw GameError() << ErrorInfo::Desc("Invalid json file") << ErrorInfo::Note(err.reason_) << boost::errinfo_at_line(err.line_) << boost::errinfo_file_name(themeDef.string());
	}
	if (rootValue.type() != js::obj_type)
		throw GameError() << ErrorInfo::Desc("Root value is not an object");

	rootObj = rootValue.get_obj();
	currentTheme = name;
	try {
		LoadFromFile(mainFont, (themePath / rootObj["main-font"].get_str()).string());
		LoadFromFile(topPanel, (themePath / rootObj["top-panel"].get_str()).string());
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(themeDef.string());
	}
}
