/*
 *	@file	RayCast.h
 *	@author	Riku
 */

#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "Collision.h"
#include "Engine.h"

struct Ray {
	Vector3 start;	// 開始点
	Vector3 dir;	// 方向ベクトル
};
// レイキャスト
bool RayCast(Engine* engine, Vector3 startPosition, Vector3 direction, float& hitLength);

// レイが衝突しているかどうか
bool RayIntersect(const Ray& ray, const AABB& box, float& hitLength);

#endif // !_RAYCAST_H_
