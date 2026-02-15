/*
 *	@file	MenuManager.cpp
 *	@author	Seki
 */

#include "MenuManager.h"
#include "../Manager/FontManager.h"

/*
 *	@brief	初期化処理
 */
void MenuManager::Initialize(Engine& engine) {
	this->engine = &engine;
	fontNameList.push_back("NormalSizeFont");
	fontNameList.push_back("SaveSlotFont");
	fontNameList.push_back("BuyItem");
	fontNameList.push_back("MiniSizeFont");
	FontManager::GetInstance().LoadFont(86, fontNameList[0]);
	FontManager::GetInstance().LoadFont(16, fontNameList[1]);
	FontManager::GetInstance().LoadFont(64, fontNameList[2]);
	FontManager::GetInstance().LoadFont(32, fontNameList[3]);
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
	for (auto& menu : useMenuList) {
		if (menu->IsVisible() && menu->IsAnimation()) menu->AnimUpdate(*engine, unscaledDeltaTime);
	}
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

	if (useMenuList.empty()) return;
	// 現在一番後ろのメニューの再開
	useMenuList.back()->Resume();
}
/*
 *	@brief		使用リストから特定のメニューを削除する
 *	@param[in]	MenuBasePtr closeMenu
 */
void MenuManager::CloseMenu(MenuBasePtr closeMenu) {
	if (useMenuList.empty() || !closeMenu) return;
	// 対象メニューを探す
	auto itr = std::find(useMenuList.begin(), useMenuList.end(), closeMenu);
	if (itr == useMenuList.end()) return;
	// それが一番後ろのメニューかどうか判定
	bool isTop = (itr == useMenuList.end() - 1);

	// 閉じる
	(*itr)->Close(*engine);
	// リストから削除
	useMenuList.erase(itr);

	// 一番上を閉じた場合のみ、次のメニューを再開
	if (isTop && !useMenuList.empty()) {
		useMenuList.back()->Resume();
	}
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