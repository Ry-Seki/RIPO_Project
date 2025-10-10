/*
 *  @file   TitleScene.cpp
 */

#include "TitleScene.h"
#include "MainGameScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include <DxLib.h>
#include <memory>


void TitleScene::Initialize(Engine& engine) {
    auto fadeOut = CreateFade(FadeType::Black, 1.0f, FadeDirection::Out, FadeMode::Stop);
    engine.StartSceneFade(fadeOut, [&engine]() {
        engine.SetNextScene(std::make_shared<MainGameScene>());
    });
}

void TitleScene::Update(Engine& engine, float deltaTime) {
    
}

void TitleScene::Render() {
    DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] •`‰æ’†...");
}