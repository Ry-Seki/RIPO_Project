/*
 *	@file	DungeonCreater.h
 *	@author	Seki
 */

#ifndef _DUNGEON_CREATER_H_
#define _DUNGEON_CREATER_H_

#include "../../../../../Data/Dungeon/DungeonFloorData.h"
#include "../../../../../Data/Dungeon/DungeonResourceData.h"

/*
 *	@brief	ダンジョン生成クラス
 */
class DungeonCreater {
private:
	FloorData floorData;
	DungeonResourceData resourceData;

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
	 *  @param[in]	const std::vector<int>& treasureIDList
	 *  @param[out]	int& stairID
	 */
	void GenerateDungeon(int floorID, const std::vector<int>& treasureIDList, int& stairID);
	/*
	 *	@brief		ダンジョンの再生成
	 *	@param[in]	int floorID
	 *  @param[in]	const std::vector<int>& enemySpawnIDList
	 *	@param[in]	const int holdTreasureID
	 *  @param[in]	GameObjectPtr& holdTreasure
	 *  @param[in]	const std::vector<int>& treasureIDList
	 *  @param[out]	int& stairID
	 */
	void RegenerateDungeon(int floorID, const std::vector<int>& enemySpawnIDList, const int holdTreasureID,
						   const std::vector<int>& treasureIDList, int& stairID);

public:
	/*
	 *	@brief		ダンジョンに必要なデータの設定
	 *	@param[in]	FloorData& setFloorData
	 *	@param[in]	DungeonResourceData& setResourceData
	 */
	inline void SetDungeonData(FloorData& setFloorData, DungeonResourceData& setResourceData) {
		floorData = setFloorData;
		resourceData = setResourceData;
	}
	/*
	 *	@brief		ダンジョンフロアデータの設定
	 *	@param[in]	FloorData& setFloorData
	 */
	inline void SetFloorData(FloorData& setFloorData) { floorData = setFloorData; }
};

#endif // !_DUNGEON_CREATER_H_