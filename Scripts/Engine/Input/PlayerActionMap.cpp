#include "PlayerActionMap.h"
#include "InputManager.h"

/*
 *	初期化処理
 */
void PlayerActionMap::Initialize() {
	isActive = false;

	// 入力設定
	// 前移動
	AddAxis(
		static_cast<int>(GameEnum::PlayerAction::ForwardMove),
		{ InputType::Key, KEY_INPUT_W },
		{ InputType::Key, KEY_INPUT_S });
	// 右移動
	AddAxis(
		static_cast<int>(GameEnum::PlayerAction::RightMove),
		{ InputType::Key, KEY_INPUT_D },
		{ InputType::Key, KEY_INPUT_A });
	// ダッシュ
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Run),
		{ InputType::Key, KEY_INPUT_LSHIFT });
	// 回避
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Avoid),
		{ InputType::Key, KEY_INPUT_LCONTROL });
	// ジャンプ
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Jump),
		{ InputType::Key, KEY_INPUT_SPACE });
	// 射撃
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Shot),
		{ InputType::Mouse, MOUSE_INPUT_RIGHT });
	// お宝持ち上げ
	AddButton(
		static_cast<int>(GameEnum::PlayerAction::Lift),
		{ InputType::Mouse, MOUSE_INPUT_LEFT });
}
