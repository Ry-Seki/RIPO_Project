/*
 *	@file	UIButton.cpp
 *	@author	Seki
 */

#include "UIButton.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void UIButton::Initialize() {
	// TODO : 各描画処理を生成、初期化する

}
/*
 *	@brief	準備前処理
 */
void UIButton::Setup() {
	buttonState = GameEnum::ButtonState::Idle;
}
/*
 *	@brief	更新処理
 */
void UIButton::Update(float deltaTime) {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

	int mouseX = 0, mouseY = 0;
	// マウスポインタの取得
	GetMousePoint(&mouseX, &mouseY);
	// 触れているか判定
	if (rect.IsHovered(mouseX, mouseY)) buttonState = GameEnum::ButtonState::Hovered;
	// 触れている状態でクリック判定
	if (buttonState == GameEnum::ButtonState::Hovered && input.buttonDown[static_cast<int>(GameEnum::MenuAction::Click)]) {
		buttonState = GameEnum::ButtonState::Pressed;
		Execute();
	}
}
/*
 *	@brief	実行処理
 */
void UIButton::Execute() {
	// 登録された関数実行
	if (!onClick) return;
	onClick();
}
/*
 *	@brief	描画処理
 */
void UIButton::Render() {
	GameEnum::ButtonState state = GetButtonState();
	int index = static_cast<int>(state);
	const bool canDrawNormal =
		index >= 0 &&
		index < rendererList.size() &&
		index < buttonHandleList.size() &&
		rendererList[index] &&
		buttonHandleList[index] >= 0;

	if (canDrawNormal) {
		rendererList[index]->Draw(rect, buttonHandleList[index], name);
		return;
	}

	// デバッグ描画
	DebugRender();
}
/*
 *	@brief	デバック用描画
 */
void UIButton::DebugRender() {
	int color = GetColor(255, 255, 255);

	switch (GetButtonState()) {
		case GameEnum::ButtonState::Hovered:
			color = GetColor(255, 255, 0);
			break;
		case GameEnum::ButtonState::Pressed:
			color = GetColor(255, 0, 0);
			break;
		case GameEnum::ButtonState::Disable:
			color = GetColor(128, 128, 128);
			break;
		case GameEnum::ButtonState::Selected:
			color = GetColor(0, 0, 255);
			break;
		default:
			color = GetColor(255, 255, 255);
			break;
	}

	DrawBox(
		rect.x,
		rect.y,
		rect.x + rect.width,
		rect.y + rect.height,
		color,
		FALSE
	);

	DrawString(
		rect.x + 8,
		rect.y + 8,
		name.c_str(),
		color
	);
}
/*
 *	@brief		ボタン状態の取得
 *	@return		GameEnum::ButtonState
 */
GameEnum::ButtonState UIButton::GetButtonState() const {
	return buttonState;
}
