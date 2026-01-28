/*
 *	@file	UIButton.cpp
 *	@author	Seki
 */

#include "UIButton.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void UIButton::Initialize() {
	// TODO : 各描画処理を生成、初期化する

}
/*
 *	@brief	更新処理
 */
void UIButton::Update(float deltaTime) {
	int mouseX = 0, mouseY = 0;
	// マウスポインタの取得
	GetMousePoint(&mouseX, &mouseY);
	// 触れているか判定
	isHovered = rect.IsHovered(mouseX, mouseY);
	// 触れている状態でクリック判定
	// TODO : ここの入力はのちにInputManager経由にする
	if (isHovered && CheckHitKey(MOUSE_INPUT_LEFT)) {
		isPressed = true;
		Execute();
	} else {
		isPressed = false;
	}
}
/*
 *	@brief	実行処理
 */
void UIButton::Execute() {
	if (!onClick) return;
	// 登録された関数実行
	onClick();
}
/*
 *	@brief	描画処理
 */
void UIButton::Render() {
	GameEnum::ButtonState state = GetButtonState();
	int index = static_cast<int>(state);

	if (index < 0 || index >= rendererList.size()) return;
	if (index < 0 || index >= buttonHandleList.size()) return;

	// 描画処理
	rendererList[index]->Draw(rect, buttonHandleList[index],name);
}
/*
 *	@brief		ボタン状態の取得
 *	@return		GameEnum::ButtonState
 */
GameEnum::ButtonState UIButton::GetButtonState() const {
	if (!isEnable)  return GameEnum::ButtonState::Disable;
	if (isPressed)  return GameEnum::ButtonState::Pressed;
	if (isHovered)  return GameEnum::ButtonState::Hovered;
	if (isSelected) return GameEnum::ButtonState::Selected;

	return GameEnum::ButtonState::Idle;
}
