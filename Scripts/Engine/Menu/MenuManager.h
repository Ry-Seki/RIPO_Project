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

 /*
  *	メニューの管理クラス
  */
class MenuManager : public Singleton<MenuManager> {
	friend class Singleton<MenuManager>;
private:
	std::vector<MenuBasePtr> menuList;

private:
	MenuManager() = default;
	~MenuManager() = default;

private:
	/*
	 *	メニューの生成
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	std::shared_ptr<T> CreateMenu() {
		auto createMenu = std::make_shared<T>();
		menuList.push_back(createMenu);
		return createMenu;
	}

public:
	/*
	 *	メニューの取得
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	std::shared_ptr<T> GetMenu() {
		for (auto& menu : menuList) {
			auto cast = std::dynamic_pointer_cast<T>(menu);
			if (!cast) continue;

			return cast;
		}
		return CreateMenu<T>();
	}
};

#endif // !_MENU_MANAGER_H_