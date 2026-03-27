/*
 *	@file	SpatialGrid.cpp
 *  @author oorui
 */

#include "SpatialGrid.h"

namespace {
	constexpr float _POLYGON_HEIGHT = 0.9f;				// 壁の角度
	constexpr float _FLOOR_LIMIT = 0.5f;				// 床の角度
	// グリッドサイズ
	constexpr float GRID_SIZE = 2000.0f;
}

 /*
  *	ワールド座標からグリッド座標に変換
  *  @param[in]	pos		変換対象の座標
  */
GridCoord SpatialGrid::WorldToGrid(const Vector3& pos) {
	return {
	(int)floor(pos.x / GRID_SIZE),  // X方向のセル
	(int)floor(pos.z / GRID_SIZE)   // Z方向のセル
	};
}

/*
 *	三角形がグリッドセル内に存在するか判定する
 *  @param[in]	Vector3		三角形の頂点
 *  @param[in]	GridCoord	判定するセル
 *  @return		セル内に三角形が存在するか
 */
bool SpatialGrid::IsTriangleInCell(const Vector3& p0, const Vector3& p1, const Vector3& p2, const GridCoord& cell) {
	// 各頂点のグリッドを取得
	GridCoord c0 = WorldToGrid(p0);
	GridCoord c1 = WorldToGrid(p1);
	GridCoord c2 = WorldToGrid(p2);

	// 頂点がセルに含まれているか判定
	bool inside =
		(c0 == cell || c1 == cell || c2 == cell);

	// 三角形のAABBを作成
	Vector3 triMin, triMax;

	// 三角形の最小値を求める
	triMin.x = min(p0.x, min(p1.x, p2.x));
	triMin.y = min(p0.y, min(p1.y, p2.y));
	triMin.z = min(p0.z, min(p1.z, p2.z));

	// 三角形の最大値を求める
	triMax.x = max(p0.x, max(p1.x, p2.x));
	triMax.y = max(p0.y, max(p1.y, p2.y));
	triMax.z = max(p0.z, max(p1.z, p2.z));

	// セルのAABBを作成
	Vector3 cellMin;
	cellMin.x = cell.x * GRID_SIZE;
	cellMin.y = -FLT_MAX;  // Y方向は無限として扱う
	cellMin.z = cell.z * GRID_SIZE;

	Vector3 cellMax;
	cellMax.x = cellMin.x + GRID_SIZE;
	cellMax.y = FLT_MAX;
	cellMax.z = cellMin.z + GRID_SIZE;

	// AABB同士の衝突判定
	bool overlap =
		(triMin.x <= cellMax.x && triMax.x >= cellMin.x) &&
		(triMin.z <= cellMax.z && triMax.z >= cellMin.z);

	// 頂点またはAABBが重なっていればtrue
	return (inside || overlap);
	
}

/*
 *	グリッドを表示
 *  @param[in]	player	参照するプレイヤー
 */
void SpatialGrid::DrawGrid(GameObject* player) {
	// プレイヤーの座標
	Vector3 playerPos = player->position;
	// プレイヤーのいるセルを取得
	GridCoord playerCell = WorldToGrid(playerPos);

	// 描画を周囲10マスに絞る
	const int RANGE = 10;

	for (int z = -RANGE; z <= RANGE; z++) {
		for (int x = -RANGE; x <= RANGE; x++) {
			GridCoord coord{ playerCell.x + x, playerCell.z + z };

			float worldX = coord.x * GRID_SIZE;
			float worldZ = coord.z * GRID_SIZE;

			// グリッドに登録されているか
			bool hasObject = (grid.find(coord) != grid.end());

			// プレイヤーセル
			bool isPlayer = (coord.x == playerCell.x && coord.z == playerCell.z);

			unsigned int color;

			if (isPlayer)
				color = GetColor(255, 0, 0);
			else if (hasObject)
				color = GetColor(0, 255, 0);
			else
				color = GetColor(100, 100, 100);

			float y = playerPos.y - 1.0f;

			VECTOR p1 = VGet(worldX, y, worldZ);
			VECTOR p2 = VGet(worldX + GRID_SIZE, y, worldZ);
			VECTOR p3 = VGet(worldX + GRID_SIZE, y, worldZ + GRID_SIZE);
			VECTOR p4 = VGet(worldX, y, worldZ + GRID_SIZE);

			DrawLine3D(p1, p2, color);
			DrawLine3D(p2, p3, color);
			DrawLine3D(p3, p4, color);
			DrawLine3D(p4, p1, color);

			// 赤くする
			//DrawTriangle3D(p1, p2, p3, color, TRUE);
			//DrawTriangle3D(p1, p3, p4, color, TRUE);
		}
	}
}

/*
 *	周囲のセルを取得
 *  @param[in]	other	セル内にいるキャラクター
 */
std::unique_ptr<MV1_COLL_RESULT_POLY_DIM> SpatialGrid::SetupDebugCollision(int modelHandle, GameObject* other) {
	// カプセルを取得
	auto capsule = other->GetComponent<CapsuleCollider>();
	if (!capsule) return std::make_unique<MV1_COLL_RESULT_POLY_DIM>();
	// プレイヤーの座標を設定
	Vector3 pos = other->position;

	// セルサイズ分の球で取得
	float radius = GRID_SIZE;

	// ポリゴン構造体作成
	auto hitDim = std::make_unique<MV1_COLL_RESULT_POLY_DIM>();

	// 球とポリゴンの判定を取る
	*hitDim = MV1CollCheck_Sphere(
		modelHandle,		// ダンジョンモデル
		-1,
		ToVECTOR(pos),		// プレイヤーの座標をVECTOR型に変換して渡す
		radius
	);

	// 当たったポリゴンを返す
	return hitDim;

}



/*
 *	ステージの当たり判定の描画
 *  @param	GameObject* キャラクター
 *  @param	Vector3		移動量
 *  @param	Vector3		直前の移動量
 */
void SpatialGrid::StageColliderGridRenderer(int modelHandle,GameObject* other, Vector3 MoveVec, Vector3 prevPos) {

	// 周囲のセルを取得
	auto hitDim = SetupDebugCollision(modelHandle,other);

	// プレイヤーのいるセルを取得
	GridCoord playerCell = WorldToGrid(other->position);

	// 壁と床を区別するための色情報を設定
	unsigned int wallColor = GetColor(255, 100, 100);
	unsigned int floorColor = GetColor(100, 255, 100);
	unsigned int lineColor = GetColor(255, 255, 255);

	// 当たった全てのポリゴンに対して描画処理を実行
	for (int i = 0; i < hitDim->HitNum; i++) {
		const auto& poly = hitDim->Dim[i];

		// 三角形の頂点取得
		Vector3 p0 = FromVECTOR(poly.Position[0]);
		Vector3 p1 = FromVECTOR(poly.Position[1]);
		Vector3 p2 = FromVECTOR(poly.Position[2]);

		// 各頂点のグリッドを取得
		GridCoord c0 = WorldToGrid(p0);
		GridCoord c1 = WorldToGrid(p1);
		GridCoord c2 = WorldToGrid(p2);

		// 三角形がプレイヤーセルに含まれていなければスキップ
		if (!IsTriangleInCell(p0, p1, p2, playerCell)) continue;

		// 壁
		bool isWall = (poly.Normal.y < _POLYGON_HEIGHT);
		// 床
		bool isFloor = (poly.Normal.y >= _FLOOR_LIMIT);

		unsigned int drawColor;

		// 床
		if (isFloor) {
			drawColor = floorColor;
		}

		// 壁
		if (isWall) {
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