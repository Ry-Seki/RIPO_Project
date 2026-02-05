/*
 *	@file	LongPressButton.cpp
 *	@author	Seki
 */

#include "LongPressButton.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void LongPressButton::Initialize() {
	UIButtonBase::Initialize();
}
/*
 *	@brief	準備前処理
 */
void LongPressButton::Setup() {
	UIButtonBase::Setup();
}
/*
 *	@brief	更新処理
 */
void LongPressButton::Update(float unscaledDeltaTime) {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

	UIButtonBase::Update(unscaledDeltaTime);

	if (inputState == GameEnum::ButtonInputState::Hover &&
		input.buttonDown[inputClickNum]) {
		OnPressDown();
	}

	if (inputState == GameEnum::ButtonInputState::Press) {
		// 押している間の処理
		OnPress(unscaledDeltaTime);
		// 離された瞬間
		if (OnReleasedUp()) OnPressUp();
	}
}
/*
 *	@brief	描画処理
 */
void LongPressButton::Render() {
	// TODO : 描画インターフェースでの描画

	DebugRender();
}
/*
 *	@brief	押された瞬間
 */
void LongPressButton::OnPressDown() {
	inputState = GameEnum::ButtonInputState::Press;

	holdElapsed = 0.0f;
	repeatElapsed = 0.0f;

	// 即時発火
	OnClickEvent();
}
/*
 *	@brief	押下中
 */
void LongPressButton::OnPress(float unscaledDeltaTime) {
	holdElapsed += unscaledDeltaTime;

	// 初回待機
	if (holdElapsed >= initialDelay) {
		repeatElapsed += unscaledDeltaTime;

		if (repeatElapsed >= repeatInterval) {
			repeatElapsed -= repeatInterval;
			OnClickEvent();

			// 加速処理
			repeatInterval -= accelerationRate * unscaledDeltaTime;
			repeatInterval = (std::max) (repeatInterval, minRepeatInterval);
		}
	}
}
/*
 *	@brief	離れた瞬間
 */
void LongPressButton::OnPressUp() {
	inputState = GameEnum::ButtonInputState::Idle;

	holdElapsed = 0.0f;
	repeatElapsed = 0.0f;

	// 次回のために初期値へ戻す
	repeatInterval = 0.3f;
}