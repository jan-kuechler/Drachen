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
			lua_pushstring(L, boost::lexical_cast<std::string>(event).c_str());
		}
	};

	template <>
	struct default_converter<const GameEvent&> : default_converter<GameEvent>
	{};

// sf::Unicode
	template <>
	struct default_converter<sf::Unicode::Text> : native_converter_base<sf::Unicode::Text>
	{
		static int compute_score(lua_State* L, int index)
		{
			return lua_type(L, index) == LUA_TSTRING ? 0 : -1;
		}

		sf::Unicode::Text from(lua_State* L, int index)
		{
			return sf::Unicode::Text(lua_tostring(L, index));
		}

		void to(lua_State* L, const sf::Unicode::Text& str)
		{
			lua_pushstring(L, static_cast<std::string>(str).c_str());
		}
	};

	template <>
	struct default_converter<const sf::Unicode::Text&> : default_converter<sf::Unicode::Text>
	{};
}

#endif //LUA_CONVERTER_H
