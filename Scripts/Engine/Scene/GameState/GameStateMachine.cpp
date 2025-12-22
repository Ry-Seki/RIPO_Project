/*
 *	@file	GameStateMachine.cpp
 *	@author	Seki
 */

#include "GameStateMachine.h"

/*
 *	@brief	初期化処理
 */
void GameStateMachine::Initialize() {
	// ゲームステートの要素分、あらかじめ確保しておく
    int maxCount = static_cast<int>(GameEnum::GameState::Max);
    // 各ステートの生成

}
/*
 *	@brief	更新処理
 */
void GameStateMachine::Update(float deltaTime) {
}
/*
 *	@brief	描画処理
 */
void GameStateMachine::Render() {
}
/*
 *	@brief		ステートの変更
 *	@param[in]	GameState type
 */
void GameStateMachine::ChageState(GameEnum::GameState type) {
    if (stateType == GameEnum::GameState::Pause) {
        // Pauseからの復帰ではprevStateを更新しない

    } else {
        prevState = currentState;
    }
    stateType = type;
}
