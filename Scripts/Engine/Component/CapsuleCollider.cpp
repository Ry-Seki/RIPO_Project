/*
 *	@file	CapsuleCollider.cpp
 *  @author Riku
 */

#include "CapsuleCollider.h"
#include "../GameObject.h"

/*
 *	更新処理
 */
void CapsuleCollider::Update(float deltaTime) {
	isHit = false;
}

/*
 *	衝突が起きたときに呼び出される処理
 *	@param	const std::shared_ptr<Component&> self	衝突した自分のコライダー
 *  @param	const std::shared_ptr<Component&> other	衝突した相手のコライダー
 */
void CapsuleCollider::OnCollision(
	const std::shared_ptr<Component>& self,
	const std::shared_ptr<Component>& other) {
	isHit = true;
}

/*
 *	デバッグ描画関数
 */
void CapsuleCollider::DebugRender() {
	auto owner = GetOwner();
	// ワールド座標のカプセル
	VECTOR capsuleStart = ToVECTOR(capsule.segment.startPoint + owner->position);
	VECTOR capsuleEnd = ToVECTOR(capsule.segment.endPoint + owner->position);
	float radius = capsule.radius;

	// hit判定によるカラー変更
	unsigned int color;
	if (isHit) {
		color = GetColor(0, 128, 0);
	}
	else {
		color = GetColor(255, 255, 255);
	}

	// 描画の瞬間でライティングを消す
	SetUseLighting(false);
	DrawCapsule3D(capsuleStart, capsuleEnd, radius, 16, color, color, FALSE);
	SetUseLighting(true);
}
