/*
 *	@file	PlayerActionMap.cpp
 *  @author Riku
 */

#include "PlayerActionMap.h"
#include "InputManager.h"

/*
 *	初期化処理
 */
void PlayerActionMap::Initialize() {
	// 入力設定
	// 前移動
	AddAxisButton(
		static_cast<int>(GameEnum::PlayerAction::ForwardMove),
		{ InputType::Key, KEY_INPUT_W },
		{ InputType::Key, KEY_INPUT_S }
	);
	// 右移動
	AddAxisButton(
		static_cast<int>(GameEnum::PlayerAction::RightMove),
		{ InputType::Key, KEY_INPUT_D },
		{ InputType::Key, KEY_INPUT_A }
	);
	// ダッシュ
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Run),
		{ InputType::Key, KEY_INPUT_LSHIFT }
	);
	// 回避
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Avoid),
		{ InputType::Key, KEY_INPUT_LCONTROL }
	);
	// ジャンプ
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Jump),
		{ InputType::Key, KEY_INPUT_SPACE }
	);
	// 射撃
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Shot),
		{ InputType::MouseButton, MOUSE_INPUT_LEFT }
	);
	// お宝持ち上げ
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Lift),
		{ InputType::MouseButton, MOUSE_INPUT_RIGHT }
	);
	// メニューを開く
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::OpenMenu),
		{ InputType::Key, KEY_INPUT_ESCAPE }
	);
	// 銃のリロード
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::BulletReload),
		{ InputType::Key, KEY_INPUT_R }
	);
	// 武器1
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::FirstWeapon),
		{ InputType::Key, KEY_INPUT_1 }
	);
	// 武器2
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::SecondWeapon),
		{ InputType::Key, KEY_INPUT_2 }
	);
	// カメラのX軸回転
	AddAxisValue(
		static_cast<int>(GameEnum::PlayerAction::CameraRotateX),
		{ InputType::MouseMove, static_cast<int>(MouseMove::X) }
	);
	// カメラのY軸回転
	AddAxisValue(
		static_cast<int>(GameEnum::PlayerAction::CameraRotateY),
		{ InputType::MouseMove, static_cast<int>(MouseMove::Y) }
	);
	// デバックカメラに切り替え
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::ChangeDebugCamera),
		{ InputType::Key, KEY_INPUT_RCONTROL }
	);
	

	// 値の初期化
	state.axis[static_cast<int>(GameEnum::PlayerAction::ForwardMove)] = 0.0f;
	state.axis[static_cast<int>(GameEnum::PlayerAction::RightMove)] = 0.0f;
	state.button[static_cast<int>(GameEnum::PlayerAction::Run)] = false;
	state.button[static_cast<int>(GameEnum::PlayerAction::Avoid)] = false;
	state.button[static_cast<int>(GameEnum::PlayerAction::Jump)] = false;
	state.button[static_cast<int>(GameEnum::PlayerAction::Shot)] = false;
	state.buttonDown[static_cast<int>(GameEnum::PlayerAction::Shot)] = false;
	state.buttonDown[static_cast<int>(GameEnum::PlayerAction::Lift)] = false;
	state.button[static_cast<int>(GameEnum::PlayerAction::OpenMenu)] = false;
	state.buttonDown[static_cast<int>(GameEnum::PlayerAction::BulletReload)] = false;
	state.buttonDown[static_cast<int>(GameEnum::PlayerAction::FirstWeapon)] = false;
	state.buttonDown[static_cast<int>(GameEnum::PlayerAction::SecondWeapon)] = false;
	state.axis[static_cast<int>(GameEnum::PlayerAction::CameraRotateX)] = 0.0f;
	state.axis[static_cast<int>(GameEnum::PlayerAction::CameraRotateY)] = 0.0f;
	state.buttonDown[static_cast<int>(GameEnum::PlayerAction::ChangeDebugCamera)] = false;

	isActive = false;
}
