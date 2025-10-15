/*
 *  @file   MainGameScene.cpp
 *  @author Seki
 */

#include "MainGameScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include "../Manager/StageManager.h"

/*
 *  初期化処理
 */
void MainGameScene::Initialize(Engine& engine) {
    // カレンダーシステム初期化（1ヶ月、5週、6日）
    calendarSystem = std::make_shared<CalendarSystem>();
    calendarSystem->Initialize(1, 5, 6);

    // カレンダーマネージャ初期化（入力＆描画担当）
    calendarManager = std::make_shared<CalendarManager>(calendarSystem);
}
/*
 *  更新処理
 */
void MainGameScene::Update(Engine& engine, float deltaTime) {
    if (FadeManager::GetInstance().IsFading()) return;

    calendarManager->Update();

    // 日が終わったら Engine 側フェード
    if (calendarManager->IsDayComplete()) {
        engine.StartFadeOutIn(0.5f, 0.5f, [this]() {
            calendarManager->NextDay(); // 日進行
        });
    }
}
/*
 *  描画処理
 */
void MainGameScene::Render() {
    calendarManager->Render();
    StageManager::GetInstance().Render();
}