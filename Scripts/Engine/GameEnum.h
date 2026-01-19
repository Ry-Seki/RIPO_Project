/*
 *	@file	GameEnum.h
 *	@author	Seki
 */

#ifndef _GAME_ENUM_H_
#define _GAME_ENUM_H_

namespace GameEnum {
	/*
	 *	@brief	ゲームモード
	 */
	enum class GameMode {
		Invalid = -1,
		NewGame,		// ニューゲーム
		LoadGame,		// ロードゲーム
		System,			// システム
		Max
	};
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
		EndAction,			// アクションの終了
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
	/*
	 *	@brief	プレイヤーのステータスの種類
	 */
	enum class PlayerStatusType {
		Invalid = -1,
		HP,				// HP
		Stamina,		// スタミナ
		Strength,		// 力
		ResistTime,		// 抵抗力
		Max
	};
	/*
	 *	@brief	ミニゲーム難易度
	 */
	enum class MiniGameLevel {
		Invalid = -1,
		Easy,
		Normal,
		Hard,
		Max
	};
}
#endif // !_GAME_ENUM_H_