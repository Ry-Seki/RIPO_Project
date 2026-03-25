/*
 *	@file	LongPressButton.cpp
 *	@author	Seki
 */

#include "LongPressButton.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

namespace {
	constexpr float _REPEATE_INTERVAL_PER = 0.3f;
}
/*
 *	@brief	ボタンの状態リセット
 */
void LongPressButton::ResetState() {
	UIButtonBase::ResetState();
	holdElapsed = 0.0f;
	repeatElapsed = 0.0f;

	// 次回のために初期値へ戻す
	repeatInterval = _REPEATE_INTERVAL_PER;
}
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
	if (handleList.empty()) return;
	int handle = handleList[static_cast<int>(GetRendererState())];

	if (handle != -1) {
		DrawGraph(x, y, handle, TRUE);
	}
#if _DEBUG
	// デバッグ描画
	DebugRender();
#endif
}
/*
 *	@brief	押された瞬間
 */
void LongPressButton::OnPressDown() {
	if (UpdateSelectButton) UpdateSelectButton();

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
	ResetState();
}