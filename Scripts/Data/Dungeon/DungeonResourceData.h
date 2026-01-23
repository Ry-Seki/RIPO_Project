/*
 *	@file	DungeonResourceData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_RESOURCE_DATA_H_
#define _DUNGEON_RESOURCE_DATA_H_

#include "DungeonStageData.h"
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
     *  @brief      ステージデータからからリソースデータを読み込む
     *  @param[in]  DungeonStageData& stageData
     *  @param[in]  bool isBossDefeated
     *  @param[in]  std::vector<int> spawnTreasureIDList
     */
    void LoadResourcesFromStageData(DungeonStageData& stageData,
                                    bool isBossDefeated,
                                    std::vector<int> spawnTreasureIDList) {

        LoadManager& load = LoadManager::GetInstance();

        // ステージ
        auto stageMap = stageData.GetCategory("Stage");
        for (const auto& [key, path] : stageMap) {

            if (key.find("StageData") != std::string::npos) {
                stageResource.emplace_back(
                    load.LoadResource<LoadModel>(path)
                );
            }
            else if (key == "StageBoneData") {
                stageBoneResource =
                    load.LoadResource<LoadJSON>(path);
            }
            else if (key == "StageFloorData") {
                stageFloorResource =
                    load.LoadResource<LoadJSON>(path);
            }
        }
        // キャラクター
        auto charMap = stageData.GetCategory("Character");
        for (const auto& [key, path] : charMap) {
            if (key == "PlayerData") {
                playerResource =
                    load.LoadResource<LoadModel>(path);
            }
            else if (key.find("EnemyData.") == 0) {
                // EnemyData.0.0 など
                enemyResource.emplace_back(
                    load.LoadResource<LoadModel>(path)
                );
            }
            else if (!isBossDefeated && key == "BossData") {
                bossResource =
                    load.LoadResource<LoadModel>(path);
            }
        }
        // お宝
        auto treasureMap = stageData.GetCategory("Treasure");
        for (const auto& [key, path] : treasureMap) {
            // .で文字列を切り分ける
            auto parts = StringUtility::Split(key, '.');
            if (parts.size() < 2) continue;
            // 分離した文字列をそれぞれ、整数に変換
            int floor = std::stoi(parts[0]);
            int treasureID = std::stoi(parts[1]);

            // 生成対象でなければスキップ
            if (std::find(spawnTreasureIDList.begin(),
                spawnTreasureIDList.end(),
                treasureID) == spawnTreasureIDList.end()) continue;
            treasureResource[floor][treasureID] = load.LoadResource<LoadModel>(path);
        }
    }
};

#endif // !_DUNGEON_RESOURCE_DATA_H_