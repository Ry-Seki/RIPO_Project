/*
 *	@file	GameConst.h
 *  @author	oorui
 */

#ifndef _GAMECONST_H_
#define _GAMECONST_H_

#include <DxLib.h>
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
	static constexpr float PLAYER_HIT_HEIGHT = 0.5f;			 // 当たり判定カプセルの半径 

	//----------------------------------------------------
	//					敵関連の定数
	//----------------------------------------------------


	//----------------------------------------------------
	//				システム関連の定数
	//----------------------------------------------------
	static constexpr int WINDOW_WIDTH = 800;				// ウィンドウの幅
	static constexpr int WINDOW_HEIGHT = 600;				// ウィンドウの高さ
	static constexpr int FORWARD_ANGLE = 0;
	static constexpr int RIGHT_ANGLE = 90;
	static constexpr int LEFT_ANGLE = -90;
	static constexpr int BACK_ANGLE = 180;
	static constexpr float _MAX_DXLIB_VOLUME = 255.0f;
	static constexpr float _MIN_VOLUME = 0.0f;
	static constexpr float _MAX_VOLUME = 100.0f;
	static constexpr int _MAX_SE_INDEX = 16;

	//----------------------------------------------------
	//				ステージ関連の定数
	//----------------------------------------------------
	static constexpr int HIT_TRYNUM = 16;								// 壁押し出し処理の最大試行回数
	static constexpr float HIT_SLIDE_LENGTH = 0.1f;						// 一度の壁押し出し処理でスライドさせる距離
	static constexpr const float VIEW_RADIUS = 60.0f;					// 取得範囲
	static constexpr DxLib::COLOR_F _MAP_DIF_LIGHTCOLOR = { 1.5f, 1.5f, 1.3f, 1.0f };	// マップ全体のライト ディフューズカラー




	//----------------------------------------------------
	//				オブジェクト名関連の定数
	//----------------------------------------------------
	static constexpr const char* _CREATE_POSNAME_ENEMY = "Enemy";		// 敵の生成位置の名前
	static constexpr const char* _CREATE_POSNAME_PLAYER = "Player";		// プレイヤーの生成位置の名前
	static constexpr const char* _CREATE_POSNAME_TREASURE = "Treasure";	// お宝の生成位置の名前
	static constexpr const char* _CREATE_POSNAME_LIGHT = "PointLight";	// ポイントライトの生成位置の名前
	static constexpr const char* _CREATE_POSITION_SPAWN = "SpawnPos";	// スポーンポジションの名前


};
#endif // !_GAMECONST_H_

