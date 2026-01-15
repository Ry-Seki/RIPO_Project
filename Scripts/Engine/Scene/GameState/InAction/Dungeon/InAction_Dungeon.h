/*
 *	@file	InAction_Dungeon.h
 *	@author	Seki
 */

#ifndef _IN_ACTION_DUNGEON_H_
#define _IN_ACTION_DUNGEON_H_

#include "../InActionBase.h"
#include "../../../DayAction/ActionDungeon/DungeonCreater.h"


// 前方宣言
class DungeonResourceData;

/*
 *	@brief	ダンジョン行動実行クラス
 */
class InAction_Dungeon : public InActionBase {
private:
	bool isStart = false;
	int currentFloor = -1;
	int nextFloor = -1;

	DungeonCreater dungeonCreater;
	DungeonResourceData resourceData;
	DungeonFloorData floorData;
	DungeonStageData stageData;

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
	 *	@brief	ダンジョン準備処理
	 */
	void SetupDungeon();
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

public:
	/*
	 *	@brief	お宝データからお宝IDを取り出す
	 */
	std::vector<int> TakeOutTreasureID();
	/*
	 *	@brief	ダンジョン終了処理
	 */
	void EndDungeon();
};

#endif // !_IN_ACTION_DUNGEON_H_