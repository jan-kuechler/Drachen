#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include "Game.h"
#include "GameEvent.h"
#include "Error.h"

struct ScriptError : public GameError
{ };


class ScriptingInterface
{
	Game& game;

	std::map<GameEvent, std::vector<luabind::object>> eventHandlers;
	std::vector<luabind::object> updateHandlers;

	lua_State* L;
public:
	ScriptingInterface(Game& game);

	void Reset();
	void Update(float elapsed);

	void ExecuteFile(const boost::filesystem::path& file);
	void ExecuteString(const std::string& str);

	void CallEventHandlers(GameEvent event);

	// exported functions, do not call from C++

	void exp_RegisterForEvent(GameEvent event, luabind::object func);
	void exp_RegisterForUpdate(luabind::object func);

private:
	void InitialiseLua();
};

#endif //SCRIPTING_INTERFACE_H
