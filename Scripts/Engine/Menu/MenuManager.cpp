/*
 *	@file	MenuManager.cpp
 *	@author	Seki
 */

#include "MenuManager.h"

/*
 *	@brief	初期化処理
 */
void MenuManager::Initialize(Engine& engine) {
	this->engine = &engine;
}
/*
 *	@brief	更新処理
 */
void MenuManager::Update(float unscaledDeltaTime) {
	if (useMenuList.empty()) return;

	// 一番上のメニューのみ操作可能
	auto topMenu = useMenuList.back();
	if (topMenu->IsInteractive()) topMenu->Update(*engine, unscaledDeltaTime);

	// メニューの更新(操作以外)
}
/*
 *	@brief	描画処理
 */
void MenuManager::Render() {
	if (useMenuList.empty()) return;
	for (auto& menu : useMenuList) {
		if (!menu->IsVisible()) continue;

		menu->Render();
	}
}
/*
 *	@brief	使用リストから一番後ろの要素を削除する
 */
void MenuManager::CloseTopMenu() {
	if (useMenuList.empty()) return;

	auto topMenu = useMenuList.back();
	if (!topMenu) return;
	// 一番後ろの要素を削除する
	topMenu->Close(*engine);
	useMenuList.pop_back();
	// 現在一番後ろのメニューの再開
	useMenuList.back()->Resume();
}
/*
 *	@brief	現在開かれている全てのリストを削除する
 */
void MenuManager::CloseAllMenu() {
	if (useMenuList.empty()) return;

	for (auto& menu : useMenuList) {
		if (!menu) continue;
		menu->Close(*engine);
	}

	useMenuList.clear();
}