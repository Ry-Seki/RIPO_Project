/*
 *	@file	FloorProcessor.h
 *	@author	Seki
 */

#ifndef _FLOOR_PROCESSOR_H_
#define _FLOOR_PROCESSOR_H_

#include "DungeonCreater.h"

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
	int holdTreasureObjectID = -1;					// プレイヤーが所持しているお宝のオブジェクトID
	int holdTreasureID = -1;						// プレイヤーが所持しているお宝ID
	DungeonCreater dungeonCreater;					// ダンジョン生成クラス
	DungeonResourceData resourceData;				// ダンジョンリソースデータ
	DungeonFloorData floorData;						// ダンジョンフロアデータ
	DungeonStageData stageData;						// ダンジョンステージデータ
	DungeonProgressData dungeonProgressData;		// ダンジョン進捗度データ

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
	 *	@brief		フロア関係なしのお宝ID一覧を取得
	 *  @return		std::vector<int>
	 */
	std::vector<int> GetAllTreasureIDTable();
	/*
	 *	@brief		フロア関係なしの生成するお宝ID一覧の取得
	 *  @return		std::vector<int>
	 */
	std::vector<int> GetAllSpawnTreasureIDTable();
	/*
	 *	@brief		フロアごとのお宝ID一覧を取得
	 *	@return		std::vector<int>
	 */
	std::vector<int> GetTreasureIDTable();
	/*
	 *	@brief		フロアごとに生成するお宝IDのみを渡す
	 *	@return		std::vector<int>
	 */
	std::vector<int> GetSpawnTreasureIDTable();

public:
	/*
	 *	@brief		プレイヤーが所持しているお宝IDの取得
	 *	@return		int
	 */
	int GetHoldTreasureID() const;
};

#endif // !_FLOOR_PROCESSOR_H_