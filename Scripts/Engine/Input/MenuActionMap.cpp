/*
 *	@file	MenuActionMap.cpp
 *	@author	Seki
 */

#include "MenuActionMap.h"
#include "../GameEnum.h"

#include <DxLib.h>

/*
 *	@brief	初期化処理
 */
void MenuActionMap::Initialize() {
	// 入力設定
	// 上下移動
	AddAxis(
		static_cast<int>(GameEnum::MenuAction::Vertical),
		{ InputType::Key, KEY_INPUT_UP },
		{ InputType::Key, KEY_INPUT_DOWN }
	);
	AddAxis(
		static_cast<int>(GameEnum::MenuAction::Vertical),
		{ InputType::Key, KEY_INPUT_W },
		{ InputType::Key, KEY_INPUT_S }
	);
	// 左右移動
	AddAxis(
		static_cast<int>(GameEnum::MenuAction::Horizontal),
		{ InputType::Key, KEY_INPUT_RIGHT },
		{ InputType::Key, KEY_INPUT_LEFT }
	);
	AddAxis(
		static_cast<int>(GameEnum::MenuAction::Horizontal),
		{ InputType::Key, KEY_INPUT_D },
		{ InputType::Key, KEY_INPUT_A }
	);
	// 決定
	AddButton(
		static_cast<int>(GameEnum::MenuAction::Decide),
		{ InputType::Key, KEY_INPUT_SPACE }
	);
	AddButton(
		static_cast<int>(GameEnum::MenuAction::Decide),
		{ InputType::Key, KEY_INPUT_RETURN }
	);
	// クリック
	AddButton(
		static_cast<int>(GameEnum::MenuAction::Click),
		{ InputType::Mouse, MOUSE_INPUT_LEFT }
	);
	// キャンセル
	AddButton(
		static_cast<int>(GameEnum::MenuAction::Cancel),
		{ InputType::Key, KEY_INPUT_ESCAPE }
	);

	// 値の初期化
	state.axis[static_cast<int>(GameEnum::MenuAction::Vertical)] = 0.0f;
	state.axis[static_cast<int>(GameEnum::MenuAction::Horizontal)] = 0.0f;
	state.buttonDown[static_cast<int>(GameEnum::MenuAction::Decide)] = false;
	state.buttonUp[static_cast<int>(GameEnum::MenuAction::Decide)] = false;
	state.buttonDown[static_cast<int>(GameEnum::MenuAction::Click)] = false;
	state.buttonUp[static_cast<int>(GameEnum::MenuAction::Click)] = false;
	state.buttonDown[static_cast<int>(GameEnum::MenuAction::Cancel)] = false;
	state.buttonUp[static_cast<int>(GameEnum::MenuAction::Cancel)] = false;

	isActive = false;
}
