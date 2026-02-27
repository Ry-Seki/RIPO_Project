/*
 *  @file   TitleScene.cpp
 *  @author Seki
 */

#include "TitleScene.h"
#include "../Engine.h"
#include "MainGameScene.h"
#include "../Load/Audio/LoadAudio.h"
#include "../Load/Sprite/LoadSprite.h"
#include "../Load/LoadManager.h"
#include "../Load/Animation/LoadAnimation_ChangeBackground.h"
#include "../Audio/AudioUtility.h"
#include "../Fade/FadeManager.h"
#include "../Fade/FadeFactory.h"
#include "../Menu/MenuManager.h"
#include "../Menu/Title/MenuTitle.h"
#include "../Menu/Title/MenuGameModeSelect.h"
#include "../Menu/System/MenuSelectSaveSlot.h"
#include "../Menu/System/MenuConfirm.h"
#include "../Menu/Title/MenuSystem.h"
#include "../Menu/System/MenuCredit.h"

#include <DxLib.h>

 /*
  *  初期化処理
  */
void TitleScene::Initialize(Engine& engine) {
	isStart = false;
	inputHandle = false;
	MenuManager::GetInstance().GetMenu<MenuTitle>();
	MenuManager::GetInstance().GetMenu<MenuGameModeSelect>();
	MenuManager::GetInstance().GetMenu<MenuSystem>();
	MenuManager::GetInstance().GetMenu<MenuSelectSaveSlot>();
	MenuManager::GetInstance().GetMenu<MenuConfirm>();
	auto titleBGM = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/BGM/Title/Peritune_Zephyr_Fields_loop.mp3");
	auto debugSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/ButtonPush_Debug.mp3");
	auto goalSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/GoalSE.mp3");
	auto selectSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/MoveButton.mp3");
	LoadManager::GetInstance().SetOnComplete([&engine, this, titleBGM, debugSE, goalSE, selectSE]() {
		AudioUtility::RegisterBGMHandle("TitleBGM", titleBGM->GetHandle());
		AudioUtility::RegisterSEHandle("DebugSE", debugSE->GetHandle());
		AudioUtility::RegisterSEHandle("GoalSE", goalSE->GetHandle());
		AudioUtility::RegisterSEHandle("SelectSE", selectSE->GetHandle());
		auto loadBG = std::make_shared<LoadAnimation_ChangeBackground>();
		SetupData(engine);
	});
}
/*
 *  ロード済みデータの設定 (コールバック)
 */
void TitleScene::SetupData(Engine& engine) {
	isStart = true;
	MenuManager::GetInstance().OpenMenu<MenuTitle>();
}
/*
 *	@brief	準備前処理
 */
void TitleScene::Setup(Engine& engine) {

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
	DrawBox(0, 0, GameConst::WINDOW_WIDTH, GameConst::WINDOW_HEIGHT, GetColor(0, 0, 0), TRUE);
#if _DEBUG
	DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] 描画中...");
#endif
}