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
#include "../../Menu/Title/MenuSelectNewGame.h"
#include "../../Menu/Title/MenuSelectLoadGame.h"
#include "../../Menu/Title/MenuSystem.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuGameModeSelect::Initialize(Engine& engine) {
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
void MenuGameModeSelect::Update(Engine& engine, float unscaledDeltaTime) {
	if (!isStart) return;

	MenuManager& menu = MenuManager::GetInstance();
	// TODO : モードによって処理を変える
	if (!inputHandle) {
		if (CheckHitKey(KEY_INPUT_1)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, 1.0f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				isVisible = false;
				menu.OpenMenu<MenuSelectNewGame>();
			});
		} else if (CheckHitKey(KEY_INPUT_2)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, 1.0f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				isVisible = false;
				menu.OpenMenu<MenuSelectLoadGame>();
			});
		} else if (CheckHitKey(KEY_INPUT_3)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, 1.0f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				isVisible = false;
				menu.OpenMenu<MenuSystem>();
			});
		} else if (CheckHitKey(KEY_INPUT_ESCAPE)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::InkSpread, 1.0f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				menu.CloseTopMenu();
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
	DrawFormatString(300, 450, GetColor(255, 255, 255), "Return->EscapeKey");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuGameModeSelect::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuGameModeSelect::Resume() {
	MenuBase::Resume();
	isStart = false;
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this]() {
		isStart = true;
		inputHandle = false;
	});
}
