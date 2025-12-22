/*
 *	@file	GameStateMachine.h
 *	@author	Seki
 */

#ifndef _GAME_STATE_MACHINE_H_
#define _GAME_STATE_MACHINE_H_

#include "../../GameEnum.h"

#include <vector>
#include <memory>

// 前方宣言
class GameStateBase;

/*
 *	@brief	ゲームステート制御クラス
 */
class GameStateMachine {
public:		// 別名定義
	using GameStatePtr = std::shared_ptr<GameStateBase>;

private:
	std::vector<GameStatePtr> stateList;	// ステートリスト
	GameEnum::GameState stateType;			// ステートの種類
	GameStatePtr currentState;				// 現在のステート
	GameStatePtr prevState;					// ひとつ前のステート(Pause用)

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize();
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime);
	/*
	 *	@brief	描画処理
	 */
	void Render();

public:
	/*
	 *	@brief		ステートの変更
	 *	@param[in]	GameState type
	 */
	void ChageState(GameEnum::GameState type);

};
#endif // !_GAME_STATE_MACHINE_H_