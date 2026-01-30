/*
 *	@file	StageCollision.cpp
 *  @author oorui
 */

#include "StageCollision.h"

#include "../GameObject.h"
#include "../Component/CapsuleCollider.h"
#include "../Component/GravityComponent.h"
#include "../GameConst.h"

 /*
  *	コンストラクタ
  */
StageCollision::StageCollision(int modelHandle)
	:modelHandle(modelHandle) {

}

/*
 *	当たり判定の更新
 *  @param	GameObject* 当たり判定をするキャラクター
 *  @param	Vector3		上記で渡されたキャラクターの移動量
 */
void StageCollision::UpdateCollision(GameObject* other, Vector3 moveVec) {
	// 移動後の位置
	Vector3 nowPos = other->position;
	// 移動前の座標
	Vector3 prevPos = nowPos - moveVec;

	// プレイヤーの座標
	Vector3 PolyPos = other->position;
	// PolyPosTopより指定座標分高い位置
	Vector3 PolyPosTop = other->position;
	PolyPosTop.y += GameConst::PLAYER_HIT_HEIGHT;

	// 縦方向のオフセット
	float polyOffset = PolyPosTop.y - PolyPos.y;
	// キャラクターが水平移動しているかどうか
	bool moveFlag = (fabs(moveVec.x) > _CHARACTER_MOVEVEC_MIN || fabs(moveVec.z) > _CHARACTER_MOVEVEC_MIN);

	// ステージのコリジョン情報取得
	auto hitDim = SetupCollision(other, moveVec);

	// 壁床ポリゴン分類
	std::vector<MV1_COLL_RESULT_POLY*> walls, floors;
	// 壁か床か振り分ける
	ClassifyPolygons(*hitDim, walls, floors, prevPos);

	// 床衝突処理
	ProcessFloorCollision(nowPos, polyOffset, floors, other, moveVec);

	// 壁衝突処理
	ProcessWallCollision(nowPos, prevPos, polyOffset, moveVec, walls, moveFlag, other);


	// 結果反映
	other->position = nowPos;

	if (hitDim && hitDim->HitNum > 0) {
		MV1CollResultPolyDimTerminate(*hitDim);
	}
}

/*
 * @brief 周囲ポリゴンの当たり判定を実行し、結果を返す
 * @param position  プレイヤー位置
 * @param MoveVec   移動ベクトル
 * @return std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> コリジョン結果構造体
 */
std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> StageCollision::SetupCollision(
	GameObject* other,
	Vector3 MoveVec) {
	// 対象がいない場合は抜ける
	if (!other) return std::make_unique<MV1_COLL_RESULT_POLY_DIM>();
	// モデルがロードされていない場合も抜ける
	if (modelHandle < 0) return std::make_unique<MV1_COLL_RESULT_POLY_DIM>();

	// 移動していない場合、最小範囲で当たり判定を検知
	Vector3 effectiveMove = MoveVec;
	if (Magnitude(effectiveMove) < _CHARACTER_MOVEVEC_MIN) {
		effectiveMove = Vector3::zero;
	}

	// 移動方向の単位ベクトル
	Vector3 forward = Vector3::zero;
	// 移動距離の長さ
	float moveLen = effectiveMove.Magnitude();
	if (moveLen > 0.0f) {
		forward = Normalized(effectiveMove);
	}

	// カプセルの取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule)return std::make_unique<MV1_COLL_RESULT_POLY_DIM>();

	// カプセルの要素をワールド座標に変換

	// 下端
	Vector3 capLower = other->position + capsule->capsule.segment.startPoint;
	// 上端
	Vector3 capTop = other->position + capsule->capsule.segment.endPoint;
	// 中心点
	Vector3 capCenter = (capLower + capTop) * _HALF;
	// 半径
	float radius = capsule->capsule.radius;

	// 前方限定の球判定用中心
	Vector3 sphereCenter = capCenter + forward * (moveLen * _HALF);

	// 球の半径
	float sphereRadius = radius + moveLen * _HALF;

	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();
	*hitDim = MV1CollCheck_Sphere(modelHandle, -1, ToVECTOR(sphereCenter), sphereRadius);

	return hitDim;
}

/*
 * @brief 壁ポリゴン／床ポリゴンの振り分け
 * @param hitDim   コリジョン結果構造体
 * @param walls    壁ポリゴン格納先
 * @param floors   床ポリゴン格納先
 */
