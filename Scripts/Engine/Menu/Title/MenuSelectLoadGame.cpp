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
#include "../System/MenuLoadMode.h"

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
	MenuBase::Open();
	auto& menu = MenuManager::GetInstance();
	menu.OpenMenu<MenuLoadMode>();
}
/*
 *	@brief	更新処理
 */
void MenuSelectLoadGame::Update(Engine& engine, float unscaledDeltaTime) {
	if (!isStart) return;
}
/*
 *	@biref	描画処理
 */
void MenuSelectLoadGame::Render() {
}
/*
 *	@brief	メニューを閉じる
 */
void MenuSelectLoadGame::Close(Engine& engine) {
	MenuBase::Close(engine);
}
/*
 *	@brief	メニューを再開
 */
void MenuSelectLoadGame::Resume() {
	MenuBase::Resume();
	auto& menu = MenuManager::GetInstance();
	menu.CloseTopMenu();
}
