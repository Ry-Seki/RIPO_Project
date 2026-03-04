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
	if (!other) return;

	// 現在位置
	Vector3 nowPos = other->position;
	// 移動前位置
	Vector3 prevPos = nowPos - moveVec;

	// 水平移動判定
	bool moveFlag =
		(fabs(moveVec.x) > _CHARACTER_MOVEVEC_MIN ||
			fabs(moveVec.z) > _CHARACTER_MOVEVEC_MIN);

	// 八分木から候補ポリゴン取得
	std::vector<DungeonPoly*> polys =
		SetupCollision(other, moveVec);

	// 壁と床に分類
	std::vector<DungeonPoly*> walls;
	std::vector<DungeonPoly*> floors;

	ClassifyPolygons(polys, walls, floors);

	// 床処理
	ProcessFloorCollision(
		nowPos,
		0.0f,
		floors,
		other,
		moveVec);

	// 壁処理
	ProcessWallCollision(
		nowPos,
		prevPos,
		0.0f,
		moveVec,
		walls,
		moveFlag,
		other);

	// 結果反映
	other->position = nowPos;
}

/*
 *	八分木での空間構築
 */
void StageCollision::BuildSpatialTree() {
	// // ステージ全体AABB取得
	// VECTOR minV, maxV;
	// MV1GetBoundingBox(modelHandle, &minV, &maxV);
	// 
	// Vector3 min = FromVECTOR(minV);
	// Vector3 max = FromVECTOR(maxV);
	// 
	// // レベル6程度で初期化
	// m_tree.Init(6, min, max);
	// 
	// int meshCount = MV1GetMeshNum(modelHandle);
	// 
	// for (int i = 0; i < meshCount; ++i) {
	// 	int polyCount = MV1GetMeshTriangleNum(modelHandle, i);
	// 
	// 	for (int j = 0; j < polyCount; ++j) {
	// 		MV1_TRIANGLE tri;
	// 		MV1GetMeshTriangle(modelHandle, i, j, &tri);
	// 
	// 		// ポリゴン生成
	// 		auto poly = std::make_unique<DungeonPoly>();
	// 
	// 		poly->poly.Position[0] = tri.Position[0];
	// 		poly->poly.Position[1] = tri.Position[1];
	// 		poly->poly.Position[2] = tri.Position[2];
	// 
	// 		// AABB計算
	// 		Vector3 p0 = FromVECTOR(tri.Position[0]);
	// 		Vector3 p1 = FromVECTOR(tri.Position[1]);
	// 		Vector3 p2 = FromVECTOR(tri.Position[2]);
	// 
	// 		poly->min = Min(Min(p0, p1), p2);
	// 		poly->max = Max(Max(p0, p1), p2);
	// 
	// 		// 八分木登録用ノード生成
	// 		auto node = std::make_unique<Object_For_Tree<DungeonPoly>>();
	// 		node->pObject = poly.get();
	// 
	// 		m_tree.Regist(&poly->min, &poly->max, node.get());
	// 
	// 		m_stagePolys.push_back(std::move(poly));
	// 		m_treeNodes.push_back(std::move(node));
	// 	}
	// }
}

/*
 * @brief 周囲ポリゴンの当たり判定を実行し、結果を返す
 * @param position  プレイヤー位置
 * @param MoveVec   移動ベクトル
 * @return std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> コリジョン結果構造体
 */
std::vector<DungeonPoly*> StageCollision::SetupCollision(
	GameObject* other,
	Vector3 MoveVec) {
	std::vector<DungeonPoly*> result;

	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return result;

	// カプセルのAABB作成
	Vector3 start = other->position + capsule->capsule.segment.startPoint;
	Vector3 end = other->position + capsule->capsule.segment.endPoint;

	Vector3 min = Min(start, end) - Vector3::one * capsule->capsule.radius;
	Vector3 max = Max(start, end) + Vector3::one * capsule->capsule.radius;

	// 八分木検索
	return GetPolygonsFromTree(min, max);
}

/*
 * @brief AABB同士が重なっているか判定する
 * @param minA AABB Aの最小座標
 * @param maxA AABB Aの最大座標
 * @param minB AABB Bの最小座標
 * @param maxB AABB Bの最大座標
 * @return true 重なっている
 * @return false 重なっていない
 */
bool StageCollision::AABBOverlap(
	const Vector3& minA,
	const Vector3& maxA,
	const Vector3& minB,
	const Vector3& maxB) {
	// 各軸で分離していたら衝突していない
	if (maxA.x < minB.x || minA.x > maxB.x) return false;
	if (maxA.y < minB.y || minA.y > maxB.y) return false;
	if (maxA.z < minB.z || minA.z > maxB.z) return false;

	// 全軸で分離していなければ衝突
	return true;
}

