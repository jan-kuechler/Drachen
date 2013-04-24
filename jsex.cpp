#include "pch.h"
#include "Error.h"

#include "json_spirit/json_spirit.h"
#include "jsex.h"

namespace js = json_spirit;
namespace fs = boost::filesystem;

js::mObject jsex::load_root_from_file(const fs::path& path)
{
	std::ifstream in(path.string());
	js::mValue rootValue;
	try {
		js::read_or_throw(in, rootValue);
	}
	catch (js::Error_position err) {
		throw GameError() << ErrorInfo::Desc("Invalid json file") << ErrorInfo::Note(err.reason_) << boost::errinfo_at_line(err.line_) << boost::errinfo_file_name(path.string());
	}

	if (rootValue.type() != js::obj_type)
		throw GameError() << ErrorInfo::Desc("Root value is not an object") << boost::errinfo_file_name(path.string());

	return rootValue.get_obj();
}
