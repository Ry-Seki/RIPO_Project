/*
 *  @file   TitleScene.cpp
 */

#include "TitleScene.h"
#include "../DelayFrameTask.h"
#include "../Delay.h"
#include "../Scheduler.h"
#include "../Time.h"
#include <DxLib.h>
#include <memory>
#include "MainGameScene.h"

bool TitleScene::Initialize(Engine& engine) {
    isTaskRun = true;
    // タスク生成 → Scheduler に登録
    m_debugTask = std::make_shared<Task<>>(DebugTask(engine));
    return true;
}

void TitleScene::Update(Engine& engine, float deltaTime) {
    if (isTaskRun) return;

    // ←ここに通常の更新処理（入力など）
    DrawFormatString(50, 150, GetColor(255, 255, 0), "通常Update中...");
}

Task<> TitleScene::DebugTask(Engine& engine) {
    DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] タスク開始");

    // 3フレーム待機
    co_await DelayFrameTask(3);
    DrawFormatString(50, 70, GetColor(255, 255, 255), "[TitleScene] 3フレーム待機完了");

    // 1秒待機
    co_await Delay(1000);
    DrawFormatString(50, 90, GetColor(255, 255, 255), "[TitleScene] 1秒待機完了");

    DrawFormatString(50, 110, GetColor(255, 255, 255), "[TitleScene] タスク完了");

    isTaskRun = false;
    // 次の Scene に遷移
    engine.SetNextScene(std::make_shared<MainGameScene>());
}

void TitleScene::Render() {
    DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] 描画中...");
    if (isTaskRun) {
        DrawFormatString(200, 300, GetColor(255, 200, 200), "Now Loading...");
    }
}