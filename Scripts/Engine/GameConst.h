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
	static constexpr float PLAYER_ENUM_DEFAULT_SIZE = 20000.0f;	 // 周囲のポリゴン検出に使用するサイズ
	static constexpr float PLAYER_HIT_HEIGHT = 200.0f;		 // 当たり判定カプセルの半径 
	static constexpr const char* _STATUS_KEY = "InitStatus";	 // プレイヤーのステータスキー
	static constexpr const char* _RASE_KEY = "RiseValue";	 // プレイヤーのステータス上昇値のキー

	static constexpr const char* STATUS_PART[4] = {			 // プレイヤーの各ステータスキー
		"HP",
		"Stamina",
		"Strength",
		"ResistTime"
	};
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
	static constexpr float FPS = 60.0f;
	static constexpr float frameTime = 1.0f / FPS;

	//----------------------------------------------------
	//				ステージ関連の定数
	//----------------------------------------------------
	static constexpr int HIT_TRYNUM = 16;								// 壁押し出し処理の最大試行回数
	static constexpr float HIT_SLIDE_LENGTH = 50.0f;						// 一度の壁押し出し処理でスライドさせる距離
	static constexpr const float VIEW_RADIUS = 60.0f;					// 取得範囲




	//----------------------------------------------------
	//				オブジェクト名関連の定数
	//----------------------------------------------------
	static constexpr const char* _CREATE_POSNAME_ENEMY = "Enemy";		// 敵の生成位置の名前
	static constexpr const char* _CREATE_POSNAME_PLAYER = "Player";		// プレイヤーの生成位置の名前
	static constexpr const char* _CREATE_POSNAME_TREASURE = "Treasure";	// お宝の生成位置の名前
	static constexpr const char* _CREATE_POSNAME_LIGHT = "PointLight";	// ポイントライトの生成位置の名前
	static constexpr const char* _CREATE_POSITION_SPAWN = "SpawnPos";	// スポーンポジションの名前


	//----------------------------------------------------
	//					お宝の名前
	//----------------------------------------------------
	static constexpr const char* _TREASURE_JAR = "jar";				// お宝の名前 : 壺
	static constexpr const char* _TREASURE_LION = "LionStatue";		// お宝の名前 : ライオンの像

};
#endif // !_GAMECONST_H_

