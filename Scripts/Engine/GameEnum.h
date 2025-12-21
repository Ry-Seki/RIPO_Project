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
		SelectAction,		// アクションの選択
		SelectDetail,		// アクションの中身を選択
		InAction,			// アクション中
	};
	/*
	 *	@brief	行動の種類
	 */
	enum class ActionType {
		Dungeon,
		Training,
		Shop,
		PartTime
	};
}
#endif // !_GAME_ENUM_H_