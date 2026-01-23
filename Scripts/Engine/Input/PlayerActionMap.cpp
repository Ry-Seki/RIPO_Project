#include "PlayerActionMap.h"

/*
 *	入力の更新
 */
void PlayerActionMap::InputUpdate() {
	state.axis.clear();
	state.button.clear();

	// 入力のタイプ別で入力判定
	for (auto binding : bindings) {
		switch (binding.type){
		case InputManager::BindingType::Axis:
			break;
		case InputManager::BindingType::Button:
			break;
		}
	}
}
