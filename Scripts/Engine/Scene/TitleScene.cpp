/*
 *  @file   TitleScene.cpp
 *  @author Seki
 */

#include "TitleScene.h"
#include "MainGameScene.h"
#include "../Engine.h"
#include "../Fade/FadeFactory.h"
#include "../Fade/FadeManager.h"
#include <DxLib.h>
#include <memory>
#include "../Manager/StageManager.h"

 /*
  *  ‰Šú‰»ˆ—
  */
void TitleScene::Initialize(Engine& engine) {
	StageManager::GetInstance().LoadStage("Res/Model/Stage/StageModel_1.mv1");
	engine.StartFadeOutIn(0.5f, 0.5f, [&engine]() {
		engine.SetNextScene(std::make_shared<MainGameScene>());
		});
}
/*
 *  XVˆ—
 */
void TitleScene::Update(Engine& engine, float deltaTime) {

}
/*
 *  •`‰æˆ—
 */
void TitleScene::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] •`‰æ’†...");
}