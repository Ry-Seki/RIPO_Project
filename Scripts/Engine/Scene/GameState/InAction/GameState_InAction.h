/*
 *	@file	GameState_InAction.h
 *	@author	Seki
 */

#ifndef _GAME_STATE_IN_ACTION_H_
#define _GAME_STATE_IN_ACTION_H_

#include "../GameStateBase.h"

#include <memory>
#include <vector>

// 前方宣言
class Engine;
class InActionBase;

/*
 *	@brief	行動実行状態
 */
class GameState_InAction : public GameStateBase {
public:		// 別名定義
	using InActionPtr = std::shared_ptr<InActionBase>;

private:
	std::vector<InActionPtr> inActionList;
	InActionPtr currentAction;

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
	 *	@brief	描画処理(デバッグ用)
	 */
	void Render() override;
	/*
	 *	@brief	片付け処理
	 */
	void Teardown() override;

public:
	/*
	 *	@brief	ロード済みデータのセット(コールバック)
	 */
	void SetupData();

};
#endif // !_GAME_STATE_IN_ACTION_H_