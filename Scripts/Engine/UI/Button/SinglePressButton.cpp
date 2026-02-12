/*
 *	@file	SinglePressButton.cpp
 *	@author	Seki
 */

#include "SinglePressButton.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void SinglePressButton::Initialize() {
	UIButtonBase::Initialize();
}
/*
 *	@brief	更新処理
 */
void SinglePressButton::Update(float unscaledDeltaTime) {
    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

    UIButtonBase::Update(unscaledDeltaTime);

    if (inputState == GameEnum::ButtonInputState::Hover 
		&& input.buttonDown[inputClickNum]) {
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
void SinglePressButton::Render() {
	if (handleList.empty()) return;
	// TODO : 描画インターフェースでの描画
	int handle = handleList[static_cast<int>(GetRendererState())];

	if (handle != -1) {
		DrawGraph(rect.x, rect.y, handle, TRUE);
	}
	// デバッグ描画
	DebugRender();
}
/*
 *	@brief	押した瞬間
 */
void SinglePressButton::OnPressDown() {
	inputState = GameEnum::ButtonInputState::Press;
}
/*
 *	@brief	押下中
 */
void SinglePressButton::OnPress(float unscaledDeltaTime) {
	inputState = GameEnum::ButtonInputState::Press;
}
/*
 *	@brief	離した瞬間
 */
void SinglePressButton::OnPressUp() {
	if (inputState != GameEnum::ButtonInputState::Press) return;

	inputState = GameEnum::ButtonInputState::Idle;

	OnClickEvent();
}