/*
 *	@file	MenuGameModeSelect.cpp
 *	@author	Seki
 */

#include "MenuGameModeSelect.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../../Engine/Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../../Menu/MenuManager.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuGameModeSelect::Initialize() {
}
/*
 *	@brief	メニューを開く
 */
void MenuGameModeSelect::Open() {
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void MenuGameModeSelect::Update(Engine& engine, float deltaTime) {
	if (!isStart) return;

	MenuManager& menu = MenuManager::GetInstance();
	// TODO : モードによって処理を変える
	if (!inputHandle) {
		if (CheckHitKey(KEY_INPUT_1)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine, &menu]() {
				menu.CloseAll();
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		} else if (CheckHitKey(KEY_INPUT_2)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine, &menu]() {
				menu.CloseAll();
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		} else if (CheckHitKey(KEY_INPUT_3)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine, &menu]() {
				menu.CloseAll();
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		} else if (CheckHitKey(KEY_INPUT_RETURN)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, 1.0f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				menu.CloseTop();
			});
		}
	}
}
/*
 *	@brief	描画処理
 */
void MenuGameModeSelect::Render() {
	DrawFormatString(50, 70, GetColor(255, 255, 255), "=== Selection GameMode ===");
	DrawFormatString(50, 100, GetColor(0, 255, 0), "1: NewGame");
	DrawFormatString(50, 120, GetColor(0, 255, 0), "2: LoadGame");
	DrawFormatString(50, 140, GetColor(0, 255, 0), "3: System");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuGameModeSelect::Close() {
	MenuBase::Close();
}
