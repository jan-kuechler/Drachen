#include "pch.h"
#include "Level.h"
#include "Error.h"

#include "json_spirit/json_spirit.h"
#include "jsex.h"

namespace js = json_spirit;
namespace fs = boost::filesystem;

void Level::LoadFromFile(const fs::path& path)
{
/* level.js layout
{
	"name": <string>,                 // name of the level
	"map": <string>,                  // map to use
	"theme": <string>,                // theme to use
	"night-time": [bool=false],       // does the level play at night?
								      
	"waves": [                        // list of waves
		{						      
			"countdown": <int>        // seconds before the wave starts
			"max-time": [int=0]       // maximal seconds before the next wave, 0=infinity
			"enemies": [              // list of enemies per spawn point
				[ // for the first spawn point
					[
						<int> (enemy type), 
						<int> (number of enemies to spawn)
					],
					...
				],
				[ // for the second spawn point
					[<int>, <int>],
					...
				], 
				...
			],
		},
		...
	],
}	

*/
	using std::string;

	auto rootObj = jsex::load_root_from_file(path);

	try {
		name  = jsex::get<string>(rootObj["name"]);
		map   = jsex::get<string>(rootObj["map"]);
		theme = jsex::get<string>(rootObj["theme"]);
		nightMode = jsex::get_opt<bool>(rootObj, "night-mode", false);


		waves.clear();
		auto& wavesArr = rootObj["waves"].get_array();
		for (auto& val: wavesArr) {
			auto& waveObj = val.get_obj();
			Wave wave;

			wave.countdown = jsex::get<int>(waveObj["countdown"]);
			wave.maxTime   = jsex::get_opt<int>(waveObj, "max-time", 0);

			js::mArray& enemyArr = waveObj["enemies"].get_array();
			for (auto& spawnVal: enemyArr) {
				std::vector<size_t> enemiesForSpawn;

				auto& spawnArr = spawnVal.get_array();
				for (auto& enemyVal: spawnArr) {
					auto& enemies = enemyVal.get_array();

					int type = enemies[0].get_int();
					int num  = enemies[1].get_int();

					while (num-- > 0)
						enemiesForSpawn.push_back(type);
				}

				wave.enemies.push_back(enemiesForSpawn);
			}

			waves.emplace_back(wave);
		}
	}
	catch (std::runtime_error err) {
		throw GameError() << ErrorInfo::Desc("Json error") << ErrorInfo::Note(err.what()) << boost::errinfo_file_name(path.string());
	}
}
