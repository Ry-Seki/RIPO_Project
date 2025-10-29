/*
 *	@file	Collision.h
 *	@author Riku
 */

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "VecMath.h"

/*
 *	軸並行境界ボックス
 */
struct AABB {
	Vector3 min;
	Vector3 max;
};

// AABB同士で衝突しているかどうか
bool BoxIntersect(const AABB& a, const AABB& b, Vector3& penetration);

#endif // !_COLLISION_H_

