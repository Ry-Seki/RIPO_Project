/*
 *	@file	MenuTitle.cpp
 *	@author	Seki
 */

#include "MenuTitle.h"
#include "../../Load/LoadManager.h"
#include "../../Load/Sprite/LoadSprite.h"

/*
 *	@brief	初期化処理
 */
void MenuTitle::Initialize() {
	LoadManager& load = LoadManager::GetInstance();
	auto menuTitleHandle = load.LoadResource<LoadSprite>(_TITLE_LOGO_PATH);
	load.SetOnComplete([this, menuTitleHandle]() {
		SetupData(menuTitleHandle->GetHandle());
	});
}
/*
 *	@brief	メニューを開く処理
 */
void MenuTitle::Open() {
	MenuBase::Open();
}
/*
 *	@brief	更新処理
 */
void MenuTitle::Update(Engine& engine, float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void MenuTitle::Render() {
	DrawGraph(0, 0, titleGraphHandle, TRUE);
}
/*
 *	@brief	メニューを閉じる処理
 */
void MenuTitle::Close() {
	MenuBase::Close();
}
/*
 *	@brief		ロード済みデータのセット
 *  @prarm[in]	int setHandle
 */
void MenuTitle::SetupData(int setHandle) {
	titleGraphHandle = setHandle;
}
