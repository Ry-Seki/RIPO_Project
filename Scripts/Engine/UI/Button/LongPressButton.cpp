/*
 *	@file	LongPressButton.cpp
 *	@author	Seki
 */

#include "LongPressButton.h"
#include "../../Input/InputUtility.h"

#include <DxLib.h>

/*
 *	@brief	‰Šú‰»ˆ—
 */
void LongPressButton::Initialize() {
}
/*
 *	@brief	€”õ‘Oˆ—
 */
void LongPressButton::Setup() {
	UIButtonBase::Setup();
	pressTime = 0.0f;
	holdDuration = 1.0f;
	isHoldCompleted = false;
}
/*
 *	@brief	XVˆ—
 */
void LongPressButton::Update(float unscaledDeltaTime) {
	auto input = InputUtility::GetInputState(GameEnum::ActionMap::MenuAction);

	UIButtonBase::Update(unscaledDeltaTime);

	if (inputState == GameEnum::ButtonInputState::Hover &&
		input.buttonDown[static_cast<int>(GameEnum::MenuAction::Click)]) {
		OnPressDown();
	}

	if (inputState == GameEnum::ButtonInputState::Press) {
		pressTime += unscaledDeltaTime;
		OnPress(unscaledDeltaTime);

		if(input.buttonUp[static_cast<int>(GameEnum::MenuAction::Click)]) OnPressUp();
	}	
}
/*
 *	@brief	•`‰æˆ—
 */
void LongPressButton::Render() {
}
/*
 *	@brief	‰Ÿ‚³‚ê‚½uŠÔ
 */
void LongPressButton::OnPressDown() {
	pressTime = 0.0f;
	isHoldCompleted = false;
	inputState = GameEnum::ButtonInputState::Press;
}
/*
 *	@brief	‰Ÿ‰º’†
 */
void LongPressButton::OnPress(float unscaledDeltaTime) {
	if (!isHoldCompleted && pressTime >= holdDuration) {
		isHoldCompleted = true;
		OnClick();
	}
}
/*
 *	@brief	—£‚ê‚½uŠÔ
 */
void LongPressButton::OnPressUp() {
	inputState = GameEnum::ButtonInputState::Idle;
	selectState = GameEnum::ButtonSelectState::Select;
}