#include "InputManager.h"
#include "PlayerActionMap.h"

/*
 *	コンストラクタ
 */
InputManager::InputManager()
	: keyState()
	, prevKeyState()
	, nowMouseInput()
	, prevMouseInput() {
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
	actionMaps[GameEnum::ActionMap::PlayerAction] = std::make_shared<PlayerActionMap>();
	for (auto [mapNum, actionMap] : actionMaps) {
		// 全アクションマップの初期化
		actionMap->Initialize();
	}
}
/*
 *	更新処理
 */
void InputManager::Update() {
	//1フレーム前のマウスのボタンの状態を保存
	prevMouseInput = nowMouseInput;
	//現在のマウスのボタンの状態を取得
	nowMouseInput = GetMouseInput();

	//1フレーム前のキーの状態を保存
	memcpy_s(prevKeyState, 256, keyState, 256);
	//今のフレームのキー状態を取得
	GetHitKeyStateAll(keyState);

	// 各アクションマップの更新処理
	for (auto [mapNum, actionMap] : actionMaps) {
		// まず入力状態のリセット
		actionMap->InputReset();
		// 有効なら更新処理
		if (actionMap->isActive)
			actionMap->InputUpdate();
	}
}