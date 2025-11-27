/*
 *	@file	DungeonCreater.h
 *	@author	Seki
 */

#ifndef _DUNGEON_CREATER_H_
#define _DUNGEON_CREATER_H_

#include "../../../../Data/Dungeon/DungeonFloorData.h"
#include "../../../../Data/Dungeon/DungeonResourceData.h"

/*
 *	@brief	ダンジョン生成クラス
 */
class DungeonCreater {
private:
	bool isFirst = true;

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
	void CreateDungeon(
		int floorID, DungeonFloorData& floorData, DungeonResourceData& resourceData, const std::vector<int>& treasureIDList, int& stairID);
	
};

#endif // !_DUNGEON_CREATER_H_