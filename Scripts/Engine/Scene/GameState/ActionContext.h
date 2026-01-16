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
	int currentDay = -1;						// 現在の経過日数
	int dungeonID = -1;							// ダンジョンID

	DungeonStageData dungeonStageData;			// ダンジョンステージデータ
	DungeonFloorData dungeonFloorData;			// ダンジョンフロアデータ

	GameEnum::ActionType actionType = GameEnum::ActionType::Invalid;				// アクションタイプ
	GameEnum::PlayerStatusType statusType = GameEnum::PlayerStatusType::Invalid;	// ステータスタイプ
	GameEnum::MiniGameLevel miniGameLevel = GameEnum::MiniGameLevel::Invalid;		// ミニゲームの難易度
};

#endif // !_ACTION_CONTEXT_H_