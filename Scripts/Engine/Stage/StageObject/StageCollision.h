/*
 *	@file	StageCollision.h
 *  @author oorui
 */

#ifndef _STAGECOLLISION_H_
#define _STAGECOLLISION_H_

#include <memory>
#include <vector>
#include <DxLib.h>
#include "../../VecMath.h"
#include "../../GameObject.h"

 /*
  * ステージの当たり判定をするクラス
  */
class StageCollision {

public:
	StageCollision() = default;
	~StageCollision() = default;

public:

	/*
	 *	ステージの当たり判定の更新
	 *  @param StageModleHandle		ステージのモデル
	 *  @param other				当たり判定をする対象
	 *  @param moveVec				対象の移動量
	 */
	void CollisionUpdate(
		int StageModelHandle,
		GameObject* other,
		Vector3 moveVec
	);

private:

	/*
	 *	@brief 衝突判定用ポリゴン取得
	 *	@param	stageModelHandle	ステージのモデル
	 *  @param	other				当たり判定をする対象
	 *  @parma	moveVec				対象の当たり判定
	 */
	std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> SetupCollision(
		int stageModelHandle,
		GameObject* other,
		Vector3 moveVec
	);

	/*
	 *	@brief 壁／床ポリゴン分類
	 *  @param	hitDim	コリジョン結果構造体
	 *  @param	walls	壁
	 *  @param	floors	床
	 */
	void ClassifyPolygons(
		const MV1_COLL_RESULT_POLY_DIM& hitDim,
		std::vector<MV1_COLL_RESULT_POLY*>& walls,
		std::vector<MV1_COLL_RESULT_POLY*>& floors
	);

	/*
	 *	@brief 壁衝突処理
	 *	@param	nowPos		現在の座標
	 *  @param	prevPos		直前の座標
	 *  @param	walls		壁
	 *  @param	other		対象
	 */
	void ProcessWallCollision(
		Vector3& nowPos,
		const Vector3& prevPos,
		const std::vector<MV1_COLL_RESULT_POLY*>& walls,
		GameObject* other
	);

	/*
	 *	@brief 床衝突処理
	 *  @param	nowPos	現在の座標
	 *  @param	floors	床
	 *  @param  other	対象
	 */
	void ProcessFloorCollision(
		Vector3& nowPos,
		const std::vector<MV1_COLL_RESULT_POLY*>& floors,
		GameObject* other
	);

private:
	static constexpr float MOVE_EPS = 0.01f;
	static constexpr float POLYGON_HEIGHT = 0.9f;




};

#endif _STAGECOLLISION_H_
