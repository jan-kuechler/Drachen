#include "pch.h"
#include "ScriptingInterface.h"
#include "Error.h"

#include "lualib.h" // for luaopen_*

#include "LuaConverter.h"

namespace fs = boost::filesystem;
using namespace luabind;

static ScriptingInterface* gInterface = nullptr;

ScriptingInterface::ScriptingInterface(Game& game)
: game(game), L(nullptr)
{
	if (gInterface) {
		throw GameError();
	}
	gInterface = this;
}

void ScriptingInterface::Reset()
{
	eventHandlers.clear();

	if (L) {
		lua_close(L);
		L = nullptr;
	}
	InitialiseLua();
}

void ScriptingInterface::ExecuteFile(const fs::path& file)
{
	int res = luaL_dofile(L, file.string().c_str());
	if (res != 0)
		throw GameError();
}

void ScriptingInterface::ExecuteString(const std::string& str)
{
	int res = luaL_dostring(L, str.c_str());
	if (res != 0)
		throw GameError();
}

void ScriptingInterface::CallEventHandlers(GameEvent event)
{
	if (eventHandlers.count(event)) {
		auto& handlers = eventHandlers[event];

		for (auto& func: handlers) {
			call_function<void>(func, event);
		}
	}
}

// exported functions

namespace Exp {
	void Log(Log::Logger::LogLevel lvl, std::string str)
	{
		Log::Logger& logger = Log::Logger::Instance();
		if (lvl <= logger.GetLogLevel()) {
			logger.GetStream(lvl) << str << std::endl;
		}
	}

	void RegisterForEvent(GameEvent event, object func)
	{
		gInterface->exp_RegisterForEvent(event, func);
	}
}

void ScriptingInterface::exp_RegisterForEvent(GameEvent event, object func)
{
	eventHandlers[event].push_back(func);
}

static const char* REMOVED_GLOBALS[] = {
	"print", "load", "loadfile", "dofile", LUA_COLIBNAME,
};

void ScriptingInterface::InitialiseLua()
{
	assert(!L);
	L = luaL_newstate();

	luaopen_base(L);
	luaopen_table(L);
	luaopen_string(L);
	luaopen_math(L);

	open(L);

	// FIXME: commented out, leads to a runtime_error("unregistered class")
	//for (auto& name: REMOVED_GLOBALS) {
	//	settable(globals(L), name, nil);
	//}

	module(L) [
		def("Log", Exp::Log),
		def("RegisterForEvent", Exp::RegisterForEvent)
	];
}