void StageCollision::ClassifyPolygons(
	const MV1_COLL_RESULT_POLY_DIM& hitDim,
	std::vector<MV1_COLL_RESULT_POLY*>& walls,
	std::vector<MV1_COLL_RESULT_POLY*>& floors,
	const Vector3& prevPos
) {

	// 全ポリゴンをチェック
	for (int i = 0; i < hitDim.HitNum; i++) {
		// ヒットしたポリゴンの配列
		const auto& poly = hitDim.Dim[i];

		// 壁
		bool isWall = (poly.Normal.y < _POLYGON_HEIGHT);
		// 床
		bool isFloor = (poly.Normal.y >= _FLOOR_LIMIT);
		// 壁かどうか
		if (isFloor) {
			// 壁ポリゴン配列に追加する
			floors.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
		}

		if (isWall) {
			// 床ポリゴン配列に追加する
			walls.push_back(const_cast<MV1_COLL_RESULT_POLY*>(&poly));
		}
	}

}

/*
 * @brief 壁ポリゴンとの衝突・スライド・押し出し処理
 * @param nowPos      現在座標
 * @param prevPos     移動前座標
 * @param polyOffset  カプセル縦方向のオフセット
 * @param MoveVec     移動ベクトル
 * @param walls       壁ポリゴン配列
 * @param moveFlag    水平移動しているかどうか
 */
void StageCollision::ProcessWallCollision(
	Vector3& nowPos,
	const Vector3& prevPos,
	float polyOffset,
	const Vector3& MoveVec,
	const std::vector<MV1_COLL_RESULT_POLY*>& walls,
	bool moveFlag,
	GameObject* other
) {
	// 壁が存在しなければ抜ける
	if (walls.empty()) return;

	// カプセルコンポーネントの取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	// カプセルを取得できなければ抜ける
	if (!capsule)return;

	// カプセルの半径を取得
	float capsuleRadius = capsule->capsule.radius;

	// ワールド座標に変換したカプセルの下端
	Vector3 capLower = nowPos + capsule->capsule.segment.startPoint;
	// ワールド座標に変換したカプセルの上端
	Vector3 capTop = nowPos + capsule->capsule.segment.endPoint;

	// 壁ポリゴンの配列分回す
	for (auto* poly : walls) {
		// 三角形の 3 頂点
		Vector3 p0 = FromVECTOR(poly->Position[0]);
		Vector3 p1 = FromVECTOR(poly->Position[1]);
		Vector3 p2 = FromVECTOR(poly->Position[2]);


		// カプセル中心点
		Vector3 capCenter = (capLower + capTop) * _HALF;

		// 最近接点を求める
		Vector3 nearest = Nearest(capCenter, p0, p1, p2);

		// どれだけ貫通しているかを求める
		Vector3 diff = capCenter - nearest;

		float dist = Magnitude(diff);

		// カプセルが触れていないなら処理しない
		float penetrate = capsuleRadius - dist;
		if (penetrate <= Vector3::zero.y) continue;

		Vector3 pushDir = Normalized(diff);

		// 水平方向のみ押し出す
		if (poly->HitPosition.y > prevPos.y + _POLYGON_HEIGHT) {
			pushDir.y = 0.0f;

		}

		// 水平方向のみで押し戻す
		if (Magnitude(pushDir) > 0.0f) {
			pushDir = Normalized(pushDir);
			nowPos += pushDir * penetrate;
		}

		// 新しいカプセル位置
		capLower = nowPos + capsule->capsule.segment.startPoint;
		capTop = nowPos + capsule->capsule.segment.endPoint;
	}

	if (moveFlag) {
		// 元の移動ベクトル
		Vector3 moveVec = MoveVec;

		// 壁の向きを一つにまとめる
		Vector3 avgNormal = Vector3::zero;
		int count = 0;

		// ポリゴンの法線を足していく
		for (auto* poly : walls) {
			// 法線をVector3型に直し、乗算
			avgNormal += FromVECTOR(poly->Normal);
			count++;
		}
		// 正規化した長さに変換
		avgNormal = Normalized((avgNormal / (float)count));

		// 移動ベクトルと壁の向きの内積
		float dot = Vector3::Dot(moveVec, avgNormal);

	}
}


/*
 * @brief 床ポリゴンとの衝突処理
 * @param nowPos      現在座標
 * @param polyOffset  カプセル縦方向のオフセット
 * @param floors      床ポリゴン配列
 */
