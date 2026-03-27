/*
 *	@file	SpatialGrid.cpp
 *  @author oorui
 */

#include "SpatialGrid.h"
#include "../GameConst.h"

namespace {
	constexpr float _POLYGON_HEIGHT = 0.9f;				// 壁の角度
	constexpr float _FLOOR_LIMIT = 0.5f;				// 床の角度
	constexpr float GRID_SIZE = 2000.0f;				// グリッドサイズ
	/*
	 * 立方体グリッドをワイヤーフレームで描画する
	 * @param[in] worldX  ワールド座標X
	 * @param[in] worldY  ワールド座標Y
	 * @param[in] worldZ  ワールド座標Z
	 * @param[in] color   描画色
	 */
	void DrawGridCube(float worldX, float worldY, float worldZ, unsigned int color) {

		// 立方体の8頂点を作成

		// 下の四角形の頂点
		VECTOR p000 = VGet(worldX, worldY, worldZ);
		VECTOR p100 = VGet(worldX + GRID_SIZE, worldY, worldZ);
		VECTOR p110 = VGet(worldX + GRID_SIZE, worldY, worldZ + GRID_SIZE);
		VECTOR p010 = VGet(worldX, worldY, worldZ + GRID_SIZE);

		// 上の四角形の頂点
		VECTOR p001 = VGet(worldX, worldY + GRID_SIZE, worldZ);
		VECTOR p101 = VGet(worldX + GRID_SIZE, worldY + GRID_SIZE, worldZ);
		VECTOR p111 = VGet(worldX + GRID_SIZE, worldY + GRID_SIZE, worldZ + GRID_SIZE);
		VECTOR p011 = VGet(worldX, worldY + GRID_SIZE, worldZ + GRID_SIZE);

		// 下の面
		DrawLine3D(p000, p100, color);
		DrawLine3D(p100, p110, color);
		DrawLine3D(p110, p010, color);
		DrawLine3D(p010, p000, color);

		// 上の面
		DrawLine3D(p001, p101, color);
		DrawLine3D(p101, p111, color);
		DrawLine3D(p111, p011, color);
		DrawLine3D(p011, p001, color);

		// 縦線
		DrawLine3D(p000, p001, color);
		DrawLine3D(p100, p101, color);
		DrawLine3D(p110, p111, color);
		DrawLine3D(p010, p011, color);
	}

}

/*
 *	ワールド座標からグリッド座標に変換
 *  @param[in]	pos		変換対象の座標
 */
GridCoord SpatialGrid::WorldToGrid(const Vector3& pos) {
	// ワールド座標をグリッド座標へ変換
	return {
		Vector3(
			(int)floor(pos.x / GRID_SIZE),  // X方向
			(int)floor(pos.y / GRID_SIZE),  // Y方向
			(int)floor(pos.z / GRID_SIZE)   // Z方向
		)
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
	triMin = Min(p0, Min(p1, p2));
	// 三角形の最大値を求める
	triMax = Max(p0, Max(p1, p2));

	// セルのAABBを作成
	Vector3 cellMin = cell.grid * GRID_SIZE;

	// セルの最大値を計算
	Vector3 cellMax = Vector3(
		cellMin.x + GRID_SIZE,
		cellMin.y + GRID_SIZE,
		cellMin.z + GRID_SIZE
	);

	// AABB同士の衝突判定
	bool overlap =
		(triMin.x <= cellMax.x && triMax.x >= cellMin.x) &&
		(triMin.y <= cellMax.y && triMax.y >= cellMin.y) &&
		(triMin.z <= cellMax.z && triMax.z >= cellMin.z);

	// 頂点またはAABBが重なっていればtrue
	return (inside || overlap);

}

/*
 *	グリッドを表示
 *  @param[in]	player	参照するプレイヤー
 */
void SpatialGrid::DrawGrid(GameObject* player) {
	// プレイヤーの座標を取得
	Vector3 playerPos = player->position;

	// プレイヤーのいるセルを取得
	GridCoord playerCell = WorldToGrid(playerPos);

	// 描画範囲
	const int RANGE = 2;

	// Z方向の範囲ループ
	for (int z = -RANGE; z <= RANGE; z++) {

		// X方向の範囲ループ
		for (int x = -RANGE; x <= RANGE; x++) {

			// Y方向の範囲ループ
			for (int y = -RANGE; y <= RANGE; y++) {

				// 描画するグリッド座標を計算
				// プレイヤーのセルを中心として周囲のセルを取得
				GridCoord coord{
					Vector3(
						playerCell.grid.x + x,
						playerCell.grid.y + y,
						playerCell.grid.z + z
					)
				};

				// グリッド座標をワールド座標に変換
				Vector3 worldPos = coord.grid * GRID_SIZE;

				// このセルにオブジェクトが登録されているか確認
				bool hasObject = (grid.find(coord) != grid.end());

				// プレイヤーがいるセルか判定
				bool isPlayer = (
					coord.grid.x == playerCell.grid.x &&
					coord.grid.y == playerCell.grid.y &&
					coord.grid.z == playerCell.grid.z
					);

				// 描画色
				unsigned int color;

				// プレイヤーセルは赤
				if (isPlayer)
					color = GetColor(255, 0, 0);

				// オブジェクトがあるセルは緑
				else if (hasObject)
					color = GetColor(0, 255, 0);

				// 何もないセルは灰色
				else
					color = GetColor(100, 100, 100);


				// 立方体の8頂点を作成
				DrawGridCube(worldPos.x, worldPos.y, worldPos.z, color);
			}
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
void SpatialGrid::StageColliderGridRenderer(int modelHandle, GameObject* other, Vector3 MoveVec, Vector3 prevPos) {

	// 周囲のセルを取得
	auto hitDim = SetupDebugCollision(modelHandle, other);

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