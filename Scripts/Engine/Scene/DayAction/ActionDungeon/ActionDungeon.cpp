/*
 *	@file	ActionDungeon.cpp
 *	@author	Seki
 */

#include "ActionDungeon.h"
#include "../../../Engine.h"
#include "../../../Load/LoadManager.h"
#include "../../../Load/Dungeon/DungeonDataLoader.h"
#include "../../../Load/JSON/LoadJSON.h"
#include "../../../Manager/StageManager.h"
#include "../../../Manager/CameraManager.h"
#include "../../../Manager/GameObjectManager.h"
#include "../../../Manager/CharacterManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Component/ModelRenderer.h"
#include <iostream>

// 静的メンバ変数の初期化
bool ActionDungeon::isFirst = true;

/*
 *	初期化処理
 */
void ActionDungeon::Initialize(Engine& engine) {
    
}
/*
 *  ロード済みのデータをセット(コールバック)
 */
void ActionDungeon::Setup(Engine& engine) {

}
/*
 *	更新処理
 */
void ActionDungeon::Update(Engine& engine, float deltaTime) {
    if (!isComplete && !inputHandle && CheckHitKey(KEY_INPUT_2)) {
        inputHandle = true;
        isComplete = true;
        Teardown();
    }

    if (CheckHitKey(KEY_INPUT_0) == 0) inputHandle = false;
}
/*
 *	描画処理
 */
void ActionDungeon::Render() {
    StageManager::GetInstance().Render();
#if _DEBUG
    DrawFormatString(50, 50, GetColor(0, 0, 0), "2 : AdvanveDay");
    {
        VECTOR pos1, pos2;

        //  XZ平面 100.0f毎に1本ライン引き
        {
            pos1 = VGet(-5000.0f, 0.0f, -5000.0f);
            pos2 = VGet(-5000.0f, 0.0f, 5000.0f);
            for (int i = 0; i <= 100; i++) {
                DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
                pos1.x += 100.0f;
                pos2.x += 100.0f;
            }

            pos1 = VGet(-5000.0f, 0.0f, -5000.0f);
            pos2 = VGet(5000.0f, 0.0f, -5000.0f);
            for (int i = 0; i <= 100; i++) {
                DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
                pos1.z += 100.0f;
                pos2.z += 100.0f;
            }
        }

        //  X軸
        {
            pos1 = VGet(0, 0, 0);
            pos2 = VScale(VGet(1, 0, 0), 5000.0f);		//  VGet(5000.0f, 0.0f, 0.0f)
            DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
        }
        //  Y軸
        {
            pos1 = VGet(0, 0, 0);
            pos2 = VScale(VGet(0, 1, 0), 5000.0f);		//  VGet(0.0f, 5000.0f, 0.0f)
            DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
        }
        //  Z軸
        {
            pos1 = VGet(0, 0, 0);
            pos2 = VScale(VGet(0, 0, 1), 5000.0f);	//  VGet(0.0f, 0.0f, 5000.0f)
            DrawLine3D(pos1, pos2, GetColor(255, 255, 255));
        }

    }
#endif
}
/*
 *  破棄処理
 */
void ActionDungeon::Teardown() {
    CharacterManager::GetInstance().RemoveCharacter(0);
    StageManager::GetInstance().Execute();
    CameraManager::GetInstance().ResetCamera();
}

void ActionDungeon::DebugInitialize(Engine& engine, DungeonStageData& setStageData) {
    isComplete = false;
    stageData = setStageData;
    LoadManager& load = LoadManager::GetInstance();
    DungeonResource dungeonResource;

    if (isFirst) {
        isFirst = false;
        GameObjectManager::GetInstance().Initialize(engine);
        CameraManager::GetInstance().Initialize(engine);
        CharacterManager::GetInstance().Initialize(engine);
        StageManager::GetInstance().Initialize(engine);
    }
    CharacterManager::GetInstance().GeneratePlayer("player", { 0, 100, 0 }, { 0, 0, 0 }, { -50, -100, -50 }, { 50,  100,  50 });
    CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });

    std::string dungeonPath;
    if(!stageData.TryGetByLeafName("StageData1", dungeonPath)) return;
    std::string dungeonBonePath;
    if(!stageData.TryGetByLeafName("StageBoneData", dungeonBonePath)) return;
    std::string playerPath;
    if(!stageData.TryGetByLeafName("PlayerData", playerPath)) return;

    dungeonResource.stageResource.push_back(load.LoadResource<LoadModel>(dungeonPath));
    dungeonResource.stageBoneResource = load.LoadResource<LoadJSON>(dungeonBonePath);
    dungeonResource.playerResource = load.LoadResource<LoadModel>(playerPath);

    load.SetOnComplete([this, &engine, dungeonResource]() {
        DebugSetup(engine, dungeonResource); 
    });
}

void ActionDungeon::DebugSetup(Engine& engine, const DungeonResource& setResource) {
    std::shared_ptr<LoadModel> stageModel = setResource.stageResource[0];
    int stageHandle = stageModel->GetHandle();
    int playerHandle = setResource.playerResource->GetHandle();
    StageManager::GetInstance().LoadStage(stageHandle);
    StageManager::GetInstance().SetStageJSONData(setResource.stageBoneResource->GetData());
    auto player = CharacterManager::GetInstance().GetCharacter(0)->GetOwner();
    CharacterManager::GetInstance().SetModelHandle(player, playerHandle);
    player->position = StageManager::GetInstance().GetStartPos();
}
/*
 *	ステージデータからロードリストに追加
 *	@param[in]	DungeonStageData& stageData			ステージデータ
 *	@param[in]	DungeonResource&  dungeonResource	セットするリソース
 */
void ActionDungeon::LoadResourcesFromStageData(Engine& engine, DungeonStageData& stageData, DungeonResource& dungeonResource) {
    LoadManager& load = LoadManager::GetInstance();
    // Stageカテゴリ
    auto stageMap = stageData.GetCategory("Stage");
    for (const auto& [key, path] : stageMap) {
        if (key.rfind("StageData", 0) == 0) {
            dungeonResource.stageResource.push_back(load.LoadResource<LoadModel>(path));
        } else if (key == "StageBoneData") {
            dungeonResource.stageBoneResource = load.LoadResource<LoadJSON>(path);
        }
    }
    // Characterカテゴリ
    auto characterMap = stageData.GetCategory("Character");
    for (const auto& [key, path] : characterMap) {
        if (key.find("Player") != std::string::npos) {
            dungeonResource.playerResource = load.LoadResource<LoadModel>(path);
        } else if (key.rfind("EnemyData") != std::string::npos) {
            dungeonResource.enemyResource.push_back(load.LoadResource<LoadModel>(path));
        }
    }
    // Treasureカテゴリ
    auto treasureMap = stageData.GetCategory("Treasure");
    for (const auto& [key, value] : treasureMap) {
        // TreasureDataが配列だった場合
        if (key == "TreasureData") {
            auto treasureList = stageData.GetArray("Treasure", "TreasureData");
            for (const auto& treasurePath : treasureList) {
                if (!treasurePath.empty()) {
                    dungeonResource.treasureResource.push_back(load.LoadResource<LoadModel>(treasurePath));
                }
            }
        }
        // 単体データ(EventTreasureData)
        else if (key == "EventTreasureData" && !value.empty()) {
            dungeonResource.eventTreasureResource = load.LoadResource<LoadModel>(value);
        }
    }    // ロード完了時のコールバック登録
    load.SetOnComplete([this, &engine, dungeonResource]() {
        DebugSetup(engine, dungeonResource);
    });
}
