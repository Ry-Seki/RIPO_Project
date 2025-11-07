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
  *  初期化処理
  */
void TitleScene::Initialize(Engine& engine) {
	engine.StartFadeOutIn(0.5f, 0.5f, [&engine]() {
		engine.SetNextScene(std::make_shared<MainGameScene>());
		});
}
/*
 *  ロード済みデータの設定 (コールバック)
 */
void TitleScene::Setup() {

}
/*
 *  更新処理
 */
void TitleScene::Update(Engine& engine, float deltaTime) {

}
/*
 *  描画処理
 */
void TitleScene::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] 描画中...");
}