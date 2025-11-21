/*
 *	@file	Collision.h
 *	@author Riku
 */

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "../VecMath.h"

// 線分を点とする基準数(指数表記)
const float EPSILON_SEGMENT = 1e-8f;

/*
 *	軸並行境界ボックス
 */
struct AABB {
	Vector3 min;
	Vector3 max;
};

/*
 *	カプセル
 */
struct Capsule {
	Vector3 startPoint;
	Vector3 endPoint;
	float radius;
};

// 衝突しているかどうか
bool Intersect(const AABB& a, const AABB& b, Vector3& penetration);
bool Intersect(const Capsule& a, const Capsule& b, Vector3& penetration);
#endif // !_COLLISION_H_

