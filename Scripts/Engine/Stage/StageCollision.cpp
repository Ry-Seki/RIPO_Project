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
	BuildSpatialGrid();
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
	ClassifyPolygons(hitDim.get(), walls, floors, prevPos);

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
	// 判定結果格納用
	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();
	hitDim->HitNum = 0;

	// 対象が無い場合は空で返す
	if (!other) return hitDim;

	// モデル未ロードなら空で返す
	if (modelHandle < 0) return hitDim;

	// カプセル取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return hitDim;

	Vector3 effectiveMove = MoveVec;

	// 極小移動ならゼロ扱い
	if (Magnitude(effectiveMove) < _CHARACTER_MOVEVEC_MIN) {
		effectiveMove = Vector3::zero;
	}

	float moveLen = effectiveMove.Magnitude();

	Vector3 forward = Vector3::zero;

	// 移動方向を正規化
	if (moveLen > 0.0f) {
		forward = Normalized(effectiveMove);
	}

	Vector3 capLower = other->position + capsule->capsule.segment.startPoint;
	Vector3 capTop = other->position + capsule->capsule.segment.endPoint;
	Vector3 capCenter = (capLower + capTop) * _HALF;

	float radius = capsule->capsule.radius;

	// 移動を考慮したスイープ球
	Vector3 sphereCenter = capCenter + forward * (moveLen * _HALF);
	float sphereRadius = radius + moveLen * _HALF;


	std::vector<MV1_COLL_RESULT_POLY*> nearbyPolys =
		QueryNearbyPolygons(sphereCenter, sphereRadius);

	if (nearbyPolys.empty())
		return hitDim;

	for (auto* poly : nearbyPolys) {
		// 三角形頂点取得
		Vector3 p0 = FromVECTOR(poly->Position[0]);
		Vector3 p1 = FromVECTOR(poly->Position[1]);
		Vector3 p2 = FromVECTOR(poly->Position[2]);

		// 球中心と三角形の最近接点を求める
		Vector3 nearest = Nearest(sphereCenter, p0, p1, p2);

		// 中心との差分
		Vector3 diff = sphereCenter - nearest;

		float dist = Magnitude(diff);

		// 半径以内なら衝突
		if (dist <= sphereRadius) {
			if (hitDim->HitNum < 256) {
				// 結果配列に追加
				hitDim->Dim[hitDim->HitNum] = *poly;
				hitDim->HitNum++;
			}

			//if (hitDim->HitNum < MV1_COLL_RESULT_POLY::) {
			//	hitDim->Dim[hitDim->HitNum++] = *poly;
			//}
		}
	}
	// printfDx("near poly = %d\n", nearbyPolys.size());
	return hitDim;
}

/*
 * @brief 壁ポリゴン／床ポリゴンの振り分け
 * @param hitDim   コリジョン結果構造体
 * @param walls    壁ポリゴン格納先
 * @param floors   床ポリゴン格納先
 */
