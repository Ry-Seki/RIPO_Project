/*
 *	@file	ActionContext.h
 *	@author	Seki
 */

#ifndef _ACTION_CONTEXT_H_
#define _ACTION_CONTEXT_H_

#include "../../GameEnum.h"
#include "../../../Data/Dungeon/DungeonStageData.h"
#include "../../../Data/Dungeon/DungeonFloorData.h"

/*
 *	@brief	ステート間で受け渡されるデータ
 */
struct ActionContext {
	GameEnum::ActionType actionType = GameEnum::ActionType::Invalid;

	int currentDay = -1;						// 現在の経過日数
	int dungeonID = -1;							// ダンジョンID

	DungeonStageData dungeonStageData;			// ダンジョンステージデータ
	DungeonFloorData dungeonFloorData;			// ダンジョンフロアデータ

};

#endif // !_ACTION_CONTEXT_H_