void StageCollision::ProcessFloorCollision(
	Vector3& nowPos,
	float polyOffset,
	const std::vector<MV1_COLL_RESULT_POLY*>& floors,
	GameObject* other,
	Vector3 moveVec
) {
	// 床がなければ落下
	if (floors.empty()) {
		auto gravity = other->GetComponent<GravityComponent>();
		if (gravity) {
			// 初回フレームのみ接地
			if (gravity->GetFirstFrame()) {
				gravity->SetGroundingFrag(true);
			}
			else {
				gravity->SetGroundingFrag(false);
			}
		}
		return;
	}
	// 接地点の最大値
	float MaxY = -FLT_MAX;
	// 接地しているかどうか
	bool isGround = false;

	// 重力コンポーネントの取得
	auto hitGrounding = other->GetComponent<GravityComponent>();
	if (!hitGrounding) return;

	// カプセルコンポーネント取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return;

	float prevY = nowPos.y - moveVec.y;

	// ワールド座標での下端
	Vector3 capStart = nowPos + capsule->capsule.segment.startPoint;
	// ワールド座標での上端
	Vector3 capEnd = nowPos + capsule->capsule.segment.endPoint;
	// カプセル中心点
	Vector3 capCenter = (capStart + capEnd) * _HALF;
	// カプセルの半径
	float capsuleRadius = capsule->capsule.radius;

	Vector3 checkPos = capStart + Vector3(moveVec.x, 0.0f, moveVec.z);


	// 貫通しているかどうかを見る
	for (auto* poly : floors) {
		// 三角形の 3 頂点
		Vector3 p0 = FromVECTOR(poly->Position[0]);
		Vector3 p1 = FromVECTOR(poly->Position[1]);
		Vector3 p2 = FromVECTOR(poly->Position[2]);

		// 中心点と三角形の最近接点を求める
		Vector3 nearest = Nearest(checkPos, p0, p1, p2);

		// 最近接点との差分と距離
		Vector3 diff = checkPos - nearest;
		float dist = Magnitude(diff);
		// 「前フレームより極端に高い床」は無視
		if (nearest.y > prevY + GameConst::PLAYER_HIT_HEIGHT * 0.1f) {
			continue;
		}
		// カプセルの半径以下なら接地
		const float EPS = _HALF;
		if (dist <= capsuleRadius) {
			// 接地判定
			if (!isGround || MaxY < nearest.y) {
				isGround = true;
				MaxY = nearest.y;
			}
		}
	}

	// 地面から離れていた場合は接地判定を行わない
	if (hitGrounding->GetFallSpeed() < 0) {
		hitGrounding->SetGroundingFrag(false);
		return;
	}

	// 接地しているかどうかの判定を重力コンポーネントに渡す
	if (isGround) {
		// 対象がいなければいけない位置
		float desiredY = (MaxY + (capsuleRadius - capsule->capsule.segment.startPoint.y));

		// 壁から押し出されたとき対策
		// 床よりもしたに押し出された場合、押し戻す
		if (nowPos.y < MaxY) {
			nowPos.y = desiredY;
		}
		// 接地している
		hitGrounding->SetGroundingFrag(true);
	}
	else {
		// 接地していない
		hitGrounding->SetGroundingFrag(false);
	}

}


/*
 *	ステージの当たり判定の描画
 */
void StageCollision::StageColliderRenderer(GameObject* other, Vector3 MoveVec, Vector3 prevPos) {
	// モデルハンドルが無効な場合は処理を中止
	if (modelHandle < 0) return;

	auto hitDim = SetupCollision(other, MoveVec);

	// 壁と床を区別するための色情報を設定
	unsigned int wallColor = GetColor(255, 100, 100);
	unsigned int floorColor = GetColor(100, 255, 100);
	unsigned int lineColor = GetColor(255, 255, 255);

	// 当たった全てのポリゴンに対して描画処理を実行
	for (int i = 0; i < hitDim->HitNum; i++) {
		const auto& poly = hitDim->Dim[i];

		// 壁
		bool isWall = (poly.Normal.y < _POLYGON_HEIGHT);
		// 床
		bool isFloor = (poly.Normal.y >= _FLOOR_LIMIT);
		unsigned int drawColor;

		// 壁かどうか
		if (isFloor) {
			// 壁ポリゴン配列に追加する
			drawColor = floorColor;
		}

		if (isWall) {
			// 床ポリゴン配列に追加する
			drawColor = wallColor;
		}

		// ポリゴンを半透明で描画
		DrawTriangle3D(poly.Position[0], poly.Position[1], poly.Position[2], drawColor, TRUE);

		// ポリゴンを白線で描画
		DrawLine3D(poly.Position[0], poly.Position[1], lineColor);
		DrawLine3D(poly.Position[1], poly.Position[2], lineColor);
		DrawLine3D(poly.Position[2], poly.Position[0], lineColor);
	}

	// 使用した衝突判定データを解放
	MV1CollResultPolyDimTerminate(*hitDim);
}