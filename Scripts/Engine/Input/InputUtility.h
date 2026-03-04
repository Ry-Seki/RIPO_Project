/*
 *	@file	InputUtility.h
 *	@author Riku
 */

#ifndef _INPUT_UTILITY_H_
#define _INPUT_UTILITY_H_

#include "InputManager.h"

/*
 *	入力関連実行処理
 *  @note	名前空間での省略可能	using namespace InputUtility
 */
namespace InputUtility {
	/*
	 *	各アクションマップの有効状態切り替え
	 *	@param[in]	GameEnum::ActionMap map	切り替えるアクションマップ
	 *  @param[in]	bool setActive			切り替え値
	 *  @author Riku
	 */
	inline void SetActionMapIsActive(GameEnum::ActionMap map, bool setActive) {
		InputManager::GetInstance().SetActionMapIsActive(map, setActive);
	}
	/*
	 *	各アクションマップの入力状態取得
	 *	@param[in]	GameEnum::ActionMap map	取得するアクションマップ
	 *	@return		ActionMapBase::ActionState
	 *  @author Riku
	 */
	inline ActionMapBase::ActionState GetInputState(GameEnum::ActionMap map) {
		return InputManager::GetInstance().GetInputState(map);
	}
	/*
	 *	マウスの表示非表示切り替え
	 *	@param[in]	bool setVisible	切り替え先
	 */
	inline void SetMouseVisible(bool setVisible) {
		InputManager::GetInstance().SetMouseVisible(setVisible);
	}
	/*
	 *	マウスの位置取得
	 *	@param[out]	int mousePosX	マウスの位置X
	 *	@param[out] int mousePosY	マウスの位置Y
	 *  @outhor Riku
	 */
	inline void GetMousePosition(int& mousePosX, int& mousePosY) {
		InputManager::GetInstance().GetMousePosition(mousePosX, mousePosY);
	}
	/*
	 *	直前の入力がマウスかどうか取得
	 *  @return bool
	 *  @author Riku
	 */
	inline bool GetPrevInputMouse() {
		return InputManager::GetInstance().GetPrevInputMouse();
	}
}

#endif // !_INPUT_UTILITY_H_
