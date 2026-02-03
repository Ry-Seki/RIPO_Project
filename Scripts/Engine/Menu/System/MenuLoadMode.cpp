/*
 *	@file	MenuLoadMode.cpp
 *	@author	Seki
 */

#include "MenuLoadMode.h"
#include "MenuSelectSaveSlot.h"
#include "../MenuManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuLoadMode::Initialize(Engine& engine) {
}
/*
 *	@brief	メニューを開く
 */
void MenuLoadMode::Open() {
	MenuBase::Open();
	auto& menu = MenuManager::GetInstance();
	auto saveMenu = menu.GetMenu<MenuSelectSaveSlot>();
	menu.OpenMenu<MenuSelectSaveSlot>();
	saveMenu->SetSaveMode(mode);
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this, &menu]() {
		isStart = true;
	});
}
/*
 *	@brief	更新処理
 */
void MenuLoadMode::Update(Engine& engine, float unscaledDeltaTime) {
}
/*
 *	@brief	描画処理
 */
void MenuLoadMode::Render() {
	DrawFormatString(50, 100, GetColor(255, 255, 255), "MenuLoadMode");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuLoadMode::Close(Engine& engine) {
}
/*
 *	@brief	メニューを中断
 */
void MenuLoadMode::Suspend() {
}
/*
 *	@brief	メニューを再開
 */
void MenuLoadMode::Resume() {
	MenuBase::Resume();
	auto& menu = MenuManager::GetInstance();
	menu.CloseTopMenu();
}
