/*
 *	@file	UIButtonBase.cpp
 *	@author	Seki
 */

#include "UIButtonBase.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void UIButtonBase::Initialize() {
	int indexMax = static_cast<int>(GameEnum::ButtonRendererState::Max);
	handleList.resize(indexMax);
	isEnable = true;
	inputState = GameEnum::ButtonInputState::Idle;
	selectState = GameEnum::ButtonSelectState::Invalid;
	inputClickNum = static_cast<int>(GameEnum::MenuAction::Click);
	for (int i = 0; i < indexMax; i++) {
		handleList[i] = -1;
	}
}
/*
 *	@brief	準備前処理
 */
void UIButtonBase::Setup() {
	isEnable = true;
	isVisible = true;
	inputState = GameEnum::ButtonInputState::Idle;
}
/*
 *	@brief	更新処理
 */
void UIButtonBase::Update(float unscaledDeltaTime) {
	if (!isEnable) return;
	// マウスポインタの取得
	int mouseX, mouseY;
	GetMousePoint(&mouseX, &mouseY);

	// 触れているか判定
	bool isHover = rect.IsHovered(mouseX, mouseY);
	if (inputState != GameEnum::ButtonInputState::Press) {
		if (isHover) {
			inputState = GameEnum::ButtonInputState::Hover;
		} else {
			inputState = GameEnum::ButtonInputState::Idle;
		}
	}
}
/*
 *	@brief	発火イベント
 */
void UIButtonBase::OnClickEvent() {
	if (OnClick) OnClick();
}
/*
 *	@brief	デバック用描画処理
 */
void UIButtonBase::DebugRender() {
	int color = GetColor(255, 255, 255);

	switch (GetRendererState()) {
		case GameEnum::ButtonRendererState::Hover:
			color = GetColor(255, 255, 0);
			break;
		case GameEnum::ButtonRendererState::Press:
			color = GetColor(255, 0, 0);
			break;
		case GameEnum::ButtonRendererState::Disable:
			color = GetColor(128, 128, 128);
			break;
		case GameEnum::ButtonRendererState::Select:
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
	if (!name.empty()) {
		DrawFormatString(rect.x + 50, rect.y + rect.height / 2, GetColor(255, 255, 255), name.c_str());
	}
}
/*
 *	@brief		描画状態の取得
 *	@return		GameEnum::ButtonRendererState
 */
GameEnum::ButtonRendererState UIButtonBase::GetRendererState() const {
	if (!isEnable) 
		return GameEnum::ButtonRendererState::Disable;

	if (inputState == GameEnum::ButtonInputState::Press)
		return GameEnum::ButtonRendererState::Press;

	if (selectState == GameEnum::ButtonSelectState::Select)
		return GameEnum::ButtonRendererState::Select;

	if (inputState == GameEnum::ButtonInputState::Hover)
		return GameEnum::ButtonRendererState::Hover;

	return GameEnum::ButtonRendererState::Idle;
}
/*
 *	@brief		押していたものが離れた瞬間を判定
 *	@return		bool
 */
bool UIButtonBase::OnReleasedUp() const {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

	return input.buttonUp[static_cast<int>(GameEnum::MenuAction::Click)]
		|| input.buttonUp[static_cast<int>(GameEnum::MenuAction::Decide)];
}
