/*
 *	@file	DungeonResourceData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_RESOURCE_DATA_H_
#define _DUNGEON_RESOURCE_DATA_H_

#include "DungeonStageData.h"
#include "../../Engine/Load/LoadManager.h"
#include "../../Engine/Load/JSON/LoadJSON.h"
#include "../../Engine/Load/Model/LoadModel.h"

#include <memory>

// 前方宣言
class Engine;

/*
 *	@breif	ダンジョンに必要なリソースデータ
 */
class DungeonResourceData {
public:
	std::vector<std::shared_ptr<LoadModel>> stageResource;
	std::vector<std::shared_ptr<LoadJSON>> stageBoneResource;
	std::shared_ptr<LoadJSON> stageFloorResource;
	std::shared_ptr<LoadModel> playerResource;
	std::vector<std::shared_ptr<LoadModel>> enemyResource;
	std::shared_ptr<LoadModel> bossResource;
	std::vector<std::shared_ptr<LoadModel>> treasureResource;
	std::shared_ptr<LoadModel> eventTreasureResource;

public:
	/*
	 *	ステージデータからロードリストに追加
	 *	@param[in]	DungeonStageData& stageData			ステージデータ
	 */
	void LoadResourcesFromStageData(DungeonStageData& stageData) {
		LoadManager& load = LoadManager::GetInstance();
		// Stageカテゴリ
		auto stageMap = stageData.GetCategory("Stage");
		for (const auto& [key, path] : stageMap) {
			if (key.rfind("StageData", 0) == 0) {
				stageResource.push_back(load.LoadResource<LoadModel>(path));
			}
			else if (key.rfind("StageBoneData", 0) == 0) {
				stageBoneResource.push_back(load.LoadResource<LoadJSON>(path));
			}
			else if (key.rfind("StageFloorData", 0) == 0) {
				stageFloorResource = load.LoadResource<LoadJSON>(path);
			}
		}
		// Characterカテゴリ
		auto characterMap = stageData.GetCategory("Character");
		for (const auto& [key, path] : characterMap) {
			if (key.find("Player") != std::string::npos) {
				playerResource = load.LoadResource<LoadModel>(path);
			}
			else if (key.rfind("EnemyData") != std::string::npos) {
				enemyResource.push_back(load.LoadResource<LoadModel>(path));
			}
		}
		// TreasureData
		auto treasureMap = stageData.GetCategory("Treasure");
		for (const auto& [key, path] : treasureMap) {
			// 数字キーだけを対象にする
			if (!path.empty() && std::all_of(key.begin(), key.end(), ::isdigit)) {
				treasureResource.push_back(load.LoadResource<LoadModel>(path));
			}
		}
		// EventTreasureData（後ほどフラグで管理）
		std::string eventTreasurePath;
		if (stageData.TryGet("Treasure.EventTreasureData", eventTreasurePath) && !eventTreasurePath.empty()) {
			eventTreasureResource = load.LoadResource<LoadModel>(eventTreasurePath);
		}
	}
};

#endif // !_DUNGEON_RESOURCE_DATA_H_