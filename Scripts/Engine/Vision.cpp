/*
 *	@file	Vision.cpp
 *  @author kuu
 */
#include "Vision.h"
#include "Scene/Scene.h"
#include "Manager/CameraManager.h"


/*
 *	視界処理
 *	param[in]	const GameObjectPtr beholder自身のposition
 *	param[in]	const Vector3& direction	視界の方向ベクトル
 *	param[in]	const Vector3& targetPos	目標のposition
 *	param[in]	float viewAngle				視野角の半角
 *	param[in]	float viewDistance			視界の距離
 */
bool Vision(const GameObject* beholder, const Vector3& direction, const Vector3& targetPos, float viewAngle, float viewDistance) {
	// 壁判定RayCast
	Scene::RayCastHit hitInfo;
	auto engine = beholder->GetEngine();
	Ray ray = { beholder->position, Direction(beholder->position, targetPos) };
	bool hit = engine->GetCurrentScene()->RayCast(
		ray, hitInfo,
		[beholder, targetPos](const ColliderBasePtr& col, float distance) {
			// プレイヤーと自分以外のオブジェクト
			return !col &&
				distance < Distance(beholder->position, targetPos);
		}
	);
	if (hit)
		return false;

	// 自身と目標の距離
	float distance = Distance(targetPos, beholder->position);
	// 視界距離判定
	if (distance > viewDistance) return false;
	// 目標の方向
	Vector3 toTargetDir = Direction(beholder->position, targetPos);
	// 視野角の計算
	float dot = Clamp(Dot(direction, toTargetDir), -1.0f, 1.0f);
	float angle = acosf(dot);
	viewAngle = viewAngle * Deg2Rad;
	if (angle > viewAngle) {
		return false;
	}
	// 当たっている
	return true;
}