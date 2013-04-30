#include "pch.h"
#include "ScriptingInterface.h"
#include "Game.h"
#include "Error.h"
#include "ResourceManager.h"
#include "DataPaths.h"

#include "lualib.h" // for luaopen_*
#include "lstate.h" // for lua_State

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
	updateHandlers.clear();
	drawables.clear();

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

void ScriptingInterface::RegisterDrawable(sf::Drawable* obj)
{
	drawables.push_back(obj);
}

void ScriptingInterface::UnregisterDrawable(sf::Drawable* obj)
{
	auto iter = boost::range::find(drawables, obj);
	if (iter != drawables.end())
		drawables.erase(iter);
}

void ScriptingInterface::Draw(RenderTarget& target)
{
	for (auto& obj: drawables) {
		target.Draw(*obj);
	}
}

void ScriptingInterface::LogInfo() const
{
	LOG(Msg, "ScriptingInterface info:");
	LOG(Msg,"\tUpdate handlers: " << updateHandlers.size());
	LOG(Msg,"\tEvent handlers:");
	for (const auto& it : eventHandlers) {
		LOG(Msg, "\t\t" << it.first << ": " << it.second.size());
	}
	LOG(Msg, "\tRegistered drawables: " << drawables.size());
	LOG(Msg, "\tLua state:");
	LOG(Msg, "\t\tAllocated bytes: " << L->l_G->totalbytes);
	LOG(Msg, "\t\tUsed bytes:      " << L->l_G->estimate << " (estimated)");
	LOG(Msg, "\t\tGC dept:         " << L->l_G->gcdept);
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

#define pick(Class_, Func_, Ret_, Arg_) static_cast<Ret_ (Class_::*) Arg_>(&Class_::Func_)

template <typename T>
static void Unregister(T& self)
{
	gInterface->UnregisterDrawable(&self);
}

template <typename T>
static void GetPosition(lua_State* L, T& self)
{
	auto pos = self.GetPosition();
	lua_pushnumber(L, pos.x);
	lua_pushnumber(L, pos.y);
}

static luabind::scope DeclColor()
{
	return class_<sf::Color>("Color")
		.def(luabind::constructor<Uint8, Uint8, Uint8>())
		.def(luabind::constructor<Uint8, Uint8, Uint8, Uint8>())
	;
}

static sf::Image& GetImage(const char *path)
{
	return gImageManager.getResource(GetLevelPackFile(path).string());
}

static luabind::scope DeclImage()
{
	return class_<sf::Image>("Image")
	;
}

static sfex::String* CreateString()
{
	auto str = new sfex::String;
	gInterface->RegisterDrawable(str);
	str->SetFont(gTheme.GetMainFont());
	return str;
}

static luabind::scope DeclString()
{
	return class_<sfex::String>("String")
		.def("SetText", &sfex::String::SetText)
		.def("GetText", &sfex::String::GetText)
		.def("SetSize", &sfex::String::SetSize)
		.def("GetSize", &sfex::String::GetSize)
		.def("SetColor", &sfex::String::SetColor)
		.def("GetColor", &sfex::String::GetColor)
		.def("SetPosition", pick(sfex::String, SetPosition, void, (float, float)))
		.def("GetPosition", &GetPosition<sfex::String>)
		.def("Show", &sfex::String::Show)
		.def("Hide", &sfex::String::Hide)
		.def("SetVisible", &sfex::String::SetVisible)
		.def("GetVisible", &sfex::String::GetVisible)
		.def("Unregister", &Unregister<sfex::String>)
	;
}

static sfex::Sprite* CreateSprite()
{
	auto sprite = new sfex::Sprite;
	gInterface->RegisterDrawable(sprite);
	return sprite;
}

static luabind::scope DeclSprite()
{
	return class_<sfex::Sprite>("Sprite")
		.def("SetImage", &sfex::Sprite::SetImage)
		.def("SetPosition", pick(sfex::Sprite, SetPosition, void, (float, float)))
		.def("GetPosition", &GetPosition<sfex::Sprite>)
		.def("Show", &sfex::Sprite::Show)
		.def("Hide", &sfex::Sprite::Hide)
		.def("SetVisible", &sfex::Sprite::SetVisible)
		.def("GetVisible", &sfex::Sprite::GetVisible)
		.def("Unregister", &Unregister<sfex::Sprite>)
	;
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
		def("Log", &Exp::Log),
		def("RegisterForEvent", &Exp::RegisterForEvent),
		def("RegisterForUpdate", &Exp::RegisterForUpdate),

		class_<Exp::DummyClass::LogLevel>("LogLevel")
		.enum_("loglevels")[
			value("Crit", Log::Logger::Crit),
			value("Error", Log::Logger::Error),
			value("Warning", Log::Logger::Warning),
			value("Msg", Log::Logger::Msg),
			value("Debug", Log::Logger::Debug),
			value("Trace", Log::Logger::Trace)
		],

		DeclColor(),

		def("GetImage", &GetImage),
		DeclImage(),

		def("CreateString", &CreateString),
		DeclString(),

		def("CreateSprite", &CreateSprite),
		DeclSprite()
	];

	globals(L)["Color"]["Black"] = &Color::Black;
	globals(L)["Color"]["White"] = &Color::White;
	globals(L)["Color"]["Red"] = &Color::Red;
	globals(L)["Color"]["Green"] = &Color::Green;
	globals(L)["Color"]["Blue"] = &Color::Blue;
	globals(L)["Color"]["Yellow"] = &Color::Yellow;
	globals(L)["Color"]["Magenta"] = &Color::Magenta;
	globals(L)["Color"]["Cyan"] = &Color::Cyan;
}
