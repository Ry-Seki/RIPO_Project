/*
 *  @file Stage.h
 *  @author oorui
 */

#ifndef _STAGE_H_
#define _STAGE_H_

#include "StageBase.h"
#include <string>
#include <vector>
#include "../Load/LoadManager.h"
#include "../VecMath.h"
 /*
  *  ステージクラス
  */
class Stage : public StageBase {
private:
	std::string modelPath;	// モデルのパス

	const static int PLAYER_MAX_HITCOLL = 1024;					// 処理するコリジョンポリゴンの最大数
	static inline const float PLAYER_ENUM_DEFAULT_SIZE = 5.0f;	// 周囲のポリゴン検出に使用するサイズ
	static inline const float PLAYER_HIT_WIDTH = 0.5f;			// 当たり判定カプセルの半径

	const static int HIT_TRYNUM = 16;					// 壁押し出し処理の最大試行回数
	static inline const float HIT_SLIDE_LENGTH = 0.1f;	// 一度の壁押し出し処理でスライドさせる距離



public:
	Stage();
	virtual ~Stage() override;

	// ステージデータ読み込み
	void Load(const std::shared_ptr<LoadModel>& model) override;

	// 更新
	void Update() override;

	// 描画
	void Render() override;

	// 終了処理
	void Execute() override;

	// 当たり判定の更新
	void UpdateCollision(Vector3* position, Vector3 PolyPos1, Vector3 PloyPos2, Vector3 MoveVec) override;
};


#endif // !_STAGE_H_