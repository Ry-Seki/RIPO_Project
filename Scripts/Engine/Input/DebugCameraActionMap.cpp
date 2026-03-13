/*
 *	@file	DebugCameraActionMap.cpp
 *	@author Riku
 */

#include "DebugCameraActionMap.h"
#include "InputManager.h"

/*
 *	初期化処理
 */
void DebugCameraActionMap::Initialize() {
	// 入力設定
	// 前移動
	AddAxisButton(
		static_cast<int>(GameEnum::DebugCameraAction::ForwardMove),
		{ InputType::Key, KEY_INPUT_W },
		{ InputType::Key, KEY_INPUT_S }
	);
	// 右移動
	AddAxisButton(
		static_cast<int>(GameEnum::DebugCameraAction::RightMove),
		{ InputType::Key, KEY_INPUT_D },
		{ InputType::Key, KEY_INPUT_A }
	);
	// 上移動
	AddAxisButton(
		static_cast<int>(GameEnum::DebugCameraAction::UpMove),
		{ InputType::Key, KEY_INPUT_SPACE },
		{ InputType::Key, KEY_INPUT_LCONTROL }
	);
	// ダッシュ
	AddButton(
		static_cast<int>(GameEnum::DebugCameraAction::Run),
		{ InputType::Key, KEY_INPUT_LSHIFT }
	);
	// カメラのX軸回転
	AddAxisValue(
		static_cast<int>(GameEnum::DebugCameraAction::RotateX),
		{ InputType::MouseMove, static_cast<int>(MouseMove::X) }
	);
	// カメラのY軸回転
	AddAxisValue(
		static_cast<int>(GameEnum::DebugCameraAction::RotateY),
		{ InputType::MouseMove, static_cast<int>(MouseMove::Y) }
	);
	// プレイヤー操作に切り替え
	AddButton(
		static_cast<int>(GameEnum::DebugCameraAction::ChangePlayerAction),
		{ InputType::Key, KEY_INPUT_RCONTROL }
	);

	// 値の初期化
	state.axis[static_cast<int>(GameEnum::DebugCameraAction::ForwardMove)] = 0.0f;
	state.axis[static_cast<int>(GameEnum::DebugCameraAction::RightMove)] = 0.0f;
	state.axis[static_cast<int>(GameEnum::DebugCameraAction::UpMove)] = 0.0f;
	state.button[static_cast<int>(GameEnum::DebugCameraAction::Run)] = false;
	state.axis[static_cast<int>(GameEnum::DebugCameraAction::RotateX)] = 0.0f;
	state.axis[static_cast<int>(GameEnum::DebugCameraAction::RotateY)] = 0.0f;
	state.buttonDown[static_cast<int>(GameEnum::DebugCameraAction::ChangePlayerAction)] = false;

	isActive = false;

}
