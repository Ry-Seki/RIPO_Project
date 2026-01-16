/*
 *  @file   MainGameScene.cpp
 *  @author Seki
 */

#include "MainGameScene.h"
#include "ResultScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include "../Manager/StageManager.h"
#include "../Load/JSON/LoadJSON.h"
#include "../System/Money/MoneyManager.h"
#include "../System/Status/PlayerStatusManager.h"
#include "../Component/CapsuleCollider.h"

#include "Selection/SelectionFactory.h"
#include "DayAction/ActionFactory.h"

/*
 *  @brief  デストラクタ
 */
MainGameScene::~MainGameScene() = default;

/*
 *  初期化処理
 */
void MainGameScene::Initialize(Engine& engine) {
    // ゲームステートの初期化
    gameState = std::make_unique<GameStateMachine>();
    gameState->Initialize(engine);
    gameState->ChageState(GameEnum::GameState::SelectAction);
    auto treasureData = LoadManager::GetInstance().LoadResource<LoadJSON>("Data/Treasure/TreasureDataList.json");
    auto itemData = LoadManager::GetInstance().LoadResource<LoadJSON>("Data/Item/ItemCatalogData.json");
    LoadManager::GetInstance().SetOnComplete([treasureData, itemData]() {
        MoneyManager::GetInstance().LoadFromJSON(treasureData->GetData(), itemData->GetData());
    });
    PlayerStatusManager::GetInstance().Initialize();
}
/*
 *  @brief  準備前処理
 */
void MainGameScene::Setup() {
}
/*
 *  更新処理
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
    gameState->Update(deltaTime);
    Scene::Update(engine, deltaTime);
}
/*
 *  描画処理
 */
void MainGameScene::Render() {
    gameState->Render();
    Scene::Render();
    DrawFormatString(50, 400, GetColor(255, 255, 255), "Money : %d", MoneyManager::GetInstance().GetCurrentMoney());

#if _DEBUG
    // 全オブジェクトのAABBCollider描画
    for (auto& obj : gameObjects) {
        auto aabb = obj->GetComponent<AABBCollider>();
        if (aabb != nullptr)
            aabb->DebugRender();
        auto capsule = obj->GetComponent<CapsuleCollider>();
        if (capsule != nullptr)
            capsule->DebugRender();
    }
#endif
}
