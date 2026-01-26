/*
 *	@file	DungeonProgressData.h
 *	@author	Seki
 */

#ifndef _DUNGEON_PROGRESS_DATA_H_
#define _DUNGEON_PROGRESS_DATA_H_

#include <unordered_map>

/*
 *	@brief	ダンジョンの進行状況
 */
struct  DungeonProgressData {
	int dungeonID = 0;

	int treasureCount = 0;									// 残りのお宝の数
	bool isBossDefeated = false;							// ボス討伐フラグ

	std::unordered_map<int, bool> treasureFlagMap;			// 通常お宝
	std::unordered_map<int, bool> eventTreasureFlagMap;		// イベントお宝（スタンプ）
};

#endif // !_DUNGEON_PROGRESS_DATA_H_