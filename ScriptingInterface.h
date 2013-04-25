#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include "Game.h"
#include "GameEvent.h"

class ScriptingInterface
{
	Game& game;

	std::map<GameEvent, std::vector<luabind::object>> eventHandlers;

	lua_State* L;
public:
	ScriptingInterface(Game& game);

	void Reset();

	void ExecuteFile(const boost::filesystem::path& file);
	void ExecuteString(const std::string& str);

	void CallEventHandlers(GameEvent event);

	// exported functions, do not call from C++

	void exp_RegisterForEvent(GameEvent event, luabind::object func);

private:
	void InitialiseLua();
};

#endif //SCRIPTING_INTERFACE_H
