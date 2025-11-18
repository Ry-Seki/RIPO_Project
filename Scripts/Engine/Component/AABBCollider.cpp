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
	auto owner = GetOwner();
	// ワールド座標のAABB
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

	// hit判定によるカラー変更
	unsigned int color;
	if (isHit) {
		color = GetColor( 0, 128, 0 );
	}
	else {
		color = GetColor( 255, 255, 255 );
	}

	// 描画
	DrawLine3D(aabbPoits[0], aabbPoits[1], color);
	DrawLine3D(aabbPoits[0], aabbPoits[2], color);
	DrawLine3D(aabbPoits[3], aabbPoits[1], color);
	DrawLine3D(aabbPoits[3], aabbPoits[2], color);
	DrawLine3D(aabbPoits[0], aabbPoits[7], color);
	DrawLine3D(aabbPoits[1], aabbPoits[4], color);
	DrawLine3D(aabbPoits[2], aabbPoits[6], color);
	DrawLine3D(aabbPoits[3], aabbPoits[5], color);
	DrawLine3D(aabbPoits[7], aabbPoits[4], color);
	DrawLine3D(aabbPoits[7], aabbPoits[6], color);
	DrawLine3D(aabbPoits[5], aabbPoits[4], color);
	DrawLine3D(aabbPoits[5], aabbPoits[6], color);

}
