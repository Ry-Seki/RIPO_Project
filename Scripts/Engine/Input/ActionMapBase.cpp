/*
 *	@file	ActionMapBase.cpp
 *  @author Riku
 */

#include "ActionMapBase.h"
#include "InputManager.h"

void ActionMapBase::InputUpdate() {
	// 入力のタイプ別で入力判定
	for (auto& bind : bindings) {
		switch (bind.type) {
		case BindingType::Axis:{
			// 軸入力の+側
			if (InputManager::GetInstance().IsInput(bind.positive)) {
				state.axis[bind.action] += 1.0f;
			}
			// 軸入力の-側
			if (InputManager::GetInstance().IsInput(bind.negative)) {
				state.axis[bind.action] -= 1.0f;
			}
			break;
		}
		case BindingType::Button:{
			// ボタン入力
			auto& input = InputManager::GetInstance();
			// 押した瞬間
			if (input.IsInputDown(bind.positive)) {
				state.buttonDown[bind.action] = true;
			}
			// 押している間
			if (input.IsInput(bind.positive)) {
				state.button[bind.action] = true;
			}
			// 離した瞬間
			if (input.IsInputUp(bind.positive)) {
				state.buttonUp[bind.action] = true;
			}
			break;
		}
		}
	}
}
