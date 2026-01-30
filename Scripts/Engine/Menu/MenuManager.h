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
#include <type_traits>
#include <algorithm>  

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
	/*
	 *	@brief	コンストラクタ
	 */
	MenuManager() = default;
	/*
	 *	@brief	デストラクタ
	 */
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
	void CloseTopMenu();
	/*
	 *	@brief		現在開かれている全てのリストを削除する
	 */
	void CloseAllMenu();

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
	 *  @return		std::shared_ptr<T>(MenuBaseの派生クラス)
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	std::shared_ptr<T> GetMenu() {
		for (auto& menu : unuseMenuList) {
			auto cast = std::dynamic_pointer_cast<T>(menu);
			if (!cast) continue;

			return cast;
		}
		return CreateMenu<T>();
	}
	/*
	 *	@brief		未使用リストから使用リストへ変更する
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	void OpenMenu() {
		auto menu = GetMenu<T>();
		if (!menu) return;

		if (!useMenuList.empty()) useMenuList.back()->Suspend();

		// すでに使用中リストにあるか判定
		auto itr = std::find(useMenuList.begin(), useMenuList.end(), menu);
		if (itr != useMenuList.end()) {
			// 既存メニューの場合は再開
			menu->Resume();
			useMenuList.erase(itr);
		} else {
			// 初回表示
			menu->Open();
		}
		useMenuList.push_back(menu);
	}
};

#endif // !_MENU_MANAGER_H_