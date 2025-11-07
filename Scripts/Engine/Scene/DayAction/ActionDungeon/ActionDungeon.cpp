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
#include "../../../Manager/StageObjectManager.h"
#include "../../../Scene/Scene.h"
#include "../../../Component/ModelRenderer.h"
#include "../../../Component/Character/CharacterUtility.h"
#include "../../../Stage/StageObject/StageObjectUtility.h"
#include "../../../Audio/AudioUtility.h"

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
        // SEの再生
        AudioUtility::PlaySE("DebugSE");
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
    CharacterUtility::RemoveAllCharacter();
    StageManager::GetInstance().Execute();
    StageObjectUtility::RemoveAllStageObject();
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
        StageObjectManager::GetInstance().Initialize(engine);
    }
    CharacterManager::GetInstance().GeneratePlayer("player", { 0, 100, 0 }, { 0, 0, 0 }, { -50, -100, -50 }, { 50,  100,  50 });
    CameraManager::GetInstance().CreateCamera("camera", { 0, 0, 0 }, { 0, 0, 0 });
    CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
    CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
    CharacterManager::GetInstance().GenerateEnemy("enemy", { 0, 0, 0 }, { 0, 0, 0 }, { -100, 0, -100 }, { 100, 300, 100 });
    StageObjectManager::GetInstance().GenerateTreasure("treasure", { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 });
    StageObjectManager::GetInstance().GenerateTreasure("treasure", { 0,0,0 }, { 0,0,0 }, { -100,0,-100 }, { 100,300,100 });

    LoadResourcesFromStageData(engine, stageData, dungeonResource);
}

void ActionDungeon::DebugSetup(Engine& engine, const DungeonResource& setResource) {
    // ステージの設定
    // モデルハンドルの取得
    int stageHandle = setResource.stageResource[0]->GetHandle();
    // モデルの設定
    StageManager::GetInstance().LoadStage(stageHandle);
    // ステージボーンデータの設定
    StageManager::GetInstance().SetStageJSONData(setResource.stageBoneResource->GetData());

    // プレイヤーの設定
    // モデルの取得
    int playerHandle = setResource.playerResource->GetHandle();
    // プレイヤーオブジェクトの取得
    auto player = CharacterManager::GetInstance().GetCharacter(0)->GetOwner();
    // 位置の設定
    player->position = StageManager::GetInstance().GetStartPos();
    // モデルの設定
    CharacterManager::GetInstance().SetModelHandle(player, playerHandle);

    // 敵の設定
    // 敵の生成位置の取得
    std::vector<Vector3> enemySpawnPos = StageManager::GetInstance().GetEnemySpwanPos();
    size_t enemyCount = enemySpawnPos.size();
    // モデルハンドルの取得
    int enemyHandle = setResource.enemyResource[0]->GetHandle();
    for (int i = 0; i < enemyCount; i++) {
        // 敵の取得
        CharacterBasePtr enemyCharacter = CharacterManager::GetInstance().GetCharacter(i + 1);
        if (!enemyCharacter) continue;
        // 敵のオブジェクトの取得
        GameObject* enemy = CharacterManager::GetInstance().GetCharacterOwner(enemyCharacter);
        if (!enemy) continue;
        // 位置の設定
        enemy->position = enemySpawnPos[i];
        // モデルの設定
        CharacterManager::GetInstance().SetModelHandle(enemy, enemyHandle);
        // コンポーネントの取得
        std::shared_ptr<EnemyComponent> component = enemy->GetComponent<EnemyComponent>();
        if (!component) continue;
        // WayPointの設定
        component->SetWayPoint(enemy->position);
    }

    // 宝の設定
    // お宝の生成位置の取得
    std::vector<Vector3> treasureSpawnPos = StageManager::GetInstance().GetTreasureSpwanPos();
    // 生成位置の要素数の取得
    size_t treasureCount = treasureSpawnPos.size();
    // ハンドルの要素数の取得
    size_t treasureTypeCount = setResource.treasureResource.size();
    for (int i = 0; i < treasureTypeCount; i++) {
        // モデルハンドルの取得
        int treasureHandle = setResource.treasureResource[i]->GetHandle();
        // 宝の取得
        StageObjectBasePtr treasureObject = StageObjectManager::GetInstance().GetStageObject(i);
        if (!treasureObject) continue;
        // 宝オブジェクトの取得
        GameObject* treasure = StageObjectManager::GetInstance().GetStageObjectOwner(treasureObject);
        if (!treasure) continue;
        // 位置の設定
        treasure->position = treasureSpawnPos[i];
        // モデルの設定
        StageObjectManager::GetInstance().SetModelHandle(treasure, treasureHandle);
    }
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
    // TreasureData
    auto treasureList = stageData.GetArray("Treasure", "TreasureData");
    for (const auto& treasurePath : treasureList) {
        if (!treasurePath.empty()) {
            dungeonResource.treasureResource.push_back(load.LoadResource<LoadModel>(treasurePath));
        }
    }
    // EventTreasureData（後ほどフラグで管理）
    std::string eventTreasurePath;
    if (stageData.TryGet("Treasure.EventTreasureData", eventTreasurePath) && !eventTreasurePath.empty()) {
        dungeonResource.eventTreasureResource = load.LoadResource<LoadModel>(eventTreasurePath);
    }    // ロード完了時のコールバック登録
    load.SetOnComplete([this, &engine, dungeonResource]() {
        DebugSetup(engine, dungeonResource);
    });
}
