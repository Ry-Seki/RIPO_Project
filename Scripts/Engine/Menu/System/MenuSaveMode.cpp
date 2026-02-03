/*
 *	@file	MenuSaveMode.cpp
 *	@author	Seki
 */

#include "MenuSaveMode.h"
#include "MenuSelectSaveSlot.h"
#include "../MenuManager.h"
#include "../../Fade/FadeFactory.h"
#include "../../Fade/FadeManager.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuSaveMode::Initialize(Engine& engine) {
}
/*
 *	@brief	メニューを開く
 */
void MenuSaveMode::Open() {
	MenuBase::Open();
	auto& menu = MenuManager::GetInstance();
	auto saveMenu = menu.GetMenu<MenuSelectSaveSlot>();
	saveMenu->SetSaveMode(mode);
	FadeBasePtr fadeIn = FadeFactory::CreateFade(FadeType::Black, 1.2f, FadeDirection::In, FadeMode::Stop);
	FadeManager::GetInstance().StartFade(fadeIn, [this, &menu]() {
		isStart = true;
		menu.OpenMenu<MenuSelectSaveSlot>();
	});
}
/*
 *	@brief	更新処理
 */  
void MenuSaveMode::Update(Engine& engine, float unscaledDeltaTime) {
}
/*
 *	@brief	描画処理
 */  
void MenuSaveMode::Render() {
	DrawFormatString(50, 100, GetColor(255, 255, 255), "MenuSaveMode");
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSaveMode::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを中断
 */
void MenuSaveMode::Suspend() {
	MenuBase::Suspend();
}
/*
 *	@brief	メニューを再開
 */
void MenuSaveMode::Resume() {
	MenuBase::Resume();
	auto& menu = MenuManager::GetInstance();
	menu.CloseTopMenu();
}
