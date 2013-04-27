#ifndef SCRIPTING_INTERFACE_H
#define SCRIPTING_INTERFACE_H

#include "GameEvent.h"
#include "Error.h"

class Game;

struct ScriptError : public GameError
{ };


class ScriptingInterface
{
	Game& game;

	std::map<GameEvent, std::vector<luabind::object>> eventHandlers;
	std::vector<luabind::object> updateHandlers;

	std::vector<sf::Drawable*> drawables;

	lua_State* L;
public:
	ScriptingInterface(Game& game);

	void Reset();
	void Update(float elapsed);

	void Draw(RenderTarget& target);

	void ExecuteFile(const boost::filesystem::path& file);
	void ExecuteString(const std::string& str);

	void CallEventHandlers(GameEvent event);

	void RegisterDrawable(sf::Drawable* obj);
	void UnregisterDrawable(sf::Drawable* obj);

	// exported functions, do not call from C++

	void exp_RegisterForEvent(GameEvent event, luabind::object func);
	void exp_RegisterForUpdate(luabind::object func);

private:
	void InitialiseLua();
};

#endif //SCRIPTING_INTERFACE_H
