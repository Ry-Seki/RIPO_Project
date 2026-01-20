/*
 *	@file	MenuManager.h
 *	@author	Seki
 */

#ifndef _MENU_MANAGER_H_
#define _MENU_MANAGER_H_

#include "../Singleton.h"
#include "MenuBase.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;

 /*
  *	メニューの管理クラス
  */
class MenuManager : public Singleton<MenuManager> {
	friend class Singleton<MenuManager>;
public:		// 別名定義
	using MenuBasePtr = std::shared_ptr<MenuBase>;

private:
	std::vector<MenuBasePtr> unuseMenuList;		// 未使用メニューリスト
	std::vector<MenuBasePtr> useMenuList;		// 使用中メニューリスト

private:
	MenuManager() = default;
	~MenuManager() = default;

public:
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float unscaledDeltaTime);
	/*
	 *	@brief	描画処理
	 */
	void Render();
	/*
	 *	@brief		使用リストから一番後ろの要素を削除する
	 */
	void RemoveBack();

private:
	/*
	 *	@brief		メニューの生成
	 *  @return		std::shared_ptr<T>(MenuBaseの派生クラス)
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	std::shared_ptr<T> CreateMenu() {
		auto createMenu = std::make_shared<T>();
		if (!createMenu) return nullptr;

		createMenu->Initialize();
		unuseMenuList.push_back(createMenu);
		return createMenu;
	}

public:
	/*
	 *	@brief		メニューの取得
	 *  @param[in]	bool isUsedMenu = false		使用中メニューフラグ
	 *  @return		std::shared_ptr<T>(MenuBaseの派生クラス)
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	std::shared_ptr<T> GetMenu(bool isUsedMenu = false) {
		if (isUsedMenu) {
			for (auto& menu : useMenuList) {
				auto cast = std::dynamic_pointer_cast<T>(menu);
				if (!cast) continue;

				return cast;
			}
			return nullptr;
		}else {
			for (auto& menu : unuseMenuList) {
				auto cast = std::dynamic_pointer_cast<T>(menu);
				if (!cast) continue;

				return cast;
			}
			return CreateMenu<T>();
		}
		return nullptr;
	}
	/*
	 *	@brief		未使用リストから使用リストへ変更する
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	void ActiveMenu() {
		auto menu = GetMenu<T>();
		if (!menu) return;

		unuseMenuList.erase(
			std::remove(unuseMenuList.begin(), unuseMenuList.end(), menu),
			unuseMenuList.end());
		menu->Open();
		useMenuList.push_back(menu);
	}
};

#endif // !_MENU_MANAGER_H_