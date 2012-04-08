#ifndef DEFAULT_EVENT_HANDLER_H
#define DEFAULT_EVENT_HANDLER_H

bool DefaultHandleEvent(RenderWindow& win, Event& event);

template <typename Res>
void LoadFromFile(Res& res, const std::string& fileName)
{
	if (!res.LoadFromFile(fileName)) {
		throw std::runtime_error("Failed to open '" + fileName + "'");
	}
}
#ifdef __APPLE__
#include "Tower.h"
inline bool sorttower(const Tower& a, const Tower& b){
    return a.GetPosition().y <b.GetPosition().y;
}
#endif

#endif //DEFAULT_EVENT_HANDLER_H
