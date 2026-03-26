/*
 *	@file	InputManager.cpp
 *	@author	Riku
 */

#include "InputManager.h"
#include "PlayerActionMap.h"
#include "MenuActionMap.h"
#include "DebugCameraActionMap.h"
#include "../GameConst.h"

/*
 *	コンストラクタ
 */
InputManager::InputManager()
	: keyState()
	, prevKeyState()
	, nowMouseInput()
	, prevMouseInput()
	, nowMousePosX()
	, prevMousePosX()
	, nowMousePosY()
	, prevMousePosY()
	, mouseVisible(true)
	, prevInputMouse(false){
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
	// 全ての入力受付の生成
	actionMaps[GameEnum::ActionMap::PlayerAction] = std::make_shared<PlayerActionMap>();
	actionMaps[GameEnum::ActionMap::MenuAction] = std::make_shared<MenuActionMap>();
	actionMaps[GameEnum::ActionMap::DebugCameraAction] = std::make_shared<DebugCameraActionMap>();
	for (auto [mapNum, actionMap] : actionMaps) {
		// 全アクションマップの初期化
		actionMap->Initialize();
	}
}
/*
 *	更新処理
 */
void InputManager::Update() {
	// マウスの表示非表示切り替え
	SetMouseDispFlag(mouseVisible);

	//1フレーム前のマウスのボタンの状態を保存
	prevMouseInput = nowMouseInput;
	//現在のマウスのボタンの状態を取得
	nowMouseInput = GetMouseInput();

	//1フレーム前のキーの状態を保存
	memcpy_s(prevKeyState, 256, keyState, 256);
	//今のフレームのキー状態を取得
	GetHitKeyStateAll(keyState);

	// 1フレーム前のマウスの位置を保存
	prevMousePosX = nowMousePosX;
	prevMousePosY = nowMousePosY;
	// 現在のマウスの位置を保存
	GetMousePoint(&nowMousePosX, &nowMousePosY);
	// マウスカーソルが非表示ならカーソルは中央固定
	if (!mouseVisible) {
		// 1フレーム目はスキップ
		if (mouseMoveSkip) {
			mouseMoveSkip = false;
		}
		else {
			// 画面中央
			int windowWidthCenter = GameConst::WINDOW_WIDTH / 2;
			int windowHeightCenter = GameConst::WINDOW_HEIGHT / 2;
			// マウスを画面中央に固定
			SetMousePoint(windowWidthCenter, windowHeightCenter);
			// 1フレーム前位置は画面中央とする
			prevMousePosX = windowWidthCenter;
			prevMousePosY = windowHeightCenter;
		}
	}

	// 直前の入力がマウスかどうか管理
	for (auto key : keyState) {
		if (key == 1)
			prevInputMouse = false;
	}
	if (nowMouseInput != 0 || 
		prevMousePosX - nowMousePosX != 0 || 
		prevMousePosY - nowMousePosY != 0) {
		prevInputMouse = true;
	}

	// 各アクションマップの更新処理
	for (auto [mapNum, actionMap] : actionMaps) {
		// まず入力状態のリセット
		actionMap->InputReset();
		// 有効なら更新処理
		if (actionMap->isActive)
			actionMap->InputUpdate();
	}
}