/*
 *	@file	RayCast.h
 *	@author	Riku
 */

#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "Collision.h"

struct Ray {
	Vector3 start;	// 開始点
	Vector3 dir;	// 方向ベクトル
};

// レイが衝突しているかどうか
bool RayIntersect(const Ray& ray, const AABB& box, float& hitLength);

#endif // !_RAYCAST_H_
