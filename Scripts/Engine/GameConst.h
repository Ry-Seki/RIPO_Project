/*
 *	@file	GameConst.h
 *  @author	oorui
 */

#ifndef _GAMECONST_H_
#define _GAMECONST_H_

 /*
  *	定数
  */
class GameConst {
public:

	//----------------------------------------------------
	//				プレイヤー関連の定数
	//----------------------------------------------------
	static constexpr   int PLAYER_MAX_HITCOLL = 1024;		 // 処理するコリジョンポリゴンの最大数
	static constexpr float PLAYER_ENUM_DEFAULT_SIZE = 5.0f;	 // 周囲のポリゴン検出に使用するサイズ
	static constexpr float PLAYER_HIT_WIDTH = 0.5f;			 // 当たり判定カプセルの半径 

	//----------------------------------------------------
	//					敵関連の定数
	//----------------------------------------------------



	//----------------------------------------------------
	//				システム関連の定数
	//----------------------------------------------------





	//----------------------------------------------------
	//				ステージ関連の定数
	//----------------------------------------------------
	static constexpr int HIT_TRYNUM = 16;							// 壁押し出し処理の最大試行回数
	static constexpr float HIT_SLIDE_LENGTH = 0.1f;					// 一度の壁押し出し処理でスライドさせる距離
	static constexpr const char* START_FRAME_NAME = "Col_StartPos";	// スタートの位置のフレーム
	static constexpr const char* GOAL_FRAME_NAME = "Col_GoalPos";	// ゴールの位置のフレーム
	static constexpr const float VIEW_RADIUS = 60.0f;				// 取得範囲

};
#endif // !_GAMECONST_H_

