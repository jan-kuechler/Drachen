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
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(themeDef.string());
	}
}

std::string Theme::GetFileName(const std::string& path, int idx) const
{
	std::string fileName = TraversePath(path, idx).get_str();
	return (GetThemePath(currentTheme) / fileName).string();
}

const js::mValue& Theme::TraversePath(const std::string& path, int idx) const
{
	std::vector<std::string> parts;
	boost::split(parts, path, boost::is_any_of("/"));
	std::string last = parts.back();
	parts.pop_back();

	const js::mObject* parent = &rootObj;
	for (auto it = parts.begin(); it != parts.end(); ++it) {
		bool arrayAccess = false;
		int arrayIndex = 0;
		std::string name = *it;

		size_t bracketPos = it->find('[');
		if (bracketPos != it->npos) {
			arrayAccess = true;
			name = it->substr(0, bracketPos);
			std::string index = it->substr(bracketPos+1);
			index.pop_back(); // remove the closing ]

			if (!index.empty()) 
				arrayIndex = boost::lexical_cast<int>(index);
			else
				arrayIndex = idx;
		}

		if (!arrayAccess) {
			parent = &parent->at(name).get_obj();
		}
		else {
			const js::mArray& arr = parent->at(name).get_array();
			parent = &arr.at(arrayIndex).get_obj();
		}
	}

	return parent->at(last);
}
