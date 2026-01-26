/*
 *	@file	GameEnum.h
 *	@author	Seki
 */

#ifndef _GAME_ENUM_H_
#define _GAME_ENUM_H_

namespace GameEnum {
	/*
	 *	@brief	シーンの種類
	 */
	enum class SceneType {
		Invalid = -1,
		Standby,		// 準備
		Title,			// タイトル
		Tutorial,		// チュートリアル
		MainGame,		// メインゲーム
		Result,			// リザルト
		Max
	};
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
		Easy,			// 簡単
		Normal,			// 普通
		Hard,			// 難しい
		Max
	};

	/*
	 *  @brief	アクションマップ
	 */
	enum class ActionMap {
		Incalid = -1,
		PlayerAction,

		Max,
	};

	/*
	 *	@brief	プレイヤーの行動
	 */
	enum class PlayerAction {
		Invalid = -1,
		ForwardMove,	// 前移動
		RightMove,		// 右移動
		Run,			// ダッシュ
		Avoid,			// 回避
		Jump,			// ジャンプ
		Shot,			// 射撃
		Lift,			// お宝持ち上げ

		Max,
	};
}
#endif // !_GAME_ENUM_H_