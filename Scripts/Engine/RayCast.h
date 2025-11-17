/*
 *	@file	RayCast.h
 *	@author	Riku
 */

#ifndef _RAYCAST_H_
#define _RAYCAST_H_

#include "Collision/Collision.h"
#include "Engine.h"

struct Ray {
	Vector3 start;	// 開始点
	Vector3 dir;	// 方向ベクトル
};

/*
 *  レイキャスト
 *  @param  Engine*         engine
 *  @param  Vector3         startPosition   レイ開始位置
 *  @param  Vector3         direction       レイの方向
 *  @param  float&          hitLength       レイが当たるまでの距離
 *  @param  GameObject*&    hitObject       当たったオブジェクト
 */
bool RayCast(Engine* engine, Vector3 startPosition, Vector3 direction, float& hitLength, GameObject*& hitObject);

/*
 *  RayとAABBの交差判定
 *  @param  Ray&            ray         判定対象1
 *  @param  AABB&           box         判定対象2
 *  @param  float&          hitLength   当たるまでの距離
 *  @param  GameObject*&    hitObject   当たった最も近いオブジェクト
 *  @param  GameObject*     checkObject 判定チェックオブジェクト
 *  @param  bool            isHit       当たったかどうか
 */
bool RayIntersect(const Ray& ray, const AABB& box, float& hitLength, GameObject*& hitObject, GameObject* checkObject, bool isHit);

/*
 *  各軸の交差判定
 *  @param  float   boxMin  AABBの各軸の最小位置
 *  @param  float   boxMax  AABBの各軸の最大位置
 *  @param  float   rayPos  レイの各軸の開始位置
 *  @param  float   rayDir  レイの各軸の方向ベクトル
 *  @param  float&  dMin    最初に当たるまでの距離
 *  @param  float&  dMax    最後に当たるまでの距離
 */
bool IntersectCheck(float boxMin, float boxMax, float rayPos, float rayDir, float& dMin, float& dMax);

#endif // !_RAYCAST_H_
