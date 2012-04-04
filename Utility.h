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

#endif //DEFAULT_EVENT_HANDLER_H
