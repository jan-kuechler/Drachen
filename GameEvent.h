#ifndef GAME_EVENT_H
#define GAME_EVENT_H

#include "Error.h"

enum GameEvent
{
	GE_LEVEL_START,
	GE_LEVEL_END,

	GE_TEST_EVENT,

	GE_UNDEFINED,
};

static inline std::ostream& operator<<(std::ostream& o, GameEvent ev)
{
	switch (ev) {
	case GE_LEVEL_START:
		return o << "LevelStart";
	case GE_LEVEL_END:
		return o << "LevelEnd";
	case GE_TEST_EVENT:
		return o << "TestEvent";
	case GE_UNDEFINED:
	default:
		return o << "UNDEFINED";
	}
}

#endif //GAME_EVENT_H