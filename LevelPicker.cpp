#include "pch.h"
#include "LevelPicker.h"
#include "ResourceManager.h"
#include "Theme.h"
#include "GlobalStatus.h"
#include "DataPaths.h"
#include "Utility.h"
#include "UiHelper.h"

#include "json_spirit/json_spirit.h"

namespace fs = boost::filesystem;
namespace js = json_spirit;

LevelPicker::LevelPicker(RenderWindow& win)
: window(win)
{
	LoadLevelPacks();
}

void LevelPicker::Reset()
{
	running = true;

	const LevelPack& pack = levelPacks[gStatus.levelPack];

	background.SetImage(gImageManager.getResource(gTheme.GetFileName("level-picker/background")));

	InitText(strName, "level-picker/name");
	InitText(strDesc, "level-picker/desc");
	strName.SetColor(gTheme.GetColor("level-picker/name/color"));
	strDesc.SetColor(gTheme.GetColor("level-picker/desc/color"));
	strName.SetText(pack.name);
	strDesc.SetText(pack.desc);
	CenterText(strName);


	auto startPos = gTheme.GetPosition("level-picker/level-buttons/start");
	auto lineOffset = gTheme.GetPosition("level-picker/level-buttons/line-offset");
	auto textOffset = gTheme.GetPosition("level-picker/level-buttons/text-offset");
	levelButtons.resize(pack.levels.size());
	levelStrings.resize(pack.levels.size());
	for (size_t i=0; i < pack.levels.size(); ++i) {
		levelButtons[i].SetImage(gImageManager.getResource(gTheme.GetFileName("level-picker/level-buttons/red")));
		levelButtons[i].SetPosition(startPos + lineOffset * static_cast<float>(i));
		levelButtons[i].SetActiveSize(Vector2f(gTheme.GetFloat("level-picker/level-buttons/line-width"), static_cast<float>(levelButtons[i].GetImage()->GetHeight())));

		levelStrings[i].SetFont(gTheme.GetMainFont());
		levelStrings[i].SetSize(gTheme.GetFloat("level-picker/level-buttons/font-size"));
		levelStrings[i].SetColor(gTheme.GetColor("level-picker/level-buttons/color"));
		levelStrings[i].SetText(std::get<0>(pack.levels[i]));
		levelStrings[i].SetPosition(startPos + textOffset + lineOffset * static_cast<float>(i));
	}

	previewImage.SetPosition(gTheme.GetPosition("level-picker/preview/position"));
	previewImage.SetImage(gImageManager.getResource(GetLevelPackFile(pack.image).string()));
}

void LevelPicker::Run()
{
	// Handle all SFML events
	Event event;
	while (window.GetEvent(event)) {
		// Handle default stuff like window closed etc.
		if (DefaultHandleEvent(window, event))
			continue;

		bool handled = false;
		boost::for_each(levelButtons, [&handled, &event](Button& btn) mutable {
				if (btn.HandleEvent(event))
					handled = true;
			});
		if (handled)
			continue;

		if (event.Type == Event::MouseButtonReleased) {
			running = false;
		}
	}

	window.Draw(background);

	window.Draw(strName);
	window.Draw(strDesc);
	window.Draw(previewImage);

	for (size_t i=0; i < levelButtons.size(); ++i) {
		window.Draw(levelButtons[i]);
		window.Draw(levelStrings[i]);
	}

	window.Display();
}

void LevelPicker::LoadLevelPacks()
{
	std::ifstream in(LevelPacksFile.string());
	js::mValue rootValue;
	try {
		js::read_or_throw(in, rootValue);
	}
	catch (js::Error_position err) {
		throw GameError() << ErrorInfo::Desc("Invalid json file") << ErrorInfo::Note(err.reason_) << boost::errinfo_at_line(err.line_) << boost::errinfo_file_name(LevelPacksFile.string());
	}

	if (rootValue.type() != js::obj_type)
		throw GameError() << ErrorInfo::Desc("Root value is not an object") << boost::errinfo_file_name(LevelPacksFile.string());

	js::mObject rootObj = rootValue.get_obj();

	try {
		js::mObject& packs = rootObj["level-packs"].get_obj();

		for (auto it = packs.begin(); it != packs.end(); ++it) {
			auto id = it->first;
			auto def = it->second.get_obj();

			LevelPack pack;

			pack.name = def["name"].get_str();
			pack.desc = def["desc"].get_str();
			pack.image = def["image"].get_str();

			js::mArray& levels = def["levels"].get_array();
			pack.levels.reserve(levels.size());
			for (size_t i=0; i < levels.size(); ++i) {
				js::mArray& arr = levels[i].get_array();

				pack.levels.push_back(std::make_tuple(arr[0].get_str(), arr[1].get_str()));
			}

			levelPacks[id] = pack;
		}
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(LevelPacksFile.string());
	}
}
