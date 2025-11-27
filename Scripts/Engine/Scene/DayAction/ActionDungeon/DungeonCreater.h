/*
 *	@file	DungeonCreater.h
 *	@author	Seki
 */

#ifndef _DUNGEON_CREATER_H_
#define _DUNGEON_CREATER_H_

#include "../../../../Data/Dungeon/DungeonFloorData.h"

/*
 *	@brief	ダンジョン生成クラス
 */
class DungeonCreater {
	DungeonFloorData floorData;

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
	 *	@brief	ダンジョン生成
	 */
	void CreateDungeon();
	
};

#endif // !_DUNGEON_CREATER_H_