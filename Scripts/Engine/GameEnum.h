/*
 *	@file	GameEnum.h
 *	@author	Seki
 */

#ifndef _GAME_ENUM_H_
#define _GAME_ENUM_H_

namespace GameEnum {
	/*
	 *	@brief	メインゲーム状態
	 */
	enum class GameState {
		Invalid = -1,
		InitialTutorial,	// NewGame時専用、一回きり
		SelectAction,		// アクションの選択
		SelectDetail,		// アクションの中身を選択
		InAction,			// アクション中
		ResultAction,		// アクションの結果
		Pause,				// ポーズ
		Max
	};
	/*
	 *	@brief	行動の種類
	 */
	enum class ActionType {
		Invalid = -1,
		Dungeon,		// ダンジョン
		Training,		// トレーニング
		Shop,			// ショップ
		PartTime,		// アルバイト

		Max
	};
}
#endif // !_GAME_ENUM_H_