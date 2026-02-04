/*
 *	@file	MenuResourcesFactory.h
 *	@author	Seki
 */

#ifndef _MENU_RESOURCES_FACTORY_H_
#define _MENU_RESOURCES_FACTORY_H_

#include "../Engine.h"
#include "../JSON.h"

// 前方宣言
struct MenuInfo;

/*
 *	@brief	メニュークラスに必要なリソースを生成するクラス
 */
class MenuResourcesFactory {

public:
	/*
	 *	@brief		メニュークラスに必要なリソースの生成
	 *	@param[in]	const JSON& json
	 *	@return		MenuInfo
	 */
	static MenuInfo Create(const JSON& json);
};

#endif // !_MENU_RESOURCES_FACTORY_H_