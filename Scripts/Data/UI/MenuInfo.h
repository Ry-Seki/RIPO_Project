/*
 *	@file	MenuInfo.h
 *	@author	Seki
 */

#ifndef _MENU_INFO_H_
#define _MENU_INFO_H_

#include <vector>
#include <memory>

// 前方宣言
class UIButtonBase;
class Sprite;
class TextBase;

/*
 *	@brief	メニューにて生成するUI構造体
 */
struct MenuInfo {
	std::vector<std::shared_ptr<Sprite>> spriteList;
	std::vector<std::shared_ptr<TextBase>> textList;
	std::vector<std::shared_ptr<UIButtonBase>> buttonList;
};
#endif // !_MENU_INFO_H_