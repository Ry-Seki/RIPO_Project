/*
 *	@file	DungeonCreater.h
 *	@author	Seki
 */

#ifndef _DUNGEON_CREATER_H_
#define _DUNGEON_CREATER_H_

#include "../../../../Data/Dungeon/DungeonFloorData.h"
#include "../../../../Data/Dungeon/DungeonResourceData.h"
#include "../../../GameObject.h"

/*
 *	@brief	ダンジョン生成クラス
 */
class DungeonCreater {
private:
	FloorData d_floorData;
	DungeonResourceData d_resourceData;

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
	 *  @param[in]	DungeonFloorData& floorData
	 *  @param[in]	DungeonResourceData& resourceData
	 *  @param[in]	const std::vector<int>& treasureIDList
	 *  @param[out]	int& stairID
	 */
	void GenerateDungeon(
		int floorID, DungeonFloorData& floorData, DungeonResourceData& resourceData, const std::vector<int>& treasureIDList, int& stairID);
	/*
	 *	@brief		ダンジョンの再生成
	 *	@param[in]	int floorID
	 *	@param[in]	const int holdTreasureID
	 *  @param[in]	GameObjectPtr& holdTreasure
	 *  @param[in]	const std::vector<int>& treasureIDList
	 *  @param[out]	int& stairID
	 */
	void RegenerateDungeon(int floorID, GameObjectList& setEnemyList, const int holdTreasureID,
						   const std::vector<int>& treasureIDList, int& stairID);
};

#endif // !_DUNGEON_CREATER_H_