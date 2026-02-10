/*
 *	@file	DungeonCreater.h
 *	@author	Seki
 */

#ifndef _DUNGEON_CREATER_H_
#define _DUNGEON_CREATER_H_

#include "../../../../../Data/Dungeon/DungeonFloorData.h"
#include "../../../../../Data/Dungeon/DungeonResourceData.h"
#include "../../../../../Data/Dungeon/DungeonProgressData.h"
#include "../../../../../Data/Dungeon/DungeonEntranceData.h"
#include "../../../../GameConst.h"

#include <vector>

/*
 *	@brief	ダンジョン生成クラス
 */
class DungeonCreater {
private:
	int dungeonID = -1;
	FloorData floorData;
	std::vector<EntranceData> entranceDataList;
	std::vector<EntranceData> stairDataList;
	EntranceData goalData;
	DungeonResourceData dungeonResourceData;
	DungeonEntranceData dungeonEntranceData;

private:

public:
	/*
	 *	@brief	初期化処理
	 */
	void Initialize();
	/*
	 *	@brief	準備前処理
	 */
	void Setup();
	/*
	 *	@brief		ダンジョン生成
	 *	@param[in]	int floorID
	 *  @param[in]	const std::vector<std::vector<int>>& treasureIDList
	 *	@param[in]	int dungeon
	 */
	void GenerateDungeon(int floorID, const std::vector<std::vector<int>>& treasureIDList, int dungeon);
	/*
	 *	@brief		ダンジョンの再生成
	 *	@param[in]	int floorID
	 *  @param[in]	const std::vector<int>& enemySpawnIDList
	 *	@param[in]	const int holdTreasureID
	 *  @param[in]	GameObjectPtr& holdTreasure
	 *  @param[in]	const std::vector<std::vector<int>>& treasureIDList
	 */
	void RegenerateDungeon(int floorID, const std::vector<int>& enemySpawnIDList, const int holdTreasureID,
						   const std::vector<std::vector<int>>& treasureIDList);

private:
	/*
	 *	@brief	階段、出口のデータの設定
	 */
	void SeparateEntranceData();

public:
	/*
	 *	@brief		ダンジョンに必要なデータの設定
	 *	@param[in]	FloorData& setFloorData
	 *	@param[in]	DungeonResourceData& setResourceData
	 *	@param[in]	
	 */
	inline void SetDungeonData(FloorData& setFloorData,
							   DungeonResourceData& setResourceData,
							   DungeonEntranceData& setEntranceData) {
		floorData = setFloorData;
		dungeonResourceData = setResourceData;
		dungeonEntranceData = setEntranceData;
	}
	/*
	 *	@brief		ダンジョンフロアデータの設定
	 *	@param[in]	FloorData& setFloorData
	 */
	inline void SetFloorData(FloorData& setFloorData) { floorData = setFloorData; }
};

#endif // !_DUNGEON_CREATER_H_