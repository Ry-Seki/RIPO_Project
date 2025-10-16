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
#include "../GameConst.h"
 /*
  *  ステージクラス
  */
class Stage : public StageBase {
private:
	std::string modelPath;	// モデルのパス

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

	/*
	 * @brief ステージの当たり判定を更新
	 * @param position     オブジェクト位置
	 * @param PolyPos1     カプセル当たり判定下端
	 * @param PolyPos2     カプセル当たり判定上端
	 * @param MoveVec      移動ベクトル
	 */
	void UpdateCollision(Vector3* position, Vector3 PolyPos1, Vector3 PolyPos2, Vector3 MoveVec) override;

private:

	/*
	 * @brief 周囲ポリゴンの当たり判定を実行し、結果を返す
	 * @param position  プレイヤー位置
	 * @param MoveVec   移動ベクトル
	 * @return std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> コリジョン結果構造体
	 */
	std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> SetupCollision(Vector3* position, Vector3 MoveVec);


	/**
	 * @brief 壁ポリゴン／床ポリゴンの振り分け
	 * @param hitDim   コリジョン結果構造体
	 * @param prevPos  移動前の座標
	 * @param walls    壁ポリゴン格納先
	 * @param floors   床ポリゴン格納先
	 */
	void ClassifyPolygons(
		const MV1_COLL_RESULT_POLY_DIM& hitDim,
		const Vector3& prevPos,
		std::vector<MV1_COLL_RESULT_POLY*>& walls,
		std::vector<MV1_COLL_RESULT_POLY*>& floors
	);

	/**
	 * @brief 壁ポリゴンとの衝突・スライド・押し出し処理
	 * @param nowPos      現在座標（処理結果として更新）
	 * @param prevPos     移動前座標
	 * @param polyOffset  カプセル縦方向のオフセット
	 * @param MoveVec     移動ベクトル
	 * @param walls       壁ポリゴン配列
	 * @param moveFlag    水平移動しているかどうか
	 */
	void ProcessWallCollision(
		Vector3& nowPos,
		const Vector3& prevPos,
		float polyOffset,
		const Vector3& MoveVec,
		const std::vector<MV1_COLL_RESULT_POLY*>& walls,
		bool moveFlag
	);

	/**
	 * @brief 床ポリゴンとの衝突処理
	 * @param nowPos      現在座標
	 * @param polyOffset  カプセル縦方向のオフセット
	 * @param floors      床ポリゴン配列
	 */
	void ProcessFloorCollision(
		Vector3& nowPos,
		float polyOffset,
		const std::vector<MV1_COLL_RESULT_POLY*>& floors
	);
};


#endif // !_STAGE_H_