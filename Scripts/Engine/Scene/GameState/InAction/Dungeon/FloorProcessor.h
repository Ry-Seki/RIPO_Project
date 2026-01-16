/*
 *	@file	FloorProcessor.h
 *	@author	Seki
 */

#ifndef _FLOOR_PROCESSOR_H_
#define _FLOOR_PROCESSOR_H_

#include "../../../DayAction/ActionDungeon/DungeonCreater.h"

#include <vector>

// 前方宣言
struct ActionContext;

/*
 *	@brief	ダンジョンのフロアを管理するクラス
 */
class FloorProcessor {
private:
	std::vector<std::vector<int>> enemyFloorList;	// フロア内の敵IDリスト
	int currentFloor = -1;							// 現在のフロア
	int nextFloor = -1;								// 移動先のフロア
	int holdTreasureID = -1;						// プレイヤーが所持しているお宝ID
	DungeonCreater dungeonCreater;					// ダンジョン生成クラス
	DungeonResourceData resourceData;				// ダンジョンリソースデータ
	DungeonFloorData floorData;						// ダンジョンフロアデータ
	DungeonStageData stageData;						// ダンジョンステージデータ

private:
	/*
	 *	@brief	現在のフロアの片付け処理
	 */
	void TeardownCurrentFloor();
	/*
	 *	@brief	敵の再生成に必要なデータへ変換
	 */
	void ConvertEnemyData();
	/*
	 *	@brief	敵の片付け処理
	 */
	void TeardownEnemy();
	/*
	 *	@brief	ステージオブジェクトの片付け処理
	 */
	void TeardownStageObject();
	/*
	 *	@brief	次のフロアの準備処理
	 */
	void SetupNextFloor();

public:
	/*
	 *	@brief		フロアの生成
	 *  @param[in]	ActionContext setContext
	 *  @param[in]	bool& isStart
	 */
	void CreateFloor(ActionContext setContext, bool& isStart);
	/*
	 *	@brief	フロアの変更処理
	 */
	void ChangeFloor();
	/*
	 *	@brief	ダンジョン終了処理
	 */
	void EndDungeon();

private:
	/*
	 *	@brief		お宝データからお宝IDを取り出す
	 *  @return		std::vector<int>
	 */
	std::vector<int> GetNameToTreasureID();

};

#endif // !_FLOOR_PROCESSOR_H_