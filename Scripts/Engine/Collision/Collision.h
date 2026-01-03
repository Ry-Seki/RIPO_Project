/*
 *	@file	Collision.h
 *	@author Riku
 */

#ifndef _COLLISION_H_
#define _COLLISION_H_

#include "../VecMath.h"
#include <variant>

// 限りなく0に近いしきい値(指数表記)
const float EPSILON = 1e-6f;

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

// 点から線分への最近接点
Vector3 PointToSegmentMinLength(const Vector3& point, const Segment& segment, float& minRatio);
// 点からAABBへの最近接点
Vector3 PointToAABBMinLength(const Vector3& point, const AABB& box);
// 線分から線分への最近接点
void SegmentBetweenMinLength(const Segment& a, const Segment& b, Vector3& aMinPoint, Vector3& bMinPoint, float& aMinRatio, float& bMinRatio);
// 線分からAABBへの最近接点
void SegmentToAABBMinLength(const Segment& segment, const AABB& box, Vector3& segMinPoint, Vector3& boxMinPoint, float& minLengthSq);
// 衝突しているかどうか
bool Intersect(const Capsule& a, const Capsule& b, Vector3& penetration);
bool Intersect(const AABB& a, const AABB& b, Vector3& penetration);
bool Intersect(const Capsule& capsule, const AABB& box, Vector3& penetration);
bool Intersect(const AABB& box, const Capsule& capsule , Vector3& penetration);
bool Intersect(const std::variant<AABB, Capsule>& a, const std::variant<AABB, Capsule>& b, Vector3& penetration);


#endif // !_COLLISION_H_

