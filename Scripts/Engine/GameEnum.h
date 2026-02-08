/*
 *	@file	GameEnum.h
 *	@author	Seki
 */

#ifndef _GAME_ENUM_H_
#define _GAME_ENUM_H_

namespace GameEnum {
	/*
	 *	@brief	シーンの種類
	 *  @author	Seki
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
	 *  @author	Seki
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
	 *  @author	Seki
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
	 *  @author	Seki
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
	 *  @author	Seki
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
	 *  @author	Seki
	 */
	enum class MiniGameLevel {
		Invalid = -1,
		Easy,			// 簡単
		Normal,			// 普通
		Hard,			// 難しい
		Max
	};
	/*
	 *	@brief	リザルトランク
	 *	@author	Seki
	 */
	enum class ResultRank {
		Invalid = -1,
		S,
		A,
		B,
		C,
		D,
		Max
	};
	/*
	 *	@brief	セーブスロットメニューでのモード
	 *	@author	Seki
	 */
	enum class SaveSlotMenuMode {
		Save,
		Load
	};
	/*
	 *	@brief	ボタンの種類
	 *	@author	Seki
	 */
	enum class ButtonType {
		Invalid = -1,
		SinglePressButton,
		LongPressButton,

		Max
	};
	/*
	 *	@brief	ボタンの入力状態
	 *	@author	Seki
	 */
	enum class ButtonInputState {
		Invalid = -1,
		Idle,			// 通常状態
		Hover,			// 触れている状態
		Press,			// 押下中状態
		Max
	};
	/*
	 *	@brief	ボタンの選択状態
	 *  @author	Seki
	 */
	enum class ButtonSelectState {
		Invalid = -1,
		Select,			// 選択状態
		Max
	};
	/*
	 *	@brief	ボタンの描画状態
	 */
	enum class ButtonRendererState {
		Invalid = -1,
		Idle,
		Hover,
		Press,
		Select,
		Disable,
		Max
	};
	/*
	 *	@brief	移動の道筋
	 *  @author Seki
	 */
	enum class NavigationDir {
		Invalid = -1,
		Left,
		Right,
		Up,
		Down,
		Max
	};
	/*
	 *	@brief	確認の種類
	 */
	enum class ConfirmResult {
		Yes,
		No
	};
	/*
	 *  @brief	アクションマップ
	 *  @author	Riku
	 */
	enum class ActionMap {
		Incalid = -1,
		PlayerAction,
		MenuAction,
		Max
	};
	/*
	 *	@brief	プレイヤーの行動
	 *	@author	Riku
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
		OpenMenu,		// メニューを開く
		BulletReload,	// 銃のリロード
		FirstWeapon,	// 武器1
		SecondWeapon,	// 武器2

		Max
	};
	/*
	 *	@brief	メニューでの入力
	 *	@author	Seki
	 */
	enum class MenuAction {
		Invalid = -1,
		Vertical,	// 垂直方向
		Horizontal,	// 水平方向
		Decide,		// 決定
		Click,		// クリック
		Cancel,		// キャンセル
		Max
	};

	/*
	 *	@brief	ウデの種類
	 *	@author	Riku
	 */
	enum class Arm {
		Invalid = -1,
		Hand,	// 手
		Weapon,	// 武器

		Max
	};
	/*
	 *	@brief	銃の種類
	 *	@author Riku
	 */
	enum class Weapon {
		Invalid = -1,
		Revolver,		// リボルバー(単発銃)
		SubmachineGun,	// サブマシンガン(連射銃)

		Max
	};

	/*
	 *	@brief	敵の種類
	 *	@author kuu
	 */
	enum class EnemyType {
		Invalid = -1,

		TutorialEnemy,	// チュートリアルの通常エネミー
		Stage1Enemy,	// ステージ1の通常エネミー
		Stage2Enemy,	// ステージ2の通常エネミー
		Stage3Enemy,	// ステージ3の通常エネミー
		Stage4Enemy,	// ステージ4の通常エネミー
		Stage1Boss,		// ステージ1のボス
		Stage2Boss,		// ステージ2のボス
		Stage3Boss,		// ステージ3のボス
		Stage4Boss,		// ステージ4のボス

		Max
	};
}
#endif // !_GAME_ENUM_H_