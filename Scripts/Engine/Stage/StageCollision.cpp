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
void StageCollision::BuildFromTriangles(
	const std::vector<Triangle>& triangles) {
	// ステージ全体AABB計算
	Vector3 stageMin(FLT_MAX, FLT_MAX, FLT_MAX);
	Vector3 stageMax(-FLT_MAX, -FLT_MAX, -FLT_MAX);

	for (const auto& tri : triangles) {
		stageMin = Min(stageMin, Min(Min(tri.p0, tri.p1), tri.p2));
		stageMax = Max(stageMax, Max(Max(tri.p0, tri.p1), tri.p2));
	}

	m_tree.Init(6, stageMin, stageMax);

	for (auto tri : triangles) {
		tri.ComputeNormal();

		auto poly = std::make_unique<DungeonPoly>();
		poly->tri = tri;

		poly->min = Min(Min(tri.p0, tri.p1), tri.p2);
		poly->max = Max(Max(tri.p0, tri.p1), tri.p2);

		auto node = std::make_unique<Object_For_Tree<DungeonPoly>>();
		node->pObject = poly.get();

		m_tree.Regist(&poly->min, &poly->max, node.get());

		m_stagePolys.push_back(std::move(poly));
		m_treeNodes.push_back(std::move(node));
	}
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

std::vector<DungeonPoly*> StageCollision::GetPolygonsFromTree(
	const Vector3& min,
	const Vector3& max) {
	std::vector<DungeonPoly*> result;
	m_tree.GetObjectsInAABB(min, max, result);
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
		float ny = p->tri.normal.y;

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
	float,
	Vector3 moveVec,
	const std::vector<DungeonPoly*>& walls,
	bool moveFlag,
	GameObject* other
) {
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return;

	float radius = capsule->capsule.radius;

	Vector3 capStart = nowPos + capsule->capsule.segment.startPoint;
	Vector3 capEnd = nowPos + capsule->capsule.segment.endPoint;
	Vector3 capCenter = (capStart + capEnd) * 0.5f;

	for (auto* poly : walls) {
		Vector3 nearest = Nearest(
			capCenter,
			poly->tri.p0,
			poly->tri.p1,
			poly->tri.p2);

		Vector3 diff = capCenter - nearest;
		float dist = Magnitude(diff);

		float penetration = radius - dist;
		if (penetration <= 0.0f) continue;

		Vector3 pushDir = Normalized(diff);

		nowPos += pushDir * penetration;

		capStart = nowPos + capsule->capsule.segment.startPoint;
		capEnd = nowPos + capsule->capsule.segment.endPoint;
		capCenter = (capStart + capEnd) * 0.5f;
	}

	// --- スライド処理 ---
	if (moveFlag) {
		Vector3 avgNormal = Vector3::zero;
		int count = 0;

		for (auto* poly : walls) {
			avgNormal += poly->tri.normal;
			count++;
		}

		if (count > 0) {
			avgNormal = (avgNormal / (float)count).Normalized();
			float dot = Dot(moveVec, avgNormal);

			if (dot < 0.0f) {
				Vector3 slideVec = moveVec - avgNormal * dot;
				nowPos = prevPos + slideVec;
			}
		}
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
	auto gravity = other->GetComponent<GravityComponent>();
	auto capsule = other->GetComponent<CapsuleCollider>();

	if (!gravity || !capsule) return;

	if (floors.empty()) {
		gravity->SetGroundingFrag(false);
		return;
	}

	float maxY = -FLT_MAX;
	bool grounded = false;

	Vector3 capStart = nowPos + capsule->capsule.segment.startPoint;
	float radius = capsule->capsule.radius;

	for (auto* poly : floors) {
		Vector3 nearest = Nearest(
			capStart,
			poly->tri.p0,
			poly->tri.p1,
			poly->tri.p2);

		float dist = Distance(capStart, nearest);

		if (dist <= radius) {
			if (nearest.y > maxY) {
				maxY = nearest.y;
				grounded = true;
			}
		}
	}

	if (grounded) {
		nowPos.y = maxY + (radius - capsule->capsule.segment.startPoint.y);
		gravity->SetGroundingFrag(true);
	}
	else {
		gravity->SetGroundingFrag(false);
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