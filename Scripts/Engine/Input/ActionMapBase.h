/*
 *	@file	ActionMapBase.h
 *  @author Riku
 */

#ifndef _ACTION_MAP_BASE_H_
#define _ACTION_MAP_BASE_H_

#include "InputManager.h"

#include <vector>

/*
 *	アクションマップの基底クラス
 */
class ActionMapBase {
protected:
	std::vector<InputManager::Binding> bindings;

public:
	ActionMapBase() = default;
	~ActionMapBase() = default;

public:
	/*
	 *	軸入力設定
	 *	@param	int	action		行動設定
	 *	@param	Input positive	+方向の入力ボタン
	 *	@param	Input negative	-方向の入力ボタン
	 */
	void AddAxis(int action, InputManager::Input positive, InputManager::Input negative) {
		bindings.push_back({ action, InputManager::BindingType::Axis, positive, negative });
	}

	/*
	 *	ボタン入力設定
	 *	@param	int action		行動設定
	 *	@param	Input input		ボタン設定ボタン
	 */
	void AddButton(int action, InputManager::Input input) {
		bindings.push_back({ action, InputManager::BindingType::Button, input, input });
	}

	/*
	 *	入力の更新
	 */
	virtual void InputUpdate() = 0;
};

#endif // !_ACTION_MAP_BASE_H_