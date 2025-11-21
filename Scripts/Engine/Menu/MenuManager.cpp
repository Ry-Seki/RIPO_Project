/*
 *	@file	MenuManager.cpp
 *	@author	Seki
 */

#include "MenuManager.h"

/*
 *	@brief	更新処理
 */
void MenuManager::Update(Engine& engine, float unscaledDeltaTime) {
	if (useMenuList.empty()) return;
	useMenuList.back()->Update(engine, unscaledDeltaTime);
}
/*
 *	@brief	描画処理
 */
void MenuManager::Render() {
	if (useMenuList.empty()) return;
	useMenuList.back()->Render();
}
/*
 *	@brief	使用リストから一番後ろの要素を削除する
 */
void MenuManager::RemoveBack() {
	if (useMenuList.empty()) return;
	auto menu = useMenuList.back();
	if (!menu) return;

	useMenuList.pop_back();
	menu->Close();
	unuseMenuList.push_back(menu);
}