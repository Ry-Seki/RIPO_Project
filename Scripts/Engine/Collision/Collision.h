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
 *	線分
 */
struct Segment {
	Vector3 startPoint;
	Vector3 endPoint;
};

/*
 *	カプセル
 */
struct Capsule {
	Segment segment;
	float radius;
};

// 衝突しているかどうか
bool Intersect(const AABB& a, const AABB& b, Vector3& penetration);
bool Intersect(const Capsule& a, const Capsule& b, Vector3& penetration);
// 点から線分への最近接点
Vector3 PointToSegmentMinLength(const Vector3& startPos, const Vector3& endPos, const Vector3& point, float& minRatio);
// 線分から線分への最近接点
void SegmentBetweenMinLength(const Segment& a, const Segment& b, Vector3& aMinPoint, Vector3& bMinPoint, float& aMinRatio, float& bMinRatio);


#endif // !_COLLISION_H_

