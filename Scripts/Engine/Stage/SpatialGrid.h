/*
 *	@file SpatialGrid.h
 *	@author oorui
 */

#ifndef _SPATOAL_GRID_H
#define _SPATOAL_GRID_H

#include <unordered_map>
#include <vector>
#include "../VecMath.h"
#include "../GameObject.h"

 // 前方宣言
class GameObject;

/*
 *	グリッド座標
 */
struct GridCoord
{
	Vector3 grid;

	// グリッド用演算子
	bool operator==(const GridCoord& other) const {
		return
			grid.x == other.grid.x &&
			grid.y == other.grid.y &&
			grid.z == other.grid.z;
			
	}
};

/*
 *	GridCoordハッシュ
 */
struct GridCoordHash
{
	std::size_t operator()(const GridCoord& coord) const {
		return
			std::hash<int>()((int)coord.grid.x) ^
			(std::hash<int>()((int)coord.grid.y) << 1) ^
			(std::hash<int>()((int)coord.grid.z) << 2);
	}
};

/*
 *	空間分割グリッドクラス
 */
class SpatialGrid
{
private:

	// グリッドデータ
	std::unordered_map<GridCoord, std::vector<GameObject*>, GridCoordHash> grid;

public:

	/*
	 *	ワールド座標からグリッド座標に変換
	 *  @param[in]	pos		変換対象の座標
	 */
	GridCoord WorldToGrid(const Vector3& pos);

	/*
	 *	三角形がグリッドセル内に存在するか判定する
	 *  @param[in]	Vector3		三角形の頂点
	 *  @param[in]	GridCoord	判定するセル
	 *  @return		セル内に三角形が存在するか
	 */
	bool IsTriangleInCell(
		const Vector3& p0,
		const Vector3& p1,
		const Vector3& p2,
		const GridCoord& cell
	);

	/*
	 *	周囲のセルを取得
	 *  @param[in]	other	セル内にいるキャラクター
	 */
	std::unique_ptr<MV1_COLL_RESULT_POLY_DIM>SetupDebugCollision(int modelHandle, GameObject* other);

public:
	/*
	 *	グリッドを表示
	 *  @param[in]	player	参照するプレイヤー
	 */
	void DrawGrid(GameObject* player);


	/*
	 *	ステージの当たり判定の描画
	 *  @param	GameObject* キャラクター
	 *  @param	Vector3		移動量
	 *  @param	Vector3		直前の移動量
	 */
	void StageColliderGridRenderer(int modelHandle, GameObject* other, Vector3 MoveVec, Vector3 prevPos);


public:
	/*
	 *	グリッドをクリアする
	 */
	void Clear() {
		grid.clear();
	};

};

#endif // !_SPATOAL_GRID_H