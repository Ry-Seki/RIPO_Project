/*
 *	@file	AABBCollider.cpp
 *  @author Riku
 */

#include "AABBCollider.h"
#include "../GameObject.h"

/*
 *	更新処理
 */
void AABBCollider::Update(float deltaTime) {
}

/*
 *	衝突が起きたときに呼び出される処理
 *	@param	const std::shared_ptr<Component&> self	衝突した自分のコライダー
 *  @param	const std::shared_ptr<Component&> other	衝突した相手のコライダー
 */
void AABBCollider::OnCollision(
	const std::shared_ptr<Component>& self,
	const std::shared_ptr<Component>& other) {
	isHit = true;
}

/*
 *	デバッグ用の描画関数
 */
void AABBCollider::DebugRender() {
	GameObject* owner = GetOwner();
	VECTOR aabbMin = ToVECTOR(aabb.min + owner->position);
	VECTOR aabbMax = ToVECTOR(aabb.max + owner->position);
	std::vector<VECTOR> aabbPoits = {
		aabbMin,
		{ aabbMin.x, aabbMin.y, aabbMax.z },
		{ aabbMax.x, aabbMin.y, aabbMin.z },
		{ aabbMax.x, aabbMin.y, aabbMax.z },
		{ aabbMin.x, aabbMax.y, aabbMax.z },
		aabbMax,
		{ aabbMax.x, aabbMax.y, aabbMin.z },
		{ aabbMin.x, aabbMax.y, aabbMin.z }
	};

	VECTOR color;
	if (isHit) {
		color = { 0, 128, 0 };
	}
	else {
		color = { 255, 255, 255 };
	}

	DrawLine3D(aabbPoits[0], aabbPoits[1], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[0], aabbPoits[2], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[3], aabbPoits[1], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[3], aabbPoits[2], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[0], aabbPoits[7], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[1], aabbPoits[4], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[2], aabbPoits[6], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[3], aabbPoits[5], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[7], aabbPoits[4], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[7], aabbPoits[6], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[5], aabbPoits[4], GetColor(color.x, color.y, color.z));
	DrawLine3D(aabbPoits[5], aabbPoits[6], GetColor(color.x, color.y, color.z));

}
