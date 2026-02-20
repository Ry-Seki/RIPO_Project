/*
 *	@file	MiniGameSokoban.h
 *	@author	Seki
 */

#ifndef _MINI_GAME_SOKOBAN_H_
#define _MINI_GAME_SOKOBAN_H_

#include "../MiniGameBase.h"
#include "SokobanMapCreater.h"
#include "../../../../../../VecMath.h"

#include <vector>

/*
 *	@brief	倉庫番ミニゲーム
 */
class MiniGameSokoban : public MiniGameBase {
	/*
	 *	@brief	箱構造体
	 */
	struct SokobanBox {
		int x = 0, y = 0;				// 座標
		float drawX = 0, drawY = 0;		// 描画座標(px)
		int moveDX = 0, moveDY = 0;		// 移動方向
		float moveTimer = 1.0f;			// 補間用(1.0f で停止)
		bool isMoving = false;			// 移動フラグ
	};
private:
	SokobanMapCreater createMap;				// マップクリエイター
	std::vector<std::vector<TileType>> initMap;	// 所持時点での倉庫番マップ
	std::vector<std::vector<TileType>> map;		// 倉庫番マップ
	std::vector<std::vector<bool>> goalMap;		// ゴールマップ
	SokobanBox player;							// プレイヤー
	std::vector<SokobanBox> boxList;			// 箱リスト

	int screenWidth = 0, screenHeight = 0;
	const float _MOVE_DURATION = 0.15f;			// 補間時間
	const int _TILE_SIZE = 64;					// タイルサイズ

	static constexpr const char* _SOKOBAN_PATH = "Data/MiniGame/Sokoban/Map/";

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
	void Update(float deltaTime) override;
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
	 *	@brief		マップの初期化処理
	 */
	void InitializeMap();
	/*
	 *  @brief		箱リストをマップから抽出
	 */
	void ParseBoxesFromMap();
	/*
	 *  @brief      プレイヤー移動開始（即時適応）
	 *  @param[in]  int dx, dy  移動方向
	 */
	void StartPlayerMove(int dx, int dy);
	/*
	 *  @brief      箱を押し、補間準備処理
	 *  @param[in]  int bx, by　箱の座標
	 *  @param[in]  int dx, dy  移動方向
	 *  @return     bool
	 */
	bool TryPushBox(int bx, int by, int dx, int dy);
	/*
	 *  @brief      補間移動更新処理
	 */
	void UpdateInterp(float deltaTime);
	/*
	 *  @brief      マップリセット処理
	 */
	void Reset();

private:
	/*
	 *	@brief		難易度からフォルダのパスに変更
	 *	@param[in]	GameEnum::MiniGameLevel setLevel
	 *	@return		std::string
	 */
	std::string GetMiniGamePath() const;
	/*
	 *  @brief      座標指定の箱の取得
	 *  @param[in]	int x, y		箱の座標
	 *  @return     SokobanBox*
	 */
	SokobanBox* GetBox(int x, int y);
	/*
	 *  @brief      補間移動中か判定
	 *  @return     bool
	 */
	bool IsMoving() const;
	/*
	 *	@brief		クリア判定
	 *	@return		bool
	 */
	bool IsClear();


public:
	/*
	 *	@brief		0->1->0での移動を行う関数
	 *	@param[in]	float moveTime
	 */
	inline float EaseInOut(float moveTime) { return 0.5f * (1 - cosf(moveTime * Pi)); }
};
#endif // !_MINI_GAME_SOKOBAN_H_