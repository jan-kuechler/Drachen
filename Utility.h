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

inline float dist(const Vector2f& a, const Vector2f& b)
{
	return norm(a - b);
}

inline float dist(const Drawable& a, const Drawable& b)
{
	return dist(a.GetPosition(), b.GetPosition());
}

inline float dot(const Vector2f& a, const Vector2f& b)
{
	return a.x * b.x + a.y * b.y;
}

inline bool PointInRect(sf::Vector2f pt, sf::Vector2f topLeft, float width, float height)
{
	return pt.x >= topLeft.x && pt.x <= topLeft.x + width && pt.y > topLeft.y && pt.y <= topLeft.y + height;
}

inline Vector2f GetCenter(const FloatRect& r)
{
	return Vector2f(r.Left + r.GetWidth() / 2.f, r.Top + r.GetHeight() / 2.f);
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
