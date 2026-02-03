/*
 *	@file	MenuSelectLoadGame.cpp
 *	@author	Seki
 */

#include "MenuSelectLoadGame.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"
#include "../../Audio/AudioUtility.h"
#include "../../Menu/MenuManager.h"
#include "../../Engine.h"
#include "../../Scene/MainGameScene.h"
#include "../System/MenuSelectSaveSlot.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuSelectLoadGame::Initialize(Engine& engine) {
}
/*
 *	@brief	メニューを開く
 */
void MenuSelectLoadGame::Open() {
	auto& menu = MenuManager::GetInstance();
	auto saveMenu = menu.GetMenu<MenuSelectSaveSlot>();
	saveMenu->SetSaveMode(mode);
	MenuBase::Open();
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this, &menu]() {
		isStart = true;
		menu.OpenMenu<MenuSelectSaveSlot>();
	});
}
/*
 *	@brief	更新処理
 */
void MenuSelectLoadGame::Update(Engine& engine, float unscaledDeltaTime) {
	if (!isStart) return;
	// TODO : セーブデータとの同期を完了させる
	MenuManager& menu = MenuManager::GetInstance();
	if (!inputHandle) {
		if (CheckHitKey(KEY_INPUT_SPACE)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			engine.StartFadeOutIn(0.5f, 0.5f, [&engine, &menu]() {
				menu.CloseAllMenu();
				engine.SetNextScene(std::make_shared<MainGameScene>());
			});
		} else if (CheckHitKey(KEY_INPUT_RETURN)) {
			AudioUtility::PlaySE("DebugSE");
			inputHandle = true;
			FadeBasePtr fadeOut = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::Out, FadeMode::Stop);
			FadeManager::GetInstance().StartFade(fadeOut, [this, &menu]() {
				menu.CloseTopMenu();
			});
		}
	}
}
/*
 *	@biref	描画処理
 */
void MenuSelectLoadGame::Render() {
	DrawFormatString(50, 70, GetColor(255, 255, 255), "LoadGame");
	DrawFormatString(300, 400, GetColor(255, 255, 255), "Play->SpaceKey");
	DrawFormatString(300, 450, GetColor(255, 255, 255), "Return->EnterKey");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectLoadGame::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuSelectLoadGame::Suspend() {

}
/*
 *	@brief	メニューを再開
 */
void MenuSelectLoadGame::Resume() {
}
