#include "MainGameScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"

void MainGameScene::Initialize(Engine& engine) {
	auto fadeIn = CreateFade(FadeType::Black, 1.0f, FadeDirection::In, FadeMode::Stop);
	engine.StartSceneFade(fadeIn);
}

void MainGameScene::Update(Engine& engine, float deltaTime) {
}

void MainGameScene::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "[MainScene] •`‰æ’†...");

}