void StageCollision::ClassifyPolygons(
	const MV1_COLL_RESULT_POLY_DIM* hitDim,
	std::vector<MV1_COLL_RESULT_POLY*>& walls,
	std::vector<MV1_COLL_RESULT_POLY*>& floors,
	const Vector3& prevPos
) {

	// 全ポリゴンをチェック
	for (int i = 0; i < hitDim->HitNum; i++) {
		// ヒットしたポリゴンの配列
		auto& poly = hitDim->Dim[i];

		// 壁
		bool isWall = (poly.Normal.y < _POLYGON_HEIGHT);
		// 床
		bool isFloor = (poly.Normal.y >= _FLOOR_LIMIT);

		// 壁かどうか
		if (isFloor) {
			// 壁ポリゴン配列に追加する
			floors.push_back(&poly);
		}

		if (isWall) {
			// 床ポリゴン配列に追加する
			walls.push_back(&poly);
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
	Vector3 prevPos,
	float polyOffset,
	Vector3 MoveVec,
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

	Vector3 checkPos = capStart + Vector3(0.0f, capsuleRadius, 0.0f);


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

	printfDx("floor count = %d\n", floors.size());

}

/*
 *	近傍セルのみ取得
 */
std::vector<MV1_COLL_RESULT_POLY*> StageCollision::QueryNearbyPolygons(Vector3 center, float radius) {
	std::vector<MV1_COLL_RESULT_POLY*> result;

	// 球のAABB計算
	Vector3 min = center - Vector3(radius, radius, radius);
	Vector3 max = center + Vector3(radius, radius, radius);

	int minX = (int)floor(min.x / _GRID_SIZE);
	int maxX = (int)floor(max.x / _GRID_SIZE);
	int minY = (int)floor(min.y / _GRID_SIZE);
	int maxY = (int)floor(max.y / _GRID_SIZE);
	int minZ = (int)floor(min.z / _GRID_SIZE);
	int maxZ = (int)floor(max.z / _GRID_SIZE);

	// 該当セルのみチェック
	for (int x = minX; x <= maxX; ++x)
		for (int y = minY; y <= maxY; ++y)
			for (int z = minZ; z <= maxZ; ++z) {
				long long key = MakeKey(x, y, z);

				auto it = spatialGrid.find(key);
				if (it != spatialGrid.end()) {
					// セル内ポリゴンを追加
					result.insert(result.end(),
						it->second.polygons.begin(),
						it->second.polygons.end());
				}
			}

	return result;
}

void StageCollision::BuildSpatialGrid() {
	int meshIndex = MV1GetMeshNum(modelHandle);
	// ステージ全体のポリゴン数を取得
	int triangleList = 0;
	int listNum = MV1GetTriangleListNum(modelHandle);
	int totalPoly = 0;
	// まず総ポリゴン数を数える
	for (int list = 0; list < listNum; list++) {
		totalPoly += MV1GetTriangleListPolygonNum(modelHandle, list);
	}
	stagePolygons.reserve(totalPoly);
	for (int list = 0; list < listNum; list++) {

		int polyNum = MV1GetTriangleListPolygonNum(modelHandle, list);

		for (int i = 0; i < polyNum; ++i) {

			// 新しいポリゴンを追加
			stagePolygons.emplace_back();

			auto& poly = stagePolygons.back();

			VECTOR v[3];

			// 三角形頂点取得
			MV1GetTriangleListPolygonVertexPosition(
				modelHandle,
				list,
				i,
				v,
				NULL
			);

			poly.Position[0] = v[0];
			poly.Position[1] = v[1];
			poly.Position[2] = v[2];

			Vector3 p0 = FromVECTOR(v[0]);
			Vector3 p1 = FromVECTOR(v[1]);
			Vector3 p2 = FromVECTOR(v[2]);

			// AABB最小・最大計算
			Vector3 min = Min(Min(p0, p1), p2);
			Vector3 max = Max(Max(p0, p1), p2);

			// AABBが含まれるセル範囲を計算
			int minX = (int)floor(min.x / _GRID_SIZE);
			int maxX = (int)floor(max.x / _GRID_SIZE);
			int minY = (int)floor(min.y / _GRID_SIZE);
			int maxY = (int)floor(max.y / _GRID_SIZE);
			int minZ = (int)floor(min.z / _GRID_SIZE);
			int maxZ = (int)floor(max.z / _GRID_SIZE);

			// その範囲のセルすべてに登録
			for (int x = minX; x <= maxX; ++x)
				for (int y = minY; y <= maxY; ++y)
					for (int z = minZ; z <= maxZ; ++z) {
						long long key = MakeKey(x, y, z);

						// セルにポリゴン追加
						spatialGrid[key].polygons.push_back(&poly);
					}
		}
	}
	//printfDx("grid cell count = %d\n", spatialGrid.size());
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

/*
 * グリッドを描画する
 */
void StageCollision::DrawSpatialGrid() {
	unsigned int color = GetColor(0, 150, 255);

	// spatialGridの全セルを描画
	for (auto& cell : spatialGrid) {
		int x, y, z;

		// キーからグリッド座標取得
		DecodeKey(cell.first, x, y, z);

		// セルのワールド座標
		Vector3 minPos(
			x * _GRID_SIZE,
			y * _GRID_SIZE,
			z * _GRID_SIZE
		);

		Vector3 maxPos = minPos + Vector3(_GRID_SIZE, _GRID_SIZE, _GRID_SIZE);

		// VECTORに変換
		VECTOR v0 = VGet(minPos.x, minPos.y, minPos.z);
		VECTOR v1 = VGet(maxPos.x, minPos.y, minPos.z);
		VECTOR v2 = VGet(maxPos.x, minPos.y, maxPos.z);
		VECTOR v3 = VGet(minPos.x, minPos.y, maxPos.z);

		VECTOR v4 = VGet(minPos.x, maxPos.y, minPos.z);
		VECTOR v5 = VGet(maxPos.x, maxPos.y, minPos.z);
		VECTOR v6 = VGet(maxPos.x, maxPos.y, maxPos.z);
		VECTOR v7 = VGet(minPos.x, maxPos.y, maxPos.z);

		// 下
		DrawLine3D(v0, v1, color);
		DrawLine3D(v1, v2, color);
		DrawLine3D(v2, v3, color);
		DrawLine3D(v3, v0, color);

		// 上
		DrawLine3D(v4, v5, color);
		DrawLine3D(v5, v6, color);
		DrawLine3D(v6, v7, color);
		DrawLine3D(v7, v4, color);

		// 縦
		DrawLine3D(v0, v4, color);
		DrawLine3D(v1, v5, color);
		DrawLine3D(v2, v6, color);
		DrawLine3D(v3, v7, color);
	}
}