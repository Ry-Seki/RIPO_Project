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
	isHit = false;
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
	// hit判定によるカラー変更
	unsigned int color;
	if (isHit) {
		color = GetColor( 0, 128, 0 );
	}
	// 判定を取らない場合もカラー変更
	else if (!isCollider) {
		color = GetColor(128, 0, 0);
	}
	else {
		color = GetColor( 255, 255, 255 );
	}

	auto owner = GetOwner();
	// ワールド座標のAABB
	//VECTOR aabbMin = ToVECTOR(aabb.min + owner->position);
	//VECTOR aabbMax = ToVECTOR(aabb.max + owner->position);
	VECTOR aabbMin = ToVECTOR(Scale(aabb.min, owner->scale) + owner->position);
	VECTOR aabbMax = ToVECTOR(Scale(aabb.max, owner->scale) + owner->position);

	std::vector<VECTOR> aabbPoints = {
		aabbMin,
		{ aabbMin.x, aabbMin.y, aabbMax.z },
		{ aabbMax.x, aabbMin.y, aabbMin.z },
		{ aabbMax.x, aabbMin.y, aabbMax.z },
		{ aabbMin.x, aabbMax.y, aabbMax.z },
		aabbMax,
		{ aabbMax.x, aabbMax.y, aabbMin.z },
		{ aabbMin.x, aabbMax.y, aabbMin.z }
	};

	// 描画
	DrawLine3D(aabbPoints[0], aabbPoints[1], color);
	DrawLine3D(aabbPoints[0], aabbPoints[2], color);
	DrawLine3D(aabbPoints[3], aabbPoints[1], color);
	DrawLine3D(aabbPoints[3], aabbPoints[2], color);
	DrawLine3D(aabbPoints[0], aabbPoints[7], color);
	DrawLine3D(aabbPoints[1], aabbPoints[4], color);
	DrawLine3D(aabbPoints[2], aabbPoints[6], color);
	DrawLine3D(aabbPoints[3], aabbPoints[5], color);
	DrawLine3D(aabbPoints[7], aabbPoints[4], color);
	DrawLine3D(aabbPoints[7], aabbPoints[6], color);
	DrawLine3D(aabbPoints[5], aabbPoints[4], color);
	DrawLine3D(aabbPoints[5], aabbPoints[6], color);

}
