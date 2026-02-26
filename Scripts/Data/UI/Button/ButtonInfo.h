/*
 *	@file	ButtonInfo.h
 *	@author	Seki
 */

#ifndef _BUTTON_INFO_H_
#define _BUTTON_INFO_H_

#include "../../../Engine/GameEnum.h"
#include "../../../Engine/UI/Rect.h"

#include <vector>
#include <string>

/*
 *	@brief	ƒ{ƒ^ƒ“î•ñ\‘¢‘Ì
 */
struct ButtonInfo {
	int x;
	int y;
	std::string name;
	GameEnum::ButtonType type;
	Rect rect;
	std::vector<std::string> resourceList;
};

#endif // !_BUTTON_INFO_H_