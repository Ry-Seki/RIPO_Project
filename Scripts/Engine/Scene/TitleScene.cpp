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

#include <DxLib.h>
#include <memory>

using namespace AudioUtility;

 /*
  *  初期化処理
  */
void TitleScene::Initialize(Engine& engine) {
	isStart = false;
	inputHandle = false;
	auto fadeIn = FadeFactory::CreateFade(FadeType::Black, 0.5f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn);
	SetBGMVolume(100);
	SetSEVolume(100);
	// MenuManager::GetInstance().GetMenu<MenuTitle>()->Initialize();
	auto debugSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/ButtonPush_Debug.mp3");
	auto goalSE = LoadManager::GetInstance().LoadResource<LoadAudio>("Res/Audio/SE/GoalSE.mp3");
	std::vector<std::shared_ptr<LoadSprite>> loadBGList;
	loadBGList.push_back(LoadManager::GetInstance().LoadResource<LoadSprite>("Res/BackGround/Trealine_LoadBackground_black1.jpg"));
	loadBGList.push_back(LoadManager::GetInstance().LoadResource<LoadSprite>("Res/BackGround/Trealine_LoadBackground_black2.jpg"));
	loadBGList.push_back(LoadManager::GetInstance().LoadResource<LoadSprite>("Res/BackGround/Trealine_LoadBackground_black3.jpg"));
	LoadManager::GetInstance().SetOnComplete([&engine, this, debugSE, goalSE, loadBGList]() {
		RegisterSEHandle("DebugSE", debugSE->GetHandle());
		RegisterSEHandle("GoalSE", goalSE->GetHandle());
		auto loadBG = std::make_shared<LoadAnimation_ChangeBackground>();
		std::vector<int> BGHandleList;
		for (int i = 0, max = loadBGList.size(); i < max; i++) {
			int handle = loadBGList[i]->GetHandle();
			BGHandleList.push_back(handle);
		}
		loadBG->SetImages(BGHandleList);
		LoadManager::GetInstance().AddAnimation(loadBG);
		SetupData(engine);
	});
}
/*
 *  ロード済みデータの設定 (コールバック)
 */
void TitleScene::SetupData(Engine& engine) {
	isStart = true;
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
	if (!isStart) return;

	if (!inputHandle) {
		if (CheckHitKey(KEY_INPUT_1)) {
			AudioUtility::PlaySE("DebugSE");
			gameMode = GameEnum::GameMode::NewGame;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine]() {
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		} else if (CheckHitKey(KEY_INPUT_2)) {
			AudioUtility::PlaySE("DebugSE");
			gameMode = GameEnum::GameMode::LoadGame;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine]() {
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		} else if (CheckHitKey(KEY_INPUT_3)) {
			AudioUtility::PlaySE("DebugSE");
			gameMode = GameEnum::GameMode::System;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine]() {
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		}
	}
}
/*
 *  描画処理
 */
void TitleScene::Render() {
	DrawFormatString(50, 50, GetColor(255, 255, 255), "[TitleScene] 描画中...");
	DrawFormatString(50, 70, GetColor(255, 255, 255), "=== Selection GameMode ===");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "1: NewGame");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "2: LoadGame");
	DrawFormatString(50, 140, GetColor(0, 255, 0), "3: System");
}