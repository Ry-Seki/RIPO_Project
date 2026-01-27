/*
 *	@file	EventSystem.h
 *	@author	Seki
 */

#ifndef _EVENT_SYSTEM_H_
#define _EVENT_SYSTEM_H_

#include "../GameEnum.h"
#include "../JSON.h"

#include <vector>
#include <unordered_map>

// 前方宣言
class UIButton;

/*
 *	@brief	移動の道筋構造体
 */
struct Navigation {
	int left = -1;
	int right = -1;
	int up = -1;
	int down = -1;
};

/*
 *	@brief	入力によって選択を動かすクラス
 */
class EventSystem {
private:
	std::vector<UIButton*> buttonList;
	std::unordered_map<int, Navigation> navigationMap;

public:
	/*
	 *	@brief		ボタンの登録
	 *  @param[in]	UIButton* setButton
	 */
	inline void RegisterButton(UIButton* setButton) {
		buttonList.push_back(setButton);
	}
};

#endif // !_EVENT_SYSTEM_H_