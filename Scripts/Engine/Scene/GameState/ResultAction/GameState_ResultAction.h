/*
 *	@file	GameState_ResultAction.h
 *	@author	Seki
 */

#ifndef _GAME_STATE_RESULT_ACTION_H_
#define _GAME_STATE_RESULT_ACTION_H_

#include "../GameStateBase.h"
#include "../../../GameEnum.h"

#include <vector>
#include <memory>

// 前方宣言
class Engine;
class ResultActionBase;

/*
 *	@brief	各アクションの結果反映ステート
 */
class GameState_ResultAction : public GameStateBase{
public:		// 別名定義
	using ResultActionPtr = std::shared_ptr<ResultActionBase>;

private:
	std::vector<ResultActionPtr> resultActionList;	// アクション結果リスト
	ResultActionPtr currentResult;					// 現在のアクション結果

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize(Engine& engine) override;
	/*
	 *	@brief	準備前処理
	 */
	void Setup() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

};

#endif // !_GAME_STATE_RESULT_ACTION_H_