/*
 *	@file	DungeonResourceData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_RESOURCE_DATA_H_
#define _DUNGEON_RESOURCE_DATA_H_

#include "DungeonStageData.h"
#include "../../Engine/GameConst.h"
#include "../../Engine/StringUtility.h"
#include "../../Engine/Load/LoadManager.h"
#include "../../Engine/Load/JSON/LoadJSON.h"
#include "../../Engine/Load/Model/LoadModel.h"

#include <vector>
#include <unordered_map>
#include <memory>

 // 前方宣言
class Engine;

/*
 * @brief	ダンジョンに必要なリソースデータ
 */
class DungeonResourceData {
public:
	// Stage
	std::vector<std::shared_ptr<LoadModel>> stageResource;
	std::shared_ptr<LoadJSON> stageBoneResource;
	std::shared_ptr<LoadJSON> stageFloorResource;

	// Character
	std::shared_ptr<LoadModel> playerResource;
	// TODO:Enemyは後ほど変更を加えるようにする
	std::vector<std::shared_ptr<LoadModel>> enemyResource;
	std::shared_ptr<LoadModel> bossResource;

	// Treasure
	std::unordered_map<int, std::unordered_map<int, std::shared_ptr<LoadModel>>> treasureResource;

public:
	/*
	 *	@brief		リソースデータのリセット
	 */
	void ClearResourceData() {
		stageResource.clear();
		stageBoneResource = nullptr;
		stageFloorResource = nullptr;
		playerResource = nullptr;
		enemyResource.clear();
		bossResource = nullptr;
		treasureResource.clear();
	}
	/*
	 *  @brief      ステージデータからからリソースデータを読み込む
	 *  @param[in]  DungeonStageData& stageData
	 *  @param[in]  bool isBossDefeated
	 *  @param[in]	bool isEventDay
	 *  @param[in]  std::vector<std::vector<int>> spawnTreasureIDList
	 */
	void LoadResourcesFromStageData(DungeonStageData& stageData,
									bool isBossDefeated,
									bool isEventDay,
									std::vector<std::vector<int>> spawnTreasureIDList) {

		LoadManager& load = LoadManager::GetInstance();

		// ステージ
		auto stageMap = stageData.GetCategory("Stage");
		for (const auto& [key, path] : stageMap) {
			if (key.find("StageData") != std::string::npos) {
				stageResource.emplace_back(load.LoadResource<LoadModel>(path));
			} else if (key == "StageBoneData") {
				stageBoneResource = load.LoadResource<LoadJSON>(path);
			} else if (key == "StageFloorData") {
				stageFloorResource = load.LoadResource<LoadJSON>(path);
			}
		}
		// キャラクター
		auto charMap = stageData.GetCategory("Character");
		for (const auto& [key, path] : charMap) {
			if (key == "PlayerData") {
				playerResource = load.LoadResource<LoadModel>(path);
			} else if (key.find("EnemyData.") == 0) {
				enemyResource.emplace_back(load.LoadResource<LoadModel>(path));
			} else if (!isBossDefeated && key == "BossData") {
				bossResource = load.LoadResource<LoadModel>(path);
			}
		}
		// お宝
		auto treasureMap = stageData.GetCategory("Treasure");
		int normalIndex = GameConst::NORMAL_TREASURE_INDEX;
		for (const auto& [key, path] : treasureMap) {
			// .で文字列を切り分ける
			auto parts = StringUtility::Split(key, '.');
			if (parts.size() < 2) continue;
			// 分離した文字列をそれぞれ整数に変換
			int floorID = std::stoi(parts[0]);
			int treasureID = std::stoi(parts[1]);
			// 生成対象でなければスキップ
			if (std::find(spawnTreasureIDList[normalIndex].begin(),
				spawnTreasureIDList[normalIndex].end(),
				treasureID) == spawnTreasureIDList[normalIndex].end()) continue;

			treasureResource[floorID][treasureID] = load.LoadResource<LoadModel>(path);
		}
		// イベントお宝
		if (isEventDay) {
			auto eventTreasureMap = stageData.GetCategory("EventTreasure");
			int eventIndex = GameConst::EVENT_TREASURE_INDEX;
			for (const auto& [key, path] : eventTreasureMap) {
				// .で文字列を切り分ける
				auto parts = StringUtility::Split(key, '.');
				if (parts.size() < 2) continue;
				// 分離した文字列をそれぞれ整数に変換
				int floorID = std::stoi(parts[0]);
				int treasureID = std::stoi(parts[1]);
				// 生成対象でなければスキップ
				if (std::find(spawnTreasureIDList[eventIndex].begin(),
							  spawnTreasureIDList[eventIndex].end(),
							  treasureID) == spawnTreasureIDList[eventIndex].end()) continue;

				treasureResource[floorID][treasureID] = load.LoadResource<LoadModel>(path);
			}
		}
	}
};

#endif // !_DUNGEON_RESOURCE_DATA_H_