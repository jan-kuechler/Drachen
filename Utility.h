#ifndef UTILITY_H
#define UTILITY_H

#include "Error.h"

bool DefaultHandleEvent(RenderWindow& win, Event& event);

static const float PI = 3.14159265f;

inline float abs(const Vector2f& v)
{
	return v.x*v.x + v.y*v.y;
}

inline float norm(const Vector2f& v)
{
	return sqrt(abs(v));
}

inline float dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

template <typename Res>
void LoadFromFile(Res& res, const std::string& fileName)
{
	try {
		if (!res.LoadFromFile(fileName)) {
			throw GameError() << ErrorInfo::Desc("Unknown error") << ErrorInfo::Loading(true) << boost::errinfo_file_name(fileName);
		}
	}
	catch (boost::exception& ex) {
		if (!boost::get_error_info<boost::errinfo_file_name>(ex)) {
			ex << boost::errinfo_file_name(fileName);
		}

		ex << ErrorInfo::Loading(true);
		throw;
	}
}

#endif //UTILITY_H
