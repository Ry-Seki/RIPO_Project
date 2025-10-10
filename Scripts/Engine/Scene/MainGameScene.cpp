#include "MainGameScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"

void MainGameScene::Initialize(Engine& engine) {
    // カレンダーシステム初期化（1ヶ月、5週、6日）
    calendarSystem = std::make_shared<CalendarSystem>();
    calendarSystem->Initialize(1, 5, 6);

    // カレンダーマネージャ初期化（入力＆描画担当）
    calendarManager = std::make_shared<CalendarManager>(calendarSystem);
}

void MainGameScene::Update(Engine& engine, float deltaTime) {
    if (CheckHitKey(KEY_INPUT_SPACE)) {
        calendarSystem->AdvanceDayWithFade(); // フェード付き日送り
    }
    // カレンダー更新
    if (calendarManager) calendarManager->Update();
}

void MainGameScene::Render() {
    // カレンダー描画
    if (calendarManager) calendarManager->Render();
}