/*
 *	@file	UIButton.h
 *	@author	Seki
 */

#ifndef _UI_BUTTON_H_
#define _UI_BUTTON_H_

#include "../../GameEnum.h"

#include <functional>

/*
 *	@brief	UIボタンクラス
 */
class UIButton {
private:
	bool isSelected = false;

	std::function<void()> onClick = nullptr;

public:

};

#endif // !_UI_BUTTON_H_