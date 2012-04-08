#ifndef UTILITY_H
#define UTILITY_H

bool DefaultHandleEvent(RenderWindow& win, Event& event);

inline float norm(const Vector2f& v)
{
	return sqrt(v.x*v.x + v.y*v.y);
}

template <typename Res>
void LoadFromFile(Res& res, const std::string& fileName)
{
	if (!res.LoadFromFile(fileName)) {
		throw std::runtime_error("Failed to open '" + fileName + "'");
	}
}

#endif //UTILITY_H
