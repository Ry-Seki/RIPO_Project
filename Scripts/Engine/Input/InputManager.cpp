#include "InputManager.h"

/*
 *	コンストラクタ
 */
InputManager::InputManager()
	:keyState(), prevKeyState(), anyKeyState(0), mouseInput(), nowMouseInput(), prevInput() {
	Initialize();
}
/*
 *	デストラクタ
 */
InputManager::~InputManager() {

}
/*
 *	初期化処理
 */
void InputManager::Initialize() {
	mouseInput[static_cast<int>(MouseInput::LeftClick)] = MOUSE_INPUT_LEFT;
	mouseInput[static_cast<int>(MouseInput::RightClick)] = MOUSE_INPUT_RIGHT;
	mouseInput[static_cast<int>(MouseInput::MiddleClick)] = MOUSE_INPUT_MIDDLE;
}
/*
 *	更新処理
 */
void InputManager::Update() {
	//1フレーム前のマウスのボタンの状態を保存
	for (int i = 0; i < static_cast<int>(MouseInput::MouseInputMax); i++) {
		prevInput[i] = nowMouseInput[i];
	}
	//1フレーム前のキーの状態を保存
	memcpy_s(prevKeyState, 256, keyState, 256);
	//今のフレームのキー状態を取得
	GetHitKeyStateAll(keyState);
	//現在のマウスのボタンの状態を取得
	for (int i = 0; i < static_cast<int>(MouseInput::MouseInputMax); i++) {
		nowMouseInput[i] = GetMouseInput() & mouseInput[i];
	}
}
