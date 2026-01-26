/*
 *	@file	InAction_Dungeon.h
 *	@author	Seki
 */

#ifndef _IN_ACTION_DUNGEON_H_
#define _IN_ACTION_DUNGEON_H_

#include "../InActionBase.h"
#include "FloorProcessor.h"

/*
 *	@brief	ダンジョン行動実行クラス
 */
class InAction_Dungeon : public InActionBase {
private:
	FloorProcessor floorProcessor;

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

private:
	/*
	 *	@brief		プレイヤーの死亡判定
	 *	@return		bool
	 */
	bool IsPlayerDead();

public:
	/*
	 *	@brief	ダンジョン終了処理
	 */
	void EndDungeon();
	/*
	 *	@brief		ダンジョン終了集計
	 *  @param[in]	int dungeonID
	 */
	void CalculationDungeon(int dungeonID);
};

#endif // !_IN_ACTION_DUNGEON_H_