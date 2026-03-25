/*
 *	@file	SinglePressButton.cpp
 *	@author	Seki
 */

#include "SinglePressButton.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

 /*
  *	@brief	ボタンの状態のリセット
  */
void SinglePressButton::ResetState() {
	inputHandle = false;
	UIButtonBase::ResetState();
}

/*
 *	@brief	初期化処理
 */
void SinglePressButton::Initialize() {
	UIButtonBase::Initialize();
}
/*
 *	@brief	準備前処理
 */
void SinglePressButton::Setup() {
	UIButtonBase::Setup();
	inputHandle = false;
}
/*
 *	@brief	更新処理
 */
void SinglePressButton::Update(float unscaledDeltaTime) {
    auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

    UIButtonBase::Update(unscaledDeltaTime);

	// マウスHover時を優先する
	if (!inputHandle && inputState == GameEnum::ButtonInputState::Hover) {
		if (UpdateSelectButton) UpdateSelectButton();
		// クリック判定
		if (input.buttonDown[inputClickNum]) {
			inputHandle = true;
			OnPressDown();
		}
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
		DrawGraph(x, y, handle, TRUE);
	}
#if _DEBUG
	// デバッグ描画
	DebugRender();
#endif
}
/*
 *	@brief	押した瞬間
 */
void SinglePressButton::OnPressDown() {
	if (UpdateSelectButton) UpdateSelectButton();

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

	OnClickEvent();
}