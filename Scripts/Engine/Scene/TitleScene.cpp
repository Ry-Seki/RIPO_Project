/*
 *  @file   TitleScene.cpp
 */

#include "TitleScene.h"
#include "../DelayFrameTask.h"
#include "../DelayTask.h"
#include "../Scheduler.h"
#include "../Time.h"
#include <DxLib.h>
#include <memory>
#include "MainGameScene.h"


bool TitleScene::Initialize(Engine& engine) {
    requestSceneChange = false;

    // coroutine Çê∂ê¨Ç∑ÇÈÇæÇØÇ≈ Scheduler Ç™ï€éù
    DebugTask(engine);

    return true;
}

TaskVoid TitleScene::DebugTask(Engine& engine) {
    co_await Delay(1000);
    co_await Delay(1000);

    requestSceneChange = true;
}

void TitleScene::Update(Engine& engine, float deltaTime) {
    if (requestSceneChange) {
        engine.SetNextScene(std::make_shared<MainGameScene>());
        requestSceneChange = false;
    }
    DrawFormatString(50, 150, GetColor(255, 255, 0), "TitleScene UpdateíÜ...");
}

void TitleScene::Render() {
    DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] ï`âÊíÜ...");
}