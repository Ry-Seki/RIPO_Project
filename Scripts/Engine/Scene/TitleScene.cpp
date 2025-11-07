/*
 *  @file   TitleScene.cpp
 *  @author Seki
 */

#include "TitleScene.h"
#include "../Engine.h"
#include "MainGameScene.h"
#include "../Load/Audio/LoadAudio.h"
#include "../Load/LoadManager.h"
#include "../Audio/AudioUtility.h"

#include <DxLib.h>
#include <memory>

 /*
  *  初期化処理
  */
void TitleScene::Initialize(Engine& engine) {
	AudioUtility::SetBGMVolume(100);
	AudioUtility::SetSEVolume(100);
	auto debugSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/ButtonPush_Debug.mp3");
	LoadManager::GetInstance().SetOnComplete([&engine, this, debugSE]() {
		AudioUtility::RegisterSEHandle("DebugSE", debugSE->GetHandle());
		Setup(engine);
	});
}
/*
 *  ロード済みデータの設定 (コールバック)
 */
void TitleScene::Setup(Engine& engine) {
	engine.StartFadeOutIn(0.5f, 0.5f, [&engine]() {
		engine.SetNextScene(std::make_shared<MainGameScene>());
	});
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