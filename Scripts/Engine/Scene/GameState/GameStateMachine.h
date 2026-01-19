/*
 *	@file	GameStateMachine.h
 *	@author	Seki
 */

#ifndef _GAME_STATE_MACHINE_H_
#define _GAME_STATE_MACHINE_H_

#include "../../GameEnum.h"
#include "ActionContext.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;
class GameStateBase;

/*
 *	@brief	ゲームステート制御クラス
 */
class GameStateMachine {
public:		// 別名定義
	using GameStatePtr = std::shared_ptr<GameStateBase>;

private:
	bool isActionEnd = false;											// アクション終了フラグ
	std::vector<GameStatePtr> stateList;								// ステートリスト
	GameEnum::GameState stateType = GameEnum::GameState::Invalid;		// ステートの種類
	ActionContext actionContext;										// ステート間で必要なデータ構造体
	GameStatePtr currentState = nullptr;								// 現在のステート
	GameStatePtr pauseState = nullptr;									// ポーズ用ステート

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine);
	/*
	 *	@brief	準備前処理
	 */
	void Setup();
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime);
	/*
	 *	@brief	描画処理
	 */
	void Render();
	/*
	 *	@brief	片付け処理
	 */
	void Teardown();
	
public:
	/*
	 *	@brief	現在のステートの片付け処理
	 */
	void TeardownCurrentState();

public:
	/*
	 *	@brief		ステートの変更
	 *	@param[in]	GameState type
	 */
	void ChageState(GameEnum::GameState type);

public:
	/*
	 *	@brief		アクション終了判定
	 *	@return		bool
	 */
	inline bool IsActionEnd() const { return isActionEnd; }
	/*
	 *	@brief		アクション終了フラグの変更
	 *	@param[in]	bool setFlag
	 */
	inline void SetIsActionEnd(bool setFlag) { isActionEnd = setFlag; }
	/*
	 *	@brief		アクションデータ構造体の取得
	 *	@return		ActionContext*
	 */
	inline ActionContext& GetActionContext() { return actionContext; }
	/*
	 *	@brief		アクションの設定
	 *	@param[in]	GameEnum::ActionType setType
	 */
	inline void SetActionType(GameEnum::ActionType setType) {
		actionContext.actionType = setType;
	}

};
#endif // !_GAME_STATE_MACHINE_H_