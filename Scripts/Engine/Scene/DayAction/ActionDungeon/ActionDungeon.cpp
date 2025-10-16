/*
 *	@file	ActionDungeon.cpp
 *	@author	Seki
 */

#include "ActionDungeon.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/Dungeon/DungeonDataLoader.h"
#include "../../../Manager/StageManager.h"
#include <iostream>

/*
 *	初期化処理
 */
void ActionDungeon::Initialize() {
    // DungeonData CSV をロード登録
    dungeonDataLoader = std::make_shared<DungeonDataLoader>("Data/Dungeon/DungeonList.csv");
    LoadManager::GetInstance().AddLoader(dungeonDataLoader);
}
/*
 *  ロード済みのデータをセット(コールバック)
 */
void ActionDungeon::Setup() {
    if (!dungeonDataLoader || !dungeonDataLoader->IsLoaded()) {
        std::cerr << "[ActionDungeon] DungeonData がロードされていません" << std::endl;
        return;
    }

    // CSV データを内部リストにコピー
    dungeonList = dungeonDataLoader->dungeonList;

    if (!dungeonList.empty()) {
        currentDungeon = dungeonList[0];
        std::cout << "[ActionDungeon] CurrentDungeon: " << currentDungeon.name
            << " Path: " << currentDungeon.dungeonPath << std::endl;

        // メインスレッド逐次ロード: モデル
        LoadManager::GetInstance().StartTask([this]() {
            auto model = LoadManager::GetInstance().LoadResource<LoadModel>(currentDungeon.dungeonPath);
            if (model && model->IsLoaded()) {
                // StageManager に渡してステージ準備
                //StageManager::GetInstance().SetStageModel(model->GetHandle());
                std::cout << "[ActionDungeon] Model loaded and set to StageManager" << std::endl;
            }
        });
    }
}
/*
 *	更新処理
 */
void ActionDungeon::Update(float deltaTime) {
}
/*
 *	描画処理
 */
void ActionDungeon::Render() {
}
