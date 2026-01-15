/*
 *	@file	GameStateMachine.cpp
 *	@author	Seki
 */

#include "GameStateMachine.h"
#include "InitialTutorial/GameState_InitialTutorial.h"
#include "SelectAction/GameState_SelectAction.h"
#include "SelectDetail/GameState_SelectDetail.h"
#include "InAction/GameState_InAction.h"
#include "ResultAction/GameState_ResultAction.h"
#include "Pause/GameState_Pause.h"

/*
 *	@brief	初期化処理
 */
void GameStateMachine::Initialize(Engine& engine) {
	// ゲームステートの要素分、あらかじめ確保しておく
    stateList.resize(static_cast<int>(GameEnum::GameState::Max));
    // 各ステートの生成
    stateList[static_cast<int>(GameEnum::GameState::InitialTutorial)]
        = std::make_shared<GameState_InitialTutorial>();
    stateList[static_cast<int>(GameEnum::GameState::SelectAction)]
        = std::make_shared<GameState_SelectAction>();
    stateList[static_cast<int>(GameEnum::GameState::SelectDetail)]
        = std::make_shared<GameState_SelectDetail>();
    stateList[static_cast<int>(GameEnum::GameState::InAction)]
        = std::make_shared<GameState_InAction>();
    stateList[static_cast<int>(GameEnum::GameState::ResultAction)]
        = std::make_shared<GameState_ResultAction>();
    stateList[static_cast<int>(GameEnum::GameState::Pause)]
        = std::make_shared<GameState_Pause>();
    // 各ステートの初期化
    for (auto& state : stateList) {
        if (!state) continue;

        state->Initialize(engine);
        state->SetOwner(this);
    }
}
/*
 *  @brief  準備前処理
 */
void GameStateMachine::Setup() {

}
/*
 *	@brief	更新処理
 */
void GameStateMachine::Update(float deltaTime) {
    if (currentState) currentState->Update(deltaTime);
}
/*
 *	@brief	描画処理
 */
void GameStateMachine::Render() {
    if (currentState) currentState->Render();
}
/*
 *  @brief  片付け処理
 */
void GameStateMachine::Teardown() {
    if (currentState) currentState->Teardown();
    if (pauseState) pauseState->Teardown();
    currentState.reset();
    pauseState.reset();
}
/*
 *  @breif  現在のステートの片付け処理
 */
void GameStateMachine::TeardownCurrentState() {
    if (!currentState) return;

    currentState->Teardown();
    currentState = nullptr;
}
/*
 *	@brief		ステートの変更
 *	@param[in]	GameState type
 */
void GameStateMachine::ChageState(GameEnum::GameState type) {
    if (stateType == type) return;

    // ポーズからの復帰の場合はポーズステートからステートを取得
    if (stateType == GameEnum::GameState::Pause) {
        if (!pauseState) return;
        // 現在のステートの片づけ処理
        TeardownCurrentState();
        // ステートの変更
        stateType = type;
        currentState = pauseState;
        pauseState = nullptr;
        // ステートの再開処理
        currentState->Resume();
        return;
    }
    // 指定ステートがポーズかそれ以外で処理を変える
    if (type == GameEnum::GameState::Pause) {
        // 現在のステートのポーズ処理
        currentState->Pause();
        // ポーズステートに移動
        pauseState = currentState;
    } else {
        // 現在のステートの片付け処理
        TeardownCurrentState();
    }
    // ステートの変更
    stateType = type;
    currentState = stateList[static_cast<int>(type)];
    // 現在のステートの準備前処理
    currentState->Setup();
}