std::vector<DungeonPoly*> StageCollision::GetPolygonsFromTree(const Vector3& min, const Vector3& max) {
	std::vector<DungeonPoly*> result;

	// 衝突候補取得
	std::vector<DungeonPoly*> pairs;
	m_tree.GetAllCollisionList(pairs);

	// 範囲内ポリゴンのみ抽出
	for (auto* poly : pairs) {
		if (AABBOverlap(min, max, poly->min, poly->max)) {
			result.push_back(poly);
		}
	}

	return result;
}

/*
 * @brief 壁ポリゴン／床ポリゴンの振り分け
 * @param hitDim   コリジョン結果構造体
 * @param walls    壁ポリゴン格納先
 * @param floors   床ポリゴン格納先
 */
void StageCollision::ClassifyPolygons(
	const std::vector<DungeonPoly*>& polys,
	std::vector<DungeonPoly*>& walls,
	std::vector<DungeonPoly*>& floors
) {
	for (auto* p : polys) {
		float ny = p->poly.Normal.y;

		if (ny >= _FLOOR_LIMIT)
			floors.push_back(p);

		if (ny < _POLYGON_HEIGHT)
			walls.push_back(p);
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
	Vector3 prevPos,
	float polyOffset,
	Vector3 MoveVec,
	const std::vector<DungeonPoly*>& walls,
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
		Vector3 p0 = FromVECTOR(poly->poly.Position[0]);
		Vector3 p1 = FromVECTOR(poly->poly.Position[1]);
		Vector3 p2 = FromVECTOR(poly->poly.Position[2]);


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

		//// 水平方向のみ押し出す
		//if (poly->HitPosition.y > prevPos.y + _POLYGON_HEIGHT) {
		//	pushDir.y = 0.0f;
		//
		//}

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

		//// ポリゴンの法線を足していく
		//for (auto* poly : walls) {
		//	// 法線をVector3型に直し、乗算
		//	avgNormal += FromVECTOR(poly->Normal);
		//	count++;
		//}
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
	const std::vector<DungeonPoly*>& floors,
	GameObject* other,
	Vector3 moveVec
) {
	// 床がなければ落下
	if (floors.empty()) {
		auto gravity = other->GetComponent<GravityComponent>();
		if (gravity) {
			gravity->SetGroundingFrag(false);
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
		Vector3 p0 = FromVECTOR(poly->poly.Position[0]);
		Vector3 p1 = FromVECTOR(poly->poly.Position[1]);
		Vector3 p2 = FromVECTOR(poly->poly.Position[2]);

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


// /*
//  *	ステージの当たり判定の描画
//  */
// void StageCollision::StageColliderRenderer(GameObject* other, Vector3 MoveVec, Vector3 prevPos) {
// 	// モデルハンドルが無効な場合は処理を中止
// 	if (modelHandle < 0) return;
// 
// 	auto hitDim = SetupCollision(other, MoveVec);
// 
// 	// 壁と床を区別するための色情報を設定
// 	unsigned int wallColor = GetColor(255, 100, 100);
// 	unsigned int floorColor = GetColor(100, 255, 100);
// 	unsigned int lineColor = GetColor(255, 255, 255);
// 
// 	// 当たった全てのポリゴンに対して描画処理を実行
// 	for (int i = 0; i < hitDim->HitNum; i++) {
// 		const auto& poly = hitDim->Dim[i];
// 
// 		// 壁
// 		bool isWall = (poly.Normal.y < _POLYGON_HEIGHT);
// 		// 床
// 		bool isFloor = (poly.Normal.y >= _FLOOR_LIMIT);
// 		unsigned int drawColor;
// 
// 		// 壁かどうか
// 		if (isFloor) {
// 			// 壁ポリゴン配列に追加する
// 			drawColor = floorColor;
// 		}
// 
// 		if (isWall) {
// 			// 床ポリゴン配列に追加する
// 			drawColor = wallColor;
// 		}
// 
// 		// ポリゴンを半透明で描画
// 		DrawTriangle3D(poly.Position[0], poly.Position[1], poly.Position[2], drawColor, TRUE);
// 
// 		// ポリゴンを白線で描画
// 		DrawLine3D(poly.Position[0], poly.Position[1], lineColor);
// 		DrawLine3D(poly.Position[1], poly.Position[2], lineColor);
// 		DrawLine3D(poly.Position[2], poly.Position[0], lineColor);
// 	}
// 
// 	// 使用した衝突判定データを解放
// 	MV1CollResultPolyDimTerminate(*hitDim);
// }