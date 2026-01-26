/*
 *	@file	ActionMapBase.cpp
 *  @author Riku
 */

#include "ActionMapBase.h"
#include "InputManager.h"

void ActionMapBase::InputUpdate() {
	// 入力のタイプ別で入力判定
	for (auto b : bindings) {
		switch (b.type) {
		case BindingType::Axis:
			// 軸入力の+側
			if (InputManager::GetInstance().IsInput(b.positive)) {
				state.axis[b.action] += 1.0f;
			}
			// 軸入力の-側
			if (InputManager::GetInstance().IsInput(b.negative)) {
				state.axis[b.action] -= 1.0f;
			}
			break;
		case BindingType::Button:
			// ボタン入力
			if (InputManager::GetInstance().IsInput(b.positive)) {
				state.button[b.action] = true;
			}
			break;
		}
	}
}
