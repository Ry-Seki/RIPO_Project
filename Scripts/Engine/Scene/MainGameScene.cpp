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

/*
 *  初期化処理
 */
void MainGameScene::Initialize(Engine& engine) {
    // カレンダー管理クラスの生成
    calendarManager = std::make_unique<CalendarManager>();
    // カレンダー管理クラスの初期化
    calendarManager->Initialize();
    // 選択管理クラスの生成
    selectionManager = std::make_unique<SelectionManager>();
    // 行動管理クラスの初期化
    actionManager = std::make_unique<ActionManager>();
    calendarManager->SetSelectionManager(selectionManager.get());
    calendarManager->SetActionManager(actionManager.get());
    selectionManager->SetActionManager(actionManager.get());
    auto treasureData = LoadManager::GetInstance().LoadResource<LoadJSON>("Data/Treasure/TreasureDataList.json");
    auto itemData = LoadManager::GetInstance().LoadResource<LoadJSON>("Data/Item/ItemCatalogData.json");
    LoadManager::GetInstance().SetOnComplete([treasureData, itemData]() {
        MoneyManager::GetInstance().LoadFromJSON(treasureData->GetData(), itemData->GetData());
    });
    PlayerStatusManager::GetInstance().Initialize();
}
/*
 *  ロード済みのデータの設定(コールバック)
 */
void MainGameScene::SetupData(Engine& engine, std::shared_ptr<LoadJSON> setSelectionData, std::shared_ptr<LoadJSON> setActionData) {

}
/*
 *  更新処理
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
    if (FadeManager::GetInstance().IsFading()) return;

    calendarManager->Update(engine);

    selectionManager->Update(engine, deltaTime);

    actionManager->Update(engine, deltaTime);

    // 基底クラスの更新処理
    Scene::Update(engine, deltaTime);
    // 日が終わったら Engine 側フェード
    if (calendarManager->IsDayComplete() && !calendarManager->IsEndDayAdvance()) {
        engine.StartFadeOutIn(1.0f, 1.0f, [&engine, this]() {
            AdvanceDay(engine); // 日進行
        });
    }
}
/*
 *  描画処理
 */
void MainGameScene::Render() {
    calendarManager->Render();
    selectionManager->Render();
    actionManager->Render();
    Scene::Render();
    DrawFormatString(50, 400, GetColor(255, 255, 255), "Money : %d", MoneyManager::GetInstance().GetCurrentMoney());

#if _DEBUG
    // 全オブジェクトのAABBCollider描画
    for (auto& obj : gameObjects) {
        auto aabb = obj->GetComponent<AABBCollider>();
        if (aabb == nullptr) continue;
        aabb->DebugRender();
    }
#endif
}
/*
 *  日にち更新処理
 */
void MainGameScene::AdvanceDay(Engine& engine) {
    if (calendarManager->IsEndDayAdvance()) {
		auto fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
        FadeManager::GetInstance().StartFade(fadeOut, [&engine]() {
			engine.SetNextScene(std::make_shared<ResultScene>());
		});
    }else {
		calendarManager->NextDay();
    }
}
