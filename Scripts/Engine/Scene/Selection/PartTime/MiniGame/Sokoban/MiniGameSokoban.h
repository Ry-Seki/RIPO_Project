/*
 *	@file	MiniGameSokoban.h
 *	@author	Seki
 */

#ifndef _MINI_GAME_SOKOBAN_H_
#define _MINI_GAME_SOKOBAN_H_

#include "../MiniGameBase.h"
#include "SokobanMapCreater.h"

#include <vector>
#include <DxLib.h>

/*
 *	@brief	倉庫番ミニゲーム
 */
class MiniGameSokoban : public MiniGameBase {
private:
	SokobanMapCreater createMap;				// マップクリエイター
	std::vector<std::vector<TileType>> map;		// 倉庫番マップ
	int playerX = 0, playerY = 0;				// プレイヤーの座標
	int tileSize = 32;							// 1タイルの描画サイズ

public:
	/*
	 *	@brief	コンストラクタ
	 */
	MiniGameSokoban() = default;
	/*
	 *	@brief	デストラクタ
	 */
	~MiniGameSokoban() = default;

public:
	/*
	 *	@brief	ミニゲーム開始時処理
	 */
	void Open() override;
	/*
	 *	@brief	更新処理
	 */
	void Update(Engine& engine, float deltaTime) override;
	/*
	 *	@brief	描画処理
	 */
	void Render() override;
	/*
	 *	@brief	ミニゲーム終了時処理
	 */
	void Close() override;

private:
	/*
	 *	@brief		プレイヤーの移動処理
	 *  @param[in]	int dx
	 *  @param[in]	int dy
	 */
	void MovePlayer(int dx, int dy);
	/*
	 *	@brief		クリア判定
	 *	@return		bool
	 */
	bool IsClear();
};
#endif // !_MINI_GAME_SOKOBAN_H_