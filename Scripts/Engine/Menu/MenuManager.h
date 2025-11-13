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

/*
 *	メニューの管理クラス
 */
class MenuManager : public Singleton<MenuManager>{
	friend class Singleton<MenuManager>;
private:
	std::vector<MenuBasePtr> menuList;

private:
	/*
	 *	コンストラクタ
	 */
	MenuManager() = default;
	/*
	 *	デストラクタ
	 */
	~MenuManager() = default;

private:
	/*
	 *	メニューの生成
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	std::shared_ptr<T> CreateMenu() {
		auto createMenu = std::shared_ptr<T>();
		if (!createMenu) return nullptr;

		menuList.push_back(createMenu);
		return createMenu;
	}

public:
	/*
	 *	メニューのゲット
	 *	@return	std::shred_ptr<T>
	 */
	template <class T, typename = std::enable_if_t<std::is_base_of_v<MenuBase, T>>>
	std::shared_ptr<T> GetMenu() {
		for (int i = 0, max = menuList.size(); i < max; i++) {
			std::shared_ptr<T> menu = std::dynamic_pointer_cast(menuList[i]);
			if (!menu) continue;

			return menu;
		}
		return CreateMenu();
	}
};
#endif // !_MENU_MANAGER_H_