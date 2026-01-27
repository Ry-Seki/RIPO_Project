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
	
}
/*
 *	@brief	更新処理
 */
void UIButton::Update(float deltaTime) {
	int mouseX = 0, mouseY = 0;
	// マウスポインタの取得
	GetMousePoint(&mouseX, &mouseY);
	// マウスが触れているか判定
	if (rect.IsHovered(mouseX, mouseY)) {
		isHovered = true;
		buttonState = GameEnum::ButtonState::Hover;
	} else {
		isHovered = false;
		buttonState = GameEnum::ButtonState::Idle;
	}
	// TODO : ここをのちにInputManagerの入力に変更する
	if (isHovered && CheckHitKey(MOUSE_INPUT_LEFT)) {
		isPressed = true;
		buttonState = GameEnum::ButtonState::Pressed;
		Execute();
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

}
