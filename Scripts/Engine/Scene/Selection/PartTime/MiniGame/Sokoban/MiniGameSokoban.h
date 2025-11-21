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
 *	@brief	箱構造体
 */
struct SokobanBox {
	int x = 0, y = 0;				// ロジック座標
	float drawX = 0, drawY = 0;		// 描画座標(px)
	int moveDX = 0, moveDY = 0;		// 移動方向
	float moveTimer = 1.0f;			// 補間用(1.0f で停止)
	bool isMoving = false;			// 移動フラグ
};

/*
 *	@brief	倉庫番ミニゲーム
 */
class MiniGameSokoban : public MiniGameBase {
private:
	SokobanMapCreater createMap;				// マップクリエイター
	std::vector<std::vector<TileType>> initMap;	// 所持時点での倉庫番マップ
	std::vector<std::vector<TileType>> map;		// 倉庫番マップ
	std::vector<std::vector<bool>> goalMap;		// ゴールマップ

	// プレイヤー
	int playerX = 0, playerY = 0;
	float playerDrawX = 0, playerDrawY = 0;
	int moveDX = 0, moveDY = 0;
	float moveTimer = 1.0f;

	const float MOVE_DURATION = 0.15f;  // 補間時間


	std::vector<SokobanBox> boxList;	// 箱リスト

	const int TILE_SIZE = 32;			// タイルサイズ

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
	void ParseBoxesFromMap();
	void StartPlayerMove(int dx, int dy);
	SokobanBox* GetBox(int x, int y);

	bool IsMoving() const;
	void UpdateInterp(float dt);
	void Reset();
	bool TryPushBox(int bx, int by, int dx, int dy);
	bool IsClear();

	// 0->1での移動を行う関数
	inline float EaseInOut(float t) {
		return t * t * (3 - 2 * t);
	}

};
#endif // !_MINI_GAME_SOKOBAN_H_