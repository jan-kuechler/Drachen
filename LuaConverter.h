#ifndef LUA_CONVERTER_H
#define LUA_CONVERTER_H

#include "GameEvent.h"
#include "Log.h"

namespace luabind
{
// GameEvent enum
	template <>
	struct default_converter<GameEvent> : native_converter_base<GameEvent>
	{
		static int compute_score(lua_State* L, int index)
		{
			return lua_type(L, index) == LUA_TSTRING ? 0 : -1;
		}

		GameEvent from(lua_State* L, int index)
		{
			std::string str = lua_tostring(L, index);

			if (str == "LevelStart")
				return GE_LEVEL_START;
			else if (str == "LevelEnd")
				return GE_LEVEL_END;
			else if (str == "TestEvent")
				return GE_TEST_EVENT;
			else
				return GE_UNDEFINED;
		}

		void to(lua_State* L, const GameEvent& event)
		{
			switch (event) {
			case GE_LEVEL_START:
				lua_pushstring(L, "LevelStart");
				return;
			case GE_LEVEL_END:
				lua_pushstring(L, "LevelEnd");
				return;

			case GE_TEST_EVENT:
				lua_pushstring(L, "TestEvent");
				return;

			default:
				throw GameError(); // TODO
			}
		}
	};

	template <>
	struct default_converter<const GameEvent&> : default_converter<GameEvent>
	{};
}

#endif //LUA_CONVERTER_H
