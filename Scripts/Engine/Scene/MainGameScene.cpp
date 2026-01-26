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
#include "../Save/SaveDataManager.h"

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
    auto treasureData = LoadManager::GetInstance().LoadResource<LoadJSON>(_TREASURE_DATA_PATH);
    auto itemData = LoadManager::GetInstance().LoadResource<LoadJSON>(_ITEM_DATA_PATH);
    LoadManager::GetInstance().SetOnComplete([this, &engine, treasureData, itemData]() {
        MoneyManager::GetInstance().LoadFromJSON(treasureData->GetData(), itemData->GetData());
        Setup(engine);
    });
    PlayerStatusManager::GetInstance().Initialize();
}
/*
 *  @brief  準備前処理
 */
void MainGameScene::Setup(Engine& engine) {
    auto& save = SaveDataManager::GetInstance();
    save.AutoSaveLoad();

    auto& context = gameState->GetActionContext();
    save.ApplyLoadData(context);
}
/*
 *  更新処理
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
    gameState->Update(deltaTime);
    Scene::Update(engine, deltaTime);
    // 遷移条件
    if (gameState->IsActionEnd()) {
        EndMainGameScene(engine);
    }
}
/*
 *  描画処理
 */
void MainGameScene::Render() {
    gameState->Render();
    Scene::Render();
    DrawFormatString(50, 400, GetColor(255, 255, 255), "Money : %d", MoneyManager::GetInstance().GetCurrentMoney());

#if _DEBUG
    // 全オブジェクトのCollider描画
    for (auto& obj : gameObjects) {
        auto aabb = obj->GetComponent<AABBCollider>();
        if (aabb != nullptr)
            aabb->DebugRender();
        auto capsule = obj->GetComponent<CapsuleCollider>();
        if (capsule != nullptr)
            capsule->DebugRender();
    }
#endif
    // 敵の攻撃仮描画
    for (auto& obj : gameObjects) {
        if (obj->name != GameConst::_CREATE_POSNAME_ENEMY)
            continue;
        auto aabb = obj->GetComponent<AABBCollider>();
        if (aabb != nullptr)
            aabb->DebugRender();
    }
}
/*
 *  @brief  メインシーン終了処理
 */
void MainGameScene::EndMainGameScene(Engine& engine) {
    auto& context = gameState->GetActionContext();
    auto& save = SaveDataManager::GetInstance();
    // アクション終了フラグの変更
    gameState->SetIsActionEnd(false);
    // オートセーブにセーブ
    save.CollectSaveData(context);
    save.AutoSave();
    // シーン遷移条件
    if (context.elapsedDay > _END_DAY) {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop);
        FadeManager::GetInstance().StartFade(fadeOut, [&engine, this]() {
            engine.SetNextScene(std::make_shared<ResultScene>());
        });
    } else {
        FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::NonStop);
        FadeManager::GetInstance().StartFade(fadeOut, [&engine, this]() {
            gameState->ChageState(GameEnum::GameState::SelectAction);
        });
    }
}
