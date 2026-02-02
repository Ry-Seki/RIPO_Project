/*
 *	@file	StageCollision.h
 *  @author oorui
 */

#ifndef _STAGECOLLISION_H_
#define _STAGECOLLISION_H_

#include <memory>
#include <vector>
#include <DxLib.h>
#include "../VecMath.h"

 // 前方宣言
class GameObject;
class CapsuleCollider;
class GravityComponent;

/*
 *	ステージの当たり判定を行うクラス
 */
class StageCollision {
private:
	int modelHandle;	// ステージのモデルハンドル

	static constexpr float _CHARACTER_MOVEVEC_MIN = 0.01f;		// キャラクターの移動量の最小値
	static constexpr float _HALF = 0.5f;						// 半分
	static constexpr float _POLYGON_HEIGHT = 0.9f;				// 壁の角度
	static constexpr float _FLOOR_LIMIT = 0.5f;					// 床の角度


public:
	/*
	 *	コンストラクタ
	 *  @param	int	ステージのモデルハンドル
	 */
	StageCollision(int modelHandle);

public:
	/*
	 *	当たり判定の更新
	 *  @param	GameObject* 当たり判定をするキャラクター
	 *  @param	Vector3		上記で渡されたキャラクターの移動量
	 */
	void UpdateCollision(GameObject* other, Vector3 moveVec);


private:

	/*
	 * @brief 周囲ポリゴンの当たり判定を実行し、結果を返す
	 * @param position  プレイヤー位置
	 * @param MoveVec   移動ベクトル
	 * @return std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> コリジョン結果構造体
	 */
	std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> SetupCollision(GameObject* other, Vector3 MoveVec);


	/**
	 * @brief 壁ポリゴン／床ポリゴンの振り分け
	 * @param hitDim   コリジョン結果構造体
	 * @param walls    壁ポリゴン格納先
	 * @param floors   床ポリゴン格納先
	 */
	void ClassifyPolygons(
		const MV1_COLL_RESULT_POLY_DIM& hitDim,
		std::vector<MV1_COLL_RESULT_POLY*>& walls,
		std::vector<MV1_COLL_RESULT_POLY*>& floors,
		const Vector3& prevPos
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
		bool moveFlag,
		GameObject* other
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
		const std::vector<MV1_COLL_RESULT_POLY*>& floors,
		GameObject* other,
		Vector3 moveVec
	);

public:

	/*
	 *	ステージデータの読み込み
	 *  @pram const int modelHandle
	 */
	void SetModelHandle(const int modelHandle);

	/*
	 *	ステージの当たり判定の描画
	 *  @param	GameObject* キャラクター
	 *  @param	Vector3		移動量
	 *  @param	Vector3		直前の移動量
	 */
	void StageColliderRenderer(GameObject* other, Vector3 MoveVec, Vector3 prevPos);


};


#endif // !_STAGECOLLISION_H_

