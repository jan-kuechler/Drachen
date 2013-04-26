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

void ScriptingInterface::Update(float elapsed)
{
	for (auto& func: updateHandlers) {
		try {
			luabind::call_function<void>(func, elapsed);
		}
		catch (luabind::error err) {
			LOG(Error, "Error executing update handler: " << lua_tostring(L, -1));
		}
	}
}

void ScriptingInterface::ExecuteFile(const fs::path& file)
{
	int res = luaL_dofile(L, file.string().c_str());
	if (res != 0)
		BOOST_THROW_EXCEPTION(ScriptError() << ErrorInfo::Desc("Error executing Lua file") << ErrorInfo::Note(lua_tostring(L, -1)) << boost::errinfo_file_name(file.string()));
}

void ScriptingInterface::ExecuteString(const std::string& str)
{
	int res = luaL_dostring(L, str.c_str());
	if (res != 0)
		BOOST_THROW_EXCEPTION(ScriptError() << ErrorInfo::Desc("Error executing Lua code") << ErrorInfo::Note(lua_tostring(L, -1)));
}

void ScriptingInterface::CallEventHandlers(GameEvent event)
{
	if (eventHandlers.count(event)) {
		auto& handlers = eventHandlers[event];

		for (auto& func: handlers) {
			try {
				call_function<void>(func, event);
			}
			catch (luabind::error err) {
				LOG(Error, "Error executing Lua event handler: " << lua_tostring(L, -1));
			}
		}
	}
}

// exported functions

namespace Exp {
	void Log(Log::Logger::LogLevel lvl, std::string str)
	{
		Log::Logger& logger = Log::Logger::Instance();
		if (lvl <= logger.GetLogLevel()) {
			logger.GetStream(lvl) << "Lua: " << str << std::endl;
		}
	}

	void RegisterForEvent(GameEvent event, object func)
	{
		gInterface->exp_RegisterForEvent(event, func);
	}

	void RegisterForUpdate(luabind::object func)
	{
		gInterface->exp_RegisterForUpdate(func);
	}

	namespace DummyClass {
		struct LogLevel {};
	}
}

void ScriptingInterface::exp_RegisterForEvent(GameEvent event, object func)
{
	eventHandlers[event].push_back(func);
}

void ScriptingInterface::exp_RegisterForUpdate(luabind::object func)
{
	updateHandlers.push_back(func);
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

	for (auto& name: REMOVED_GLOBALS) {
		globals(L)[name] =  nil;
	}

	module(L) [
		def("Log", Exp::Log),
		def("RegisterForEvent", Exp::RegisterForEvent),
		def("RegisterForUpdate", Exp::RegisterForUpdate),

		class_<Exp::DummyClass::LogLevel>("LogLevel")
		.enum_("loglevels")[
			value("Crit", Log::Logger::Crit),
			value("Error", Log::Logger::Error),
			value("Warning", Log::Logger::Warning),
			value("Msg", Log::Logger::Msg),
			value("Debug", Log::Logger::Debug),
			value("Trace", Log::Logger::Trace)
		]
	];
}
