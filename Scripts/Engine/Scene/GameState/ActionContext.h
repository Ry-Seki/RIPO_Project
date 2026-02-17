/*
 *	@file	ActionContext.h
 *	@author	Seki
 */

#ifndef _ACTION_CONTEXT_H_
#define _ACTION_CONTEXT_H_

#include "../../GameEnum.h"
#include "../../../Data/Dungeon/DungeonStageData.h"
#include "../../../Data/Dungeon/DungeonFloorData.h"

#include <vector>

// 前方宣言
struct GameProgressData;

/*
 *	@brief	ステート間で受け渡されるデータ
 */
struct ActionContext {
	int elapsedDay = 0;						// 現在の経過日数
	int dungeonID = 0;						// ダンジョンID
	int prevIncome = 0;						// 前回の稼ぎ
	bool isPlayerDead = false;				// プレイヤーの死亡判定
	bool isHalf = false;					// 半日かどうか
	bool isCurrentEvent = false;			// イベント中かどうか				
	bool isClear = false;					// ゲームクリアフラグ

	DungeonStageData dungeonStageData;			// ダンジョンステージデータ
	DungeonFloorData dungeonFloorData;			// ダンジョンフロアデータ
	std::vector<int> buyStatusIDList;			// 購入したステータスアイテムIDリスト
	std::vector<int> buyWeaponIDList;			// 購入した武器IDリスト
	GameEnum::ActionType actionType = GameEnum::ActionType::Invalid;				// アクションタイプ
	GameEnum::PlayerStatusType statusType = GameEnum::PlayerStatusType::Invalid;	// ステータスタイプ
	GameEnum::MiniGameLevel miniGameLevel = GameEnum::MiniGameLevel::Invalid;		// ミニゲームの難易度

	/*
	 *	@biref		セーブデータに情報を渡す
	 *	@return		GameProgressData
	 */
	GameProgressData GetSaveData() const;
	/*
	 *	@brief		セーブデータから情報を設定
	 *  @param[in]	const GameProgressData& data
	 */
	void ApplyLoadData(const GameProgressData& data);
};

#endif // !_ACTION_CONTEXT_H